#pragma once
#include "common/utils.hpp"

namespace BootManager
{
    constexpr const char* TAG = "BootManager";

    /**
     * @brief Checks for special boot state, if any, runs the appropriate boot sequence.
     * @returns boolean true if special state is detected, else false.
     * @note If this function returns true, the standard booting sequence should be aborted.
     */
    bool CheckForSpecialBoot();

    bool boot_ZERO_CALIB_needed();

    bool boot_CALIBRATION_needed();

    bool boot_UPDATE_needed();

    /// @brief INTERNAL, boot sequence in ZERO_CALIB mode
    void boot_ZERO_CALIB();

    /// @brief INTERNAL, boot sequence in CALIBRATION mode
    void boot_CALIBRATION();

    /// @brief INTERNAL, boot sequence in UPDATE mode
    void boot_UPDATE();
}