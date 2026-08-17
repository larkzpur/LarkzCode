#pragma once

#include <stdint.h>

namespace board_config
{
    //NUCLE-F411RE on-board LED
    constexpr uint32_t LED_PORT_BASE = 0x40020000UL;    //GPIOA base
    constexpr uint32_t LED_PIN = 5U;                      //PA5

    //RCC
    constexpr uint32_t RCC_BASE = 0x40023800UL;          //RCC base
    constexpr uint32_t RCC_AHB1ENR_OFFSET = 0x30UL;      //RCC AHB1 peripheral clock enable register offset
    constexpr uint32_t RCC_AHB1ENR_GPIOA = (1U << 0);    //GPIOA clock enable bit

    //GPIO registers
    constexpr uint32_t GPIO_MODER_OFFSET = 0x00UL;       //GPIO port mode register offset
    constexpr uint32_t GPIO_ODR_OFFSET = 0x14UL;        //GPIO port output data register offset

    // Delay cycles for ~1 second at 84 MHz when each loop iteration takes ~6 cycles.
    // Approximation: (84,000,000 / 6) ~= 14,000,000 iterations.
    constexpr uint32_t DELAY_1_SECOND = 14000000UL; // ~1 second delay cycles
}