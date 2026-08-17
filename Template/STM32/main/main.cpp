#include "platform/system.hpp"
#include "infrastructure_gpio/gpio_led.hpp"
#include "application/blink_use_case.hpp"

int main(void)
{
    // Initialize the system (clock, etc.)
    SystemInit();

    // Create instances of GPIO_LED and BlinkUseCase 
    infrastructrure_gpio::GPIO_LED led;

    // Create an instance of BlinkUseCase with the LED
    application::BlinkUseCase blinkUseCase(led);

    // Initialize the LED and start the blinking use case
    blinkUseCase.init();
    blinkUseCase.run();

    // The program should never reach this point, but return 0 to satisfy the compiler
    return 0;
}