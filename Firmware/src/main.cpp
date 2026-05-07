#include <freertos/FreeRTOS.h>
#include "common/Log.hpp"
#include "Robot.hpp"
#include "common/config.hpp"
#include "common/RPC.hpp"
#include "boot/BootManager.hpp"

static const char* TAG = "Main";

static Robot robot;
CameraDriver cam;
WiFiManager wifi;

#ifdef __cplusplus
extern "C" {
#endif
void app_main()
{
    // Check for special boot state (zero-calibration, pending update, etc.)
    if (BootManager::CheckForSpecialBoot())
    {
        // Special boot state detected.
        // The corresponding boot will be handled by the BootManager.
        // just return to avoid booting as normal (should not happen anyway)
        return;
    }

    LOG_INFO(TAG, "Initializing robot (FIRMWARE_VERSION=%s) ...", FIRMWARE_VERSION);

    if (Error err = robot.init(); err != Error::None)
    {
        LOG_ERROR(TAG, "Failed to initialize robot. Error : %s", ErrorToString(err));
        return;
    }

    LOG_INFO(TAG, "Robot initialized successfully.");

    // TEMPORARY : To avoid impossible position errors
    robot.getDecisionLoop().askBodyPosition(0, 0, DEFAULT_BODY_HEIGHT_M);
    robot.getDecisionLoop().askBodyRotation(0, 0, 0);
    robot.getDecisionLoop().askBodyVelocity(0, 0, 0);
    
    LOG_INFO(TAG, "Starting robot ...");

    if (Error err = robot.start(); err != Error::None)
    {
        LOG_ERROR(TAG, "Failed to start robot. Error : %s", ErrorToString(err));
        return;
    }

    LOG_INFO(TAG, "Robot started successfully !");

    // RPC::ExecuteThreadSafe<Error>([](){
    //     return Robot::GetInstance().getBody().enable();
    // }, [](Error err){});
    
    LOG_INFO(TAG, ">>> Robot is operational.");
}
#ifdef __cplusplus
}
#endif