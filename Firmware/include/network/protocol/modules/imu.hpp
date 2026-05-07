#pragma once
#include "network/protocol/Protocol.hpp"
#include <esp_system.h>

namespace Protocol
{
namespace IMU
{
    constexpr uint8_t MODULE_ID = 0x07;

    static void GetAcceleration(const RequestContext& ctx, const uint8_t* payload)
    {
        ctx.respond(ResponseStatus::Ok);
        // TODO : Implement
    }

    static void GetAngularVelocity(const RequestContext& ctx, const uint8_t* payload)
    {
        ctx.respond(ResponseStatus::Ok);
        // TODO : Implement
    }

    static void GetDownVector(const RequestContext& ctx, const uint8_t* payload)
    {
        ctx.respond(ResponseStatus::Ok);
        // TODO : Implement
    }

    static void GetOrientation(const RequestContext& ctx, const uint8_t* payload)
    {
        ctx.respond(ResponseStatus::Ok);
        // TODO : Implement
    }

    static void GetCalibrationState(const RequestContext& ctx, const uint8_t* payload)
    {
        ctx.respond(ResponseStatus::Ok);
        // TODO : Implement
    }

    static void GetCalibrationData(const RequestContext& ctx, const uint8_t* payload)
    {
        ctx.respond(ResponseStatus::Ok);
        // TODO : Implement
    }

    static void SetCalibrationData(const RequestContext& ctx, const uint8_t* payload)
    {
        ctx.respond(ResponseStatus::Ok);
        // TODO : Implement
    }

    static void StartCalibration(const RequestContext& ctx, const uint8_t* payload)
    {
        ctx.respond(ResponseStatus::Ok);
        // TODO : Implement
    }

    static void StopCalibration(const RequestContext& ctx, const uint8_t* payload)
    {
        ctx.respond(ResponseStatus::Ok);
        // TODO : Implement
    }

    static void GetCalibrationProgress(const RequestContext& ctx, const uint8_t* payload)
    {
        ctx.respond(ResponseStatus::Ok);
        // TODO : Implement
    }


    static ActionCallback actions[] = {
        GetAcceleration,           // 0x00
        GetAngularVelocity,        // 0x01
        GetDownVector,             // 0x02
        GetOrientation,            // 0x03
        GetCalibrationState,       // 0x04
        GetCalibrationData,        // 0x05
        SetCalibrationData,        // 0x06
        StartCalibration,          // 0x07
        StopCalibration,           // 0x08
        GetCalibrationProgress,    // 0x09
    };

    static void Register(Dispatcher& dispatcher)
    {
        dispatcher.registerModule(MODULE_ID, actions, sizeof(actions));
    }
}
}