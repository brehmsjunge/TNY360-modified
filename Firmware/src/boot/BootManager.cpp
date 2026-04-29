#include "boot/BootManager.hpp"
#include "common/Log.hpp"
#include "common/NVS.hpp"

namespace BootManager
{
    bool CheckForSpecialBoot()
    {
        LOG_SCOPE(TAG, "BootManager::CheckForSpecialBoot");

        // Init NVS to check for flags
        if (Error err = NVS::Init(); err != Error::None)
        {
            LOG_ERROR(TAG, "Error initializing NVS : %s", ErrorToString(err));
            return true; // block robot from booting as normal
        }

        NVS::Handle* nvsHandle;
        if (Error err = NVS::Open("boot", &nvsHandle); err != Error::None)
        {
            LOG_ERROR(TAG, "Error opening NVS 'boot' namespace : %s", ErrorToString(err));
            return true; // block robot from booting as normal
        }

        // Check for zero-calibration flag
        bool zeroCalibrated;
        if (Error err = nvsHandle->get<bool>("zero-calib", zeroCalibrated); err != Error::None)
        {
            if (err == Error::NotFound) // not found ? normal if first boot ever
            {
                zeroCalibrated = false;
            }
            else // something went wrong
            {
                LOG_ERROR(TAG, "Failed to get 'zero-calib' flag from NVS : %s", ErrorToString(err));
                return true; // block robot from booting as normal
            }
        }

        NVS::Close(nvsHandle);

        if (!zeroCalibrated)
        {
            LOG_INFO(TAG, "Zero-Calibration flag missing. Starting in ZERO_CALIB boot mode");
            boot_ZERO_CALIB();
            return true; // don't boot as normal
        }

        if (boot_UPDATE_needed())
        {
            LOG_INFO(TAG, "Update is pending. Starting in UPDATE boot mode");
            boot_UPDATE();
            return true; // don't boot as normal
        }
        
        return false; // robot can proceed as normal
    }
}