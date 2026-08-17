#include "gpio_led.hpp"
#include "config.hpp"

namespace infrastructure_gpio
{
    void GPIO_LED::init()
    {
        //Enable GPIOA clock
        volatile uint32_t *rcc_ahb1enr = reinterpret_cast<volatile uint32_t *>(board_config::RCC_BASE + board_config::RCC_AHB1ENR_OFFSET);
        *rcc_ahb1enr |= board_config::RCC_AHB1ENR_GPIOA;

        //Set PA5 as output (01 in MODER register)
        volatile uint32_t *gpio_moder = reinterpret_cast<volatile uint32_t *>(board_config::LED_PORT_BASE + board_config::GPIO_MODER_OFFSET);

        *gpio_moder &= ~(0x3UL << (board_config::LED_PIN * 2)); // Clear mode bits for PA5
        *gpio_moder |= (0x1UL << (board_config::LED_PIN * 2));  // Set mode to output for PA5
    }

    void GPIO_LED::set()
    {
        volatile uint32_t *gpio_odr = reinterpret_cast<volatile uint32_t *>(board_config::LED_PORT_BASE + board_config::GPIO_ODR_OFFSET);
        *gpio_odr |= (1UL << board_config::LED_PIN); // Set PA5 high
    }

    void GPIO_LED::clear()
    {
        volatile uint32_t *gpio_odr = reinterpret_cast<volatile uint32_t *>(board_config::LED_PORT_BASE + board_config::GPIO_ODR_OFFSET);
        *gpio_odr &= ~(1UL << board_config::LED_PIN); // Set PA5 low
    }

    void GPIO_LED::toggle()
    {
        volatile uint32_t *gpio_odr = reinterpret_cast<volatile uint32_t *>(board_config::LED_PORT_BASE + board_config::GPIO_ODR_OFFSET);
        *gpio_odr ^= (1UL << board_config::LED_PIN); // Toggle PA5
    }
}