#pragma once
#include "network/protocol/Protocol.hpp"
#include <esp_system.h>

namespace Protocol
{
namespace Protocol
{
    constexpr uint8_t MODULE_ID = 0x01;

    static void SetStreamFrequency(const RequestContext& ctx, const uint8_t* payload)
    {
        ctx.respond(ResponseStatus::Ok);
        // TODO : Implement
    }

    static void SetStreamFlags(const RequestContext& ctx, const uint8_t* payload)
    {
        ctx.respond(ResponseStatus::Ok);
        // TODO : Implement
    }


    static ActionCallback actions[] = {
        SetStreamFrequency,        // 0x00
        SetStreamFlags,            // 0x01
    };

    static void Register(Dispatcher& dispatcher)
    {
        dispatcher.registerModule(MODULE_ID, actions, sizeof(actions));
    }
}
}