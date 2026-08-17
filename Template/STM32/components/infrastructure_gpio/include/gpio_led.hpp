#pragma once

#include "gpio.hpp"
#include <stdint.h>

namespace infrastructure_gpio
{
    class GPIO_LED : public abstraction::GPIO
    {
    public:
        GPIO_LED() = default;
        //Non-virtual destructor is not allowed in base class, so we can use default destructor in derived class
        ~GPIO_LED() = default;

        void init() override;
        void set() override;
        void clear() override;
        void toggle() override;

    private:
        uint32_t m_port_base;
        uint32_t m_pin;
    };
}