#pragma once
#include "network/protocol/Protocol.hpp"
#include <esp_system.h>

namespace Protocol
{
namespace System
{
    constexpr uint8_t MODULE_ID = 0x00;

    static void Ping(const RequestContext& ctx, const uint8_t* payload)
    {
        ctx.respond(ResponseStatus::Ok);
    }

    static void Reboot(const RequestContext& ctx, const uint8_t* payload)
    {
        ctx.respond(ResponseStatus::Ok);
        esp_restart();
    }

    static void SetSettings(const RequestContext& ctx, const uint8_t* payload)
    {
        ctx.respond(ResponseStatus::Ok);
        // TODO : Implement
    }

    static void GetSettings(const RequestContext& ctx, const uint8_t* payload)
    {
        ctx.respond(ResponseStatus::Ok);
        // TODO : Implement
    }

    static void SetAutolifeLevel(const RequestContext& ctx, const uint8_t* payload)
    {
        ctx.respond(ResponseStatus::Ok);
        // TODO : Implement
    }

    static void GetAutolifeLevel(const RequestContext& ctx, const uint8_t* payload)
    {
        ctx.respond(ResponseStatus::Ok);
        // TODO : Implement
    }


    static ActionCallback actions[] = {
        Ping,                      // 0x00
        Reboot,                    // 0x01
        SetSettings,               // 0x02
        GetSettings,               // 0x03
        SetAutolifeLevel,          // 0x04
        GetAutolifeLevel,          // 0x05
    };

    static void Register(Dispatcher& dispatcher)
    {
        dispatcher.registerModule(MODULE_ID, actions, sizeof(actions));
    }
}
}