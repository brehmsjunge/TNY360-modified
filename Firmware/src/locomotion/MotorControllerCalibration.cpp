#include "locomotion/MotorController.hpp"
#include "common/Log.hpp"
#include "locomotion/calibration/get_feedback_noise.hpp"
#include "locomotion/calibration/check_feedback_inversion.hpp"
#include "locomotion/calibration/get_feedback_latency.hpp"

constexpr const char* TAG = "MtrCtrl-Calib";

Error MotorController::run_calibration_sequence()
{
    MotorDriver::Value pwm_center = (motor_attributes.pwm_min + motor_attributes.pwm_max) / 2;

    AnalogDriver::Value noise_mV;
    bool feedback_inverted;
    int feedback_latency;

    LOG_SCOPE(TAG, "Starting Motor Calibration");
    this->calibration_progress = 0.0f;

    // Disable all motors
    LOG_DEBUG(TAG, "Disabling all motors");
    {
        for (MotorDriver::Channel i = 0; i < MotorDriver::CHANNEL_COUNT; i++)
        {
            RETURN_ERROR(MotorDriver::SetPWM(i, 0));
        }
        RETURN_ERROR(MotorDriver::SendData());
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    this->calibration_progress = 0.1f;

    // Move motor to center
    LOG_DEBUG(TAG, "Moving to center");
    {
        RETURN_ERROR(MotorDriver::SetPWM(motor_channel, pwm_center))
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    this->calibration_progress = 0.2f;

    // Get motor feedback noise
    LOG_DEBUG(TAG, "Getting feedback noise");
    {
        FeedbackNoiseParams params;
        if (Error err = get_feedback_noise(params, analog_channel, noise_mV); err != Error::None)
        {
            LOG_ERROR(TAG, "Feedback noise detection failed with error [%s]", ErrorToString(err));
            return Error::Unknown;
        }
        LOG_DEBUG(TAG, "==> Feedback noise is %dmV", noise_mV);
    }
    this->calibration_progress = 0.3f;

    // Check feedback inversion
    LOG_DEBUG(TAG, "Checking feedback inversion");
    {
        FeedbackInversionParams params;
        params.pwm_center = pwm_center;
        params.pwm_center = MotorDriver::MS_TO_PWM(0.2);
        params.feedback_noise = noise_mV;
        if (Error err = check_feedback_inversion(params, motor_channel, analog_channel, feedback_inverted); err != Error::None)
        {
            LOG_ERROR(TAG, "Feedback invertion detection failed with error [%s]", ErrorToString(err));
            return Error::Unknown;
        }
        LOG_DEBUG(TAG, "==> Feedback inverted : %s", feedback_inverted? "true": "false");
    }
    this->calibration_progress = 0.4f;

    // Test feedback latency
    LOG_DEBUG(TAG, "Estimating feedback latency");
    {
        FeedbackLatencyParams params;
        params.pwm_center = pwm_center;
        params.pwm_center = MotorDriver::MS_TO_PWM(0.2);
        params.feedback_noise = noise_mV;
        if (Error err = get_feedback_latency(params, motor_channel, analog_channel, feedback_latency); err != Error::None)
        {
            LOG_ERROR(TAG, "Feedback latency estimation failed with error [%s]", ErrorToString(err));
            return Error::Unknown;
        }
        LOG_DEBUG(TAG, "==> Feedback inverted : %s", feedback_inverted? "true": "false");
    }
    this->calibration_progress = 0.5f;

    // Test deadband size
    LOG_DEBUG(TAG, "LOG");
    {

    }
    this->calibration_progress = 0.6f;

    // Test min bound
    LOG_DEBUG(TAG, "LOG");
    {

    }
    this->calibration_progress = 0.7f;

    // Test max bound
    LOG_DEBUG(TAG, "LOG");
    {

    }
    this->calibration_progress = 0.8f;

    // Test max speed
    LOG_DEBUG(TAG, "LOG");
    {

    }
    this->calibration_progress = 0.9f;

    // Save calibration data
    LOG_DEBUG(TAG, "LOG");
    {

    }
    this->calibration_progress = 1.0f;

    return Error::None;
}