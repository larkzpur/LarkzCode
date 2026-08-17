#pragma once

#include "gpio_led.hpp"
#include <stdint.h>

namespace application
{
    class BlinkUseCase
    {
    public:
        BlinkUseCase(infrastructure_gpio::GPIO_LED &led) : m_led(led) {}
        void init();
        void run();
        void alt_run();

    private:
        static void delay_cycles(volatile uint32_t cycles);
        infrastructure_gpio::GPIO_LED &m_led;
    };
}
