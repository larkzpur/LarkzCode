#include "application/blink_use_case.hpp"
#include "board_config/config.hpp"

namespace application
{
    void BlinkUseCase::init()
    {
        m_led.init();
    }

    void BlinkUseCase::run()
    {
        for(;;)
        {
            m_led.toggle();
        }
    }

    void BlinkUseCase::delay_cycles(volatile uint32_t cycles)
    {
        while (cycles--)
        {
            __asm("nop");
        }
    }
}