#include "network/NetworkManager.hpp"
#include "network/SerialTransport.hpp"
#include "network/protocol/Protocol.hpp"
#include "common/Log.hpp"
#include "Robot.hpp"

static SerialTransport serial_transport;
NetworkManager::NetworkManager()
{
}

Error NetworkManager::init()
{
    LOG_SCOPE(TAG, "NetworkManager::init");

    // initialize the protocol dispatcher (required for all transports)
    if (Error err = Protocol::Init(); err != Error::None)
    {
        return err;
    }

    // initialize USB serial transport (replaces WiFi + WebSocket)
    if (Error err = serial_transport.init(); err != Error::None)
    {
        LOG_ERROR(TAG, "Failed to initialize SerialTransport");
        return err;
    }

    return Error::None;
}

Error NetworkManager::deinit()
{
    // stop the serial transport and release UART
    if (Error err = serial_transport.deinit(); err != Error::None)
    {
        return err;
    }

    // deinitialize the protocol dispatcher
    if (Error err = Protocol::Deinit(); err != Error::None)
    {
        return err;
    }

    return Error::None;
}