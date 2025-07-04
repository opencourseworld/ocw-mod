#pragma once

#include <common.hpp>
#include "binaryoffsethelper.hpp"
#include "oe/oe_displayversion.hpp"

namespace exl::util {

    /* Add versions here if you want to take advantage of this feature. */
    enum class UserVersion {
        DEFAULT,
        /* Members can be assigned any constant of your choosing. */
        V3_0_2 = 0xF302,
    };

    namespace impl {
        ALWAYS_INLINE UserVersion DetermineUserVersion() {
            return UserVersion::V3_0_2;
            // /* 
            //     Implement a method of determining your application version here if you want to take advantage of this feature.
            //     An example may be apply Murmur3 over the .text and .rodata of the main executable or calling nn::oe::GetDisplayVersion.
            //     The former would have complications if other mods are in effect, and the latter may have issues with people doing safe downgrades on console.
            // */
            // nn::oe::DisplayVersion displayVersion;
            // nn::oe::GetDisplayVersion(&displayVersion);

            // if (strncmp(displayVersion.name, "3.0.2", 6) == 0) {
            //     return UserVersion::V3_0_2;
            // } else {
            //     return UserVersion::DEFAULT;
            // }
        }
    }
}
