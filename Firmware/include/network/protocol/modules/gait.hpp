#pragma once
#include "network/protocol/Protocol.hpp"
#include "common/BinaryReader.hpp"
#include "Robot.hpp"
#include <esp_system.h>

namespace Protocol
{
namespace Gait
{
    constexpr uint8_t MODULE_ID = 0x02;

    static void SetType(const RequestContext& ctx, const uint8_t* payload)
    {
        BinaryReader reader(payload, ctx.expected_len);

        uint8_t gaitType;
        if (reader.read(gaitType) != Error::None)
        {
            ctx.respond(ResponseStatus::InvalidParameters);
            return;
        }

        Robot::GetInstance().getDecisionLoop().askGaitType((::GaitPlanner::GaitType) gaitType);
        ctx.respond(ResponseStatus::Ok);
    }

    static void GetType(const RequestContext& ctx, const uint8_t* payload)
    {
        BinaryReader reader(payload, ctx.expected_len);

        uint8_t gaitType = (uint8_t) Robot::GetInstance().getDecisionLoop().getControlIntent().gait;
        ctx.respond(ResponseStatus::Ok, (uint8_t*) &gaitType, sizeof(gaitType));
    }


    static ActionCallback actions[] = {
        SetType,                   // 0x00
        GetType,                   // 0x01
    };

    static void Register(Dispatcher& dispatcher)
    {
        dispatcher.registerModule(MODULE_ID, actions, sizeof(actions));
    }
}
}