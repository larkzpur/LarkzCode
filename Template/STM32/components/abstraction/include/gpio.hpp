#pragma once

#include <stdint.h>

namespace abstraction
{
    class GPIO
    {
    public:
        ~GPIO() = default;

        virtual void init() = 0;
        virtual void set() = 0;
        virtual void clear() = 0;
        virtual void toggle() = 0;
    };
}
