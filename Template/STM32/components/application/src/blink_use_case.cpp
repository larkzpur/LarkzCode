#include "blink_use_case.hpp"
#include "config.hpp"

namespace application
{
    void BlinkUseCase::init()
    {
        m_led.init();
    }

    void BlinkUseCase::run()
    {
        for (;;)
        {
            m_led.toggle();
        }
    }

    void BlinkUseCase::alt_run()
    {
        for (;;)
        {
            m_led.set();
            delay_cycles(board_config::DELAY_1_SECOND / 10U);
            m_led.clear();

            delay_cycles(3U * board_config::DELAY_1_SECOND);
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