#include "network/SerialTransport.hpp"
#include "network/protocol/Protocol.hpp"
#include "common/Log.hpp"
#include "driver/uart.h"
#include <string.h>
#include <stdlib.h>

#define SERIAL_BUF_SIZE   (4096)             // receive/send buffer size in bytes
#define SERIAL_STACK_SIZE (8192)             // stack memory for the RX task
#define SERIAL_TASK_PRIO  (tskIDLE_PRIORITY + 5) // medium priority, below control loop
#define SERIAL_TASK_CORE  (1)                // run on Core 1 (reflex/control side)

Error SerialTransport::init()
{
    LOG_SCOPE(TAG, "SerialTransport::init");

    // configure UART parameters
    uart_config_t uart_config = {
        .baud_rate  = BAUDRATE,                  // 921600 baud transmission speed
        .data_bits  = UART_DATA_8_BITS,          // 8 bits per character (standard)
        .parity     = UART_PARITY_DISABLE,       // no bit-level error checking needed
        .stop_bits  = UART_STOP_BITS_1,          // 1 stop bit (standard)
        .flow_ctrl  = UART_HW_FLOWCTRL_DISABLE,  // no hardware handshake over USB
    };

    // apply UART configuration to the driver
    if (uart_param_config(UART_PORT, &uart_config) != ESP_OK)
    {
        LOG_ERROR(TAG, "Failed to configure UART");
        return Error::SoftwareFailure;
    }

    // install UART driver with TX and RX buffers
    if (uart_driver_install(UART_PORT, SERIAL_BUF_SIZE, SERIAL_BUF_SIZE, 0, nullptr, 0) != ESP_OK)
    {
        LOG_ERROR(TAG, "Failed to install UART driver");
        return Error::SoftwareFailure;
    }

    // set flag so the RX task knows it should keep running
    running = true;

    // start the background task that listens for incoming USB packets
    BaseType_t err = xTaskCreatePinnedToCore(
        rx_task_fn,       // function to run
        "SerialRX",       // task name for debugging
        SERIAL_STACK_SIZE, // stack memory
        this,             // pass current instance so the task can access our variables
        SERIAL_TASK_PRIO, // priority
        &rx_task,         // FreeRTOS stores the task handle here
        SERIAL_TASK_CORE  // pin to Core 1
    );

    if (err != pdPASS)
    {
        running = false;
        LOG_ERROR(TAG, "Failed to create SerialRX task");
        return Error::Unknown;
    }

    LOG_INFO(TAG, "SerialTransport ready @ %u baud", BAUDRATE);
    return Error::None;
}

Error SerialTransport::deinit()
{
    // tell the RX task to stop looping
    running = false;

    if (rx_task != nullptr)
    {
        vTaskDelay(pdMS_TO_TICKS(100)); // wait 100ms so the task can finish its current loop
        vTaskDelete(rx_task);           // delete the FreeRTOS task
        rx_task = nullptr;              // reset handle so we know no task is running
    }

    uart_driver_delete(UART_PORT); // release the UART driver
    return Error::None;
}

// FreeRTOS needs a static function as task entry point
// we passed 'this' as arg so we can get back to our instance
void SerialTransport::rx_task_fn(void* arg)
{
    static_cast<SerialTransport*>(arg)->receive_loop();
}

void SerialTransport::receive_loop()
{
    static uint8_t buf[SERIAL_BUF_SIZE]; // static so it's not reallocated every loop

    while (running)
    {
        // wait for magic byte 0xAA — start of a packet
        uint8_t byte;
        if (uart_read_bytes(UART_PORT, &byte, 1, pdMS_TO_TICKS(10)) < 1) continue;
        if (byte != MAGIC_0) continue;

        // wait for magic byte 0xBB — confirms this is really a packet start
        if (uart_read_bytes(UART_PORT, &byte, 1, pdMS_TO_TICKS(10)) < 1) continue;
        if (byte != MAGIC_1) continue;

        // read 2-byte packet length (little-endian)
        uint16_t total_len = 0;
        if (uart_read_bytes(UART_PORT, (uint8_t*)&total_len, 2, pdMS_TO_TICKS(20)) < 2) continue;

        // sanity check: length must be at least a header and fit in our buffer
        if (total_len < sizeof(Protocol::MessageHeader) || total_len > SERIAL_BUF_SIZE)
        {
            LOG_WARNING(TAG, "Invalid packet length: %u", total_len);
            continue;
        }

        // read the full packet into the buffer
        int received = uart_read_bytes(UART_PORT, buf, total_len, pdMS_TO_TICKS(100));
        if (received < (int)total_len)
        {
            LOG_WARNING(TAG, "Incomplete packet (%d / %u bytes)", received, total_len);
            continue;
        }

        // hand the complete packet to the T360 protocol dispatcher
        // it will figure out what to do (movement command, sensor query, etc.)
        Protocol::GetDispatcher().handlePacket(this, nullptr, buf, total_len);
    }

    // running == false → task ends itself cleanly
    vTaskDelete(nullptr);
}

void SerialTransport::sendResponse(void* context, const Protocol::MessageHeader& header, const uint8_t* payload)
{
    // calculate total packet size: header + payload
    uint16_t total_len = sizeof(Protocol::MessageHeader) + header.length;

    // build the 4 framing bytes: magic 0xAA 0xBB + length in little-endian
    uint8_t frame_header[4] = {
        MAGIC_0,
        MAGIC_1,
        (uint8_t)(total_len & 0xFF),         // lower byte of length
        (uint8_t)((total_len >> 8) & 0xFF)   // upper byte of length
    };

    // send framing bytes, then header, then payload
    uart_write_bytes(UART_PORT, (const char*)frame_header, 4);
    uart_write_bytes(UART_PORT, (const char*)&header, sizeof(Protocol::MessageHeader));
    if (payload != nullptr && header.length > 0) // only send payload if there is one
    {
        uart_write_bytes(UART_PORT, (const char*)payload, header.length);
    }
}