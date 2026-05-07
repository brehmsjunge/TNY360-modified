#pragma once
#include <freertos/FreeRTOS.h>
#include "common/utils.hpp"
#include "drivers/AnalogDriver.hpp"

struct FeedbackNoiseParams
{
    /// @brief Number of samples to use for noise detection
    uint16_t nb_samples = 10;
    /// @brief Wait delay between each noise sample
    uint16_t sample_delay_ms = 50;
};

/**
 * @brief Get a motor feedback noise in mV
 * @param params Noise detection parameters
 * @param channel AnalogDriver channel to sample
 * @param out_value [out] Final noise value output variable
 * @note Returned noise value is the absolute difference between the minimum and maximum sample
 */
Error get_feedback_noise(FeedbackNoiseParams params, AnalogDriver::Channel channel, AnalogDriver::Value& out_value)
{
    AnalogDriver::Value values[params.nb_samples];

    // Read the voltages
    for (uint16_t i = 0; i < params.nb_samples; i++)
    {
        RETURN_ERROR(AnalogDriver::ReadAllChannels());
        RETURN_ERROR(AnalogDriver::GetVoltage(channel, values[i]));
        vTaskDelay(pdMS_TO_TICKS(params.sample_delay_ms));
    }

    // Calculate the noise (simple min/max bounds for now)
    // Maybe this could be improved using standard deviation or something else
    AnalogDriver::Value max_value = values[0], min_value = values[0];
    for (uint16_t i = 0; i < params.nb_samples; i++)
    {
        if (values[i] < min_value) min_value = values[i];
        if (values[i] > max_value) max_value = values[i];
    }

    // Return result
    out_value = max_value - min_value;
    return Error::None;
}