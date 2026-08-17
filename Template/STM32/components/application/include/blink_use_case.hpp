#pragma once

#include "abstraction/gpio.hpp"
#include <stdint.h>

namespace application
{
    class BlinkUseCase
    {
    public:
        BlinkUseCase(abstraction::GPIO &led) : m_led(led){}
        void init();
        void run();

    private:
        static void delay_cycles(volatile uint32_t cycles);
        abstraction::GPIO &m_led;
    };
}
