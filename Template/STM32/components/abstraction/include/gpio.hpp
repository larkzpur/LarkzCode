#pragma once

#include <stdint.h>

namespace abstraction
{
    class GPIO
    {
    public:
        ~GPIO() = default;

        void init() {}
        void set() {}
        void clear() {}
        void toggle() {}
    };
}
