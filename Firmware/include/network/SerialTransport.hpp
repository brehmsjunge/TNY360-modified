#pragma once

// T360 transport interface — SerialTransport must implement this
#include "network/protocol/Protocol.hpp"
// ESP32 UART driver
#include "driver/uart.h"
// FreeRTOS core
#include "freertos/FreeRTOS.h"
// FreeRTOS task functions (xTaskCreatePinnedToCore, vTaskDelete, etc.)
#include "freertos/task.h"

// USB Serial transport — replaces WebSocket as the dashboard connection
// implements Protocol::ITransport so the dispatcher can use it like any other transport
class SerialTransport : public Protocol::ITransport
{
public:
    constexpr static const char* TAG = "SerialTransport"; // log tag for Serial Monitor output

    constexpr static uint8_t  MAGIC_0  = 0xAA;           // first magic byte — marks packet start
    constexpr static uint8_t  MAGIC_1  = 0xBB;           // second magic byte — confirms packet start
    constexpr static uint32_t BAUDRATE = 921600;          // transmission speed in baud
    constexpr static uart_port_t UART_PORT = UART_NUM_0;  // UART0 = USB-Serial port on ESP32-S3

    Error init();     // starts UART and launches the RX background task
    Error deinit();   // stops the RX task and releases the UART driver

    // called by the dispatcher when the robot wants to send a response to the dashboard
    void sendResponse(void* context, const Protocol::MessageHeader& header, const uint8_t* payload) override;

private:
    TaskHandle_t rx_task = nullptr; // handle to the FreeRTOS RX task, nullptr = not running
    bool running = false;           // flag to control the RX loop — false = stop

    static void rx_task_fn(void* arg); // FreeRTOS entry point (must be static)
    void receive_loop();               // actual receive loop, runs until running == false
    void send_framed(const uint8_t* data, size_t len); // sends data with 0xAA 0xBB framing
};