/*!
 * File HPMSObject.h
 */

#pragma once

#include <string>

namespace hpms
{
    struct Object
    {
        virtual const std::string Name() const = 0;
    };
}
