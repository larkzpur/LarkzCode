#pragma once

#include "gpio.hpp"
#include <stdint.h>

namespace infrastructure_gpio
{
    class GPIO_LED : public abstraction::GPIO
    {
    public:
        GPIO_LED() = default;
        ~GPIO_LED() = default;

        void init();
        void set();
        void clear();
        void toggle();

    private:
        uint32_t m_port_base;
        uint32_t m_pin;
    };
}