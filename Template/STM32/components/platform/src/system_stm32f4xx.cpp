#include "system.hpp"
#include <stdint.h>

#define PERIPH_BASE             ((uint32_t)0x40000000UL)

#define AHB1PERIPH_BASE         (PERIPH_BASE + 0x00020000UL)
#define APB1PERIPH_BASE         (PERIPH_BASE + 0x00000000UL)

#define RCC_BASE                (AHB1PERIPH_BASE + 0x00003800UL)
#define FLASH_R_BASE            (AHB1PERIPH_BASE + 0x00003C00UL)
#define PWR_BASE                (APB1PERIPH_BASE + 0x00007000UL)

/* RCC registers */
#define RCC_CR                  (*(volatile uint32_t *)(RCC_BASE + 0x00UL))
#define RCC_PLLCFGR             (*(volatile uint32_t *)(RCC_BASE + 0x04UL))
#define RCC_CFGR                (*(volatile uint32_t *)(RCC_BASE + 0x08UL))
#define RCC_AHB1ENR             (*(volatile uint32_t *)(RCC_BASE + 0x30UL))
#define RCC_APB1ENR             (*(volatile uint32_t *)(RCC_BASE + 0x40UL))

/* PWR registers */
#define PWR_CR                  (*(volatile uint32_t *)(PWR_BASE + 0x00UL))

/* FLASH registers */
#define FLASH_ACR               (*(volatile uint32_t *)(FLASH_R_BASE + 0x00UL))

/* RCC_CR bits */
#define RCC_CR_HSION            (1UL << 0)
#define RCC_CR_HSIRDY           (1UL << 1)
#define RCC_CR_PLLON            (1UL << 24)
#define RCC_CR_PLLRDY           (1UL << 25)

/* RCC_CFGR bits */
#define RCC_CFGR_SW_MASK        (3UL << 0)
#define RCC_CFGR_SW_HSI         (0UL << 0)
#define RCC_CFGR_SW_PLL         (2UL << 0)

#define RCC_CFGR_SWS_MASK       (3UL << 2)
#define RCC_CFGR_SWS_HSI        (0UL << 2)
#define RCC_CFGR_SWS_PLL        (2UL << 2)

#define RCC_CFGR_PPRE1_2        (4UL << 10)

/* RCC_APB1ENR bits */
#define RCC_APB1ENR_PWREN       (1UL << 28)

/* PWR_CR bits */
#define PWR_CR_VOS_MASK         (3UL << 14)
#define PWR_CR_VOS_SCALE2       (2UL << 14)

/* FLASH_ACR bits */
#define FLASH_ACR_LATENCY_MASK  (7UL << 0)
#define FLASH_ACR_LATENCY_2WS   (2UL << 0)
#define FLASH_ACR_PRFTEN        (1UL << 8)
#define FLASH_ACR_ICEN          (1UL << 9)
#define FLASH_ACR_DCEN          (1UL << 10)


static void set_sysclk_84mhz_pll_from_hsi(void)
{
    uint32_t pllcfgr;

    /*
     * HSI
     * ---
     * STM32F411 HSI = 16 MHz
     *
     * Wait until HSI is ready.
     */
    while ((RCC_CR & RCC_CR_HSIRDY) == 0U)
    {
        ;
    }

    /*
     * Enable PWR peripheral clock.
     */
    RCC_APB1ENR |= RCC_APB1ENR_PWREN;

    /*
     * Configure voltage scaling.
     */
    PWR_CR &= ~PWR_CR_VOS_MASK;
    PWR_CR |= PWR_CR_VOS_SCALE2;

    /*
     * Configure Flash:
     *
     * 84 MHz requires 2 wait states.
     */
    FLASH_ACR &= ~FLASH_ACR_LATENCY_MASK;

    FLASH_ACR |=
        FLASH_ACR_LATENCY_2WS |
        FLASH_ACR_PRFTEN |
        FLASH_ACR_ICEN |
        FLASH_ACR_DCEN;

    /*
     * Disable PLL before reconfiguration.
     */
    RCC_CR &= ~RCC_CR_PLLON;

    /*
     * Wait until PLL is completely stopped.
     */
    while ((RCC_CR & RCC_CR_PLLRDY) != 0U)
    {
        ;
    }

    /*
     * PLL configuration
     *
     * HSI  = 16 MHz
     *
     * PLLM = 16
     * PLLN = 336
     * PLLP = 2
     * PLLQ = 7
     *
     * VCO input:
     *
     *     16 MHz / 16 = 1 MHz
     *
     * VCO output:
     *
     *     1 MHz * 336 = 336 MHz
     *
     * SYSCLK:
     *
     *     336 MHz / 2 = 168 MHz
     *
     * NOTE:
     * The above calculation gives 168 MHz, not 84 MHz.
     *
     * Therefore PLLN must be 168 for 84 MHz:
     *
     *     (16 / 16) * 168 / 2 = 84 MHz
     */

    pllcfgr = 0U;

    /* PLLM = 16 */
    pllcfgr |= (16U << 0);

    /* PLLN = 168 */
    pllcfgr |= (168U << 6);

    /* PLLP = 2 (00b) */
    pllcfgr |= (0U << 16);

    /* PLLQ = 7 */
    pllcfgr |= (7U << 24);

    /*
     * Select HSI as PLL source.
     *
     * PLLSRC = 0 -> HSI
     */
    RCC_PLLCFGR = pllcfgr;

    /*
     * Configure APB1 prescaler.
     *
     * PPRE1 = 100b -> divide by 2
     *
     * 84 MHz / 2 = 42 MHz APB1
     */
    RCC_CFGR &= ~(7UL << 10);
    RCC_CFGR |= RCC_CFGR_PPRE1_2;

    /*
     * Enable PLL.
     */
    RCC_CR |= RCC_CR_PLLON;

    /*
     * Wait until PLL is ready.
     */
    while ((RCC_CR & RCC_CR_PLLRDY) == 0U)
    {
        ;
    }

    /*
     * Select PLL as system clock.
     */
    RCC_CFGR &= ~RCC_CFGR_SW_MASK;
    RCC_CFGR |= RCC_CFGR_SW_PLL;

    /*
     * Wait until PLL is actually selected as SYSCLK.
     */
    while ((RCC_CFGR & RCC_CFGR_SWS_MASK) != RCC_CFGR_SWS_PLL)
    {
        ;
    }
}


extern "C" void SystemInit(void)
{
    set_sysclk_84mhz_pll_from_hsi();
}