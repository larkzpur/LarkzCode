//#include "system.hpp"
#include "gpio_led.hpp"
#include "blink_use_case.hpp"

int main(void)
{
    // Initialize the system (clock, etc.)
    //SystemInit();

    // Create instances of GPIO_LED and BlinkUseCase 
    infrastructure_gpio::GPIO_LED led;

    // Create an instance of BlinkUseCase with the LED
    application::BlinkUseCase blinkUseCase(led);

    // Initialize the LED and start the blinking use case
    blinkUseCase.init();
    blinkUseCase.alt_run();

    // The program should never reach this point, but return 0 to satisfy the compiler
    return 0;
}