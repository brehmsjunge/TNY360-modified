#include "network/WebSocket.hpp"
#include "common/Log.hpp"
#include "Robot.hpp"
#include <esp_wifi.h>

namespace WebSocketUtils
{
    struct AsyncWebsocketResponse {
        httpd_handle_t hd;
        int fd;
        uint8_t* payload;
        size_t len;
    };

    static void ws_async_send_worker(void *arg) {
        AsyncWebsocketResponse* resp = static_cast<AsyncWebsocketResponse*>(arg);
        
        httpd_ws_frame_t ws_pkt;
        memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
        ws_pkt.type = HTTPD_WS_TYPE_BINARY;
        ws_pkt.payload = resp->payload;
        ws_pkt.len = resp->len;
        ws_pkt.final = true;
        
        esp_err_t err = httpd_ws_send_frame_async(resp->hd, resp->fd, &ws_pkt);
        if (err != ESP_OK) {
            LOG_ERROR("WebSocket", "httpd_ws_send_frame_async failed with error 0x%0x", err);
        }
        
        free(resp->payload);
        delete resp;
    }
}

WebSocket::WebSocket(uint16_t port) : server_port(port)
{
}

Error WebSocket::init()
{
    LOG_SCOPE(TAG, "WebSocket::init");
    
    // create the web server
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = server_port;
    config.max_uri_handlers = 8;
    config.ctrl_port = server_port + 1;
    config.max_open_sockets = 3;
    // config.lru_purge_enable = true;

    // start the server
    if (httpd_start(&server_handle, &config) != ESP_OK)
    {
        server_handle = nullptr;
        LOG_ERROR(TAG, "Failed to start WebSocket server");
        return Error::Unknown;
    }

    // register URI handlers and return
    return register_uri_handlers();
}

Error WebSocket::deinit()
{
    if (server_handle)
    {
        httpd_stop(server_handle);
        server_handle = nullptr;
    }
    return Error::None;
}

Error WebSocket::register_uri_handlers()
{
    httpd_uri_t ws = {
        .uri        = "/",
        .method     = HTTP_GET,
        .handler    = [](httpd_req_t* req) {
            return Robot::GetInstance().getNetworkManager().getWebSocket().ws_handler(req);
        },
        .user_ctx   = NULL,
        .is_websocket = true,
        .handle_ws_control_frames = false,
        .supported_subprotocol = ""
    };
    httpd_register_uri_handler((httpd_handle_t) server_handle, &ws);

    esp_event_handler_register(ESP_HTTP_SERVER_EVENT, HTTP_SERVER_EVENT_ON_CONNECTED,
        [](void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data){
            Robot::GetInstance().getNetworkManager().getWebSocket().on_connected();
        }, nullptr
    );
    esp_event_handler_register(ESP_HTTP_SERVER_EVENT, HTTP_SERVER_EVENT_DISCONNECTED,
        [](void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data){
            Robot::GetInstance().getNetworkManager().getWebSocket().on_disconnected();
        }, nullptr
    );

    return Error::None;
}

void WebSocket::on_connected()
{
    nb_connected_clients++;
    if (nb_connected_clients == 1) {
        // First client connected, disable low power mode
        LOG_DEBUG(TAG, "Client connected, disabling Wi-Fi low power mode.");
        esp_wifi_set_ps(WIFI_PS_NONE);
    }
}

void WebSocket::on_disconnected()
{
    nb_connected_clients--;
    if (nb_connected_clients < 0) {
        nb_connected_clients = 0; // Ensure the count doesn't go negative
    }

    if (nb_connected_clients == 0) {
        // No client connected, activate low power mode
        LOG_DEBUG(TAG, "No clients connected, switching Wi-Fi to low power mode.");
        esp_wifi_set_ps(WIFI_PS_MIN_MODEM);
    }
}

void WebSocket::sendResponse(void* context, const Protocol::MessageHeader& header, const uint8_t* payload)
{
    int fd = (int)(uintptr_t)context;

    size_t total_len = sizeof(Protocol::MessageHeader) + header.length;

    uint8_t* packet = (uint8_t*)malloc(total_len);
    if (!packet) {
        LOG_ERROR(TAG, "WebSocket::sendResponse malloc failed");
        return;
    }

    memcpy(packet, &header, sizeof(Protocol::MessageHeader));
    if (header.length > 0 && payload != nullptr) {
        memcpy(packet + sizeof(Protocol::MessageHeader), payload, header.length);
    }

    WebSocketUtils::AsyncWebsocketResponse* work_arg = new WebSocketUtils::AsyncWebsocketResponse{
        .hd = this->server_handle,
        .fd = fd,
        .payload = packet,
        .len = total_len
    };

    if (esp_err_t err = httpd_queue_work(this->server_handle, WebSocketUtils::ws_async_send_worker, work_arg); err != ESP_OK) {
        LOG_ERROR(TAG, "httpd_queue_work failed with error 0x%0x", err);
        free(packet);
        delete work_arg;
    }
}

esp_err_t WebSocket::ws_handler(httpd_req_t* ws_req)
{
    if (ws_req->method == HTTP_GET) {
        // Handshake request, nothing to do
        return ESP_OK;
    }

    httpd_ws_frame_t ws_pkt;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    
    esp_err_t ret = httpd_ws_recv_frame(ws_req, &ws_pkt, 0);
    if (ret != ESP_OK) {
        LOG_ERROR(TAG, "httpd_ws_recv_frame failed with error 0x%0x", ret);
        return ret;
    }

    if (ws_pkt.len == 0) {
        LOG_DEBUG(TAG, "Empty WebSocket frame received");
        return ESP_OK;
    }

    if (ws_pkt.len > WEBSOCKET_MAX_MSG_SIZE) {
        LOG_WARNING(TAG, "WebSocket message too large: %d bytes", ws_pkt.len);
        return ESP_ERR_NO_MEM;
    }

    ws_pkt.payload = (uint8_t*) malloc(ws_pkt.len + 1);
    if (!ws_pkt.payload) {
        LOG_ERROR(TAG, "WebSocket::ws_handler malloc failed");
        return ESP_ERR_NO_MEM;
    }

    ret = httpd_ws_recv_frame(ws_req, &ws_pkt, ws_pkt.len);
    if (ret != ESP_OK) {
        LOG_ERROR(TAG, "httpd_ws_recv_frame failed with error 0x%X", ret);
        free(ws_pkt.payload);
        return ESP_FAIL;
    }

    ws_pkt.payload[ws_pkt.len] = 0;  // Null-terminate the payload for safety
    
    int fd = httpd_req_to_sockfd(ws_req);
    Protocol::GetDispatcher().handlePacket(this, (void*)(uintptr_t)fd, ws_pkt.payload, ws_pkt.len);

    free(ws_pkt.payload);
    return ESP_OK;
}
