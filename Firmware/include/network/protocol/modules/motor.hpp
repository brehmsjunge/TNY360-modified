#pragma once
#include "network/protocol/Protocol.hpp"
#include <esp_system.h>

namespace Protocol
{
namespace Motor
{
    constexpr uint8_t MODULE_ID = 0x06;

    static void SetPWMDutyCycle(const RequestContext& ctx, const uint8_t* payload)
    {
        ctx.respond(ResponseStatus::Ok);
        // TODO : Implement
    }

    static void GetPWMDutyCycle(const RequestContext& ctx, const uint8_t* payload)
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
        SetPWMDutyCycle,           // 0x00
        GetPWMDutyCycle,           // 0x01
        GetCalibrationState,       // 0x02
        GetCalibrationData,        // 0x03
        SetCalibrationData,        // 0x04
        StartCalibration,          // 0x05
        StopCalibration,           // 0x06
        GetCalibrationProgress,    // 0x07
    };

    static void Register(Dispatcher& dispatcher)
    {
        dispatcher.registerModule(MODULE_ID, actions, sizeof(actions));
    }
}
}