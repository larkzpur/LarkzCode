#include "platform/system.hpp"
#include <stdint.h>

#define PERIPH_BASE           ((uint32_t)0x40000000UL)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000UL)
#define APB1PERIPH_BASE       (PERIPH_BASE + 0x00000000UL)

#define RCC_BASE             (AHB1PERIPH_BASE + 0x00003800UL)
#define FLASH_R_BASE           (AHB1PERIPH_BASE + 0x00003C00UL)

#define PWR_BASE             (APB1PERIPH_BASE + 0x00007000UL)

#define RCC_CR (*(volatile uint32_t *)(RCC_BASE + 0x00UL))
#define RCC_PLLCFGR (*(volatile uint32_t *)(RCC_BASE + 0x04UL))
#define RCC_CFGR (*(volatile uint32_t *)(RCC_BASE + 0x08UL))
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x40UL))

#define PWR_CR (*(volatile uint32_t *)(PWR_BASE + 0x00UL))
#define FLASH_ACR (*(volatile uint32_t *)(FLASH_R_BASE + 0x00UL))

#define RCC_CR_HSION (1U << 0)
#define RCC_CR_HSIRDY (1U << 1)
#define RCC_CR_PLLON (1U << 24)
#define RCC_CR_PLLRDY (1U << 25)

#define RCC_CFGR_SW_MASK (3UL << 0)
#define RCC_CFGR_SW_HSI (0UL << 0)
#define RCC_CFGR_SW_PLL (2UL << 0)
#define RCC_CFGR_SWS_MASK (3UL << 2)
#define RCC_CFGR_SWS_HSI (0UL << 2)
#define RCC_CFGR_SWS_PLL (2UL << 2)
#define RCC_CFGR_PPRE1_2 (4UL << 10)

#define RCC_APB1ENR_PWREN (1UL << 28)

#define PWR_CR_VOS_MASK (3UL << 14)
#define PWR_CR_VOS_SCALE2 (2UL << 14)

#define FLASH_ACR_LATENCY_2WS (2UL << 0)
#define FLASH_ACR_LATERNCY_1WS (1UL << 0)
#define FLASH_ACR_PRFTEN (1UL << 8)
#define FLASH_ACR_ICEN (1UL << 9)
#define FLASH_ACR_DCEN (1UL << 10)

#if !defined(USE_SYSCLK_84MHZ) && !defined(USE_SYSCLK_42MHZ)
static void set_sysclk_16mhz_hsi(void)
{
    // Enable HSI
    RCC_CR |= RCC_CR_HSION;
    while (!(RCC_CR & RCC_CR_HSIRDY)==0U)
        ;

    // Set HSI as system clock
    RCC_CFGR &= ~RCC_CFGR_SW_MASK;
    RCC_CFGR |= RCC_CFGR_SW_HSI;
    while ((RCC_CFGR & RCC_CFGR_SWS_MASK) != RCC_CFGR_SWS_HSI)
        ;
    
    RCC_CR &= ~RCC_CR_PLLON; // Disable PLL
    while ((RCC_CR & RCC_CR_PLLRDY) != 0U)
        ;   
}
#endif

#ifdef USE_SYSCLK_84MHZ
static void set_sysclk_84mhz_pll_from_hsi(void)
{
    uint32_t pllcfgr;

    RCC_CR |= RCC_CR_HSION; // Enable HSI
    while (!(RCC_CR & RCC_CR_HSIRDY)==0U)
        ;
    
    RCC_APB1ENR |= RCC_APB1ENR_PWREN; // Enable PWR clock
    PWR_CR &= ~PWR_CR_VOS_MASK; // Clear VOS bits
    PWR_CR |= PWR_CR_VOS_SCALE2; // Set VOS to Scale 2 mode

    FLASH_ACR |= FLASH_ACR_LATENCY_2WS | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN; // Set flash latency and enable caches

    RCC_CR &= ~RCC_CR_PLLON; // Disable PLL
    while ((RCC_CR & RCC_CR_PLLRDY) != 0U)
        ;
    
    /* HSI=16MHz, PLLM=16, PLLN=336, PLLP=2, PLLQ=7 */
    //pllcfgr = (16U << 0) | (336U << 6) | (0U << 16) | (2U << 24) | (7U << 24);
    pllcfgr = (16U << 0);
    pllcfgr |= (336U << 6);
    pllcfgr |= (0U << 16);
    pllcfgr |= (0U << 16); // PLLP=2 (00)
    pllcfgr |= (7U << 24); // PLLQ=7
    RCC_PLLCFGR = pllcfgr;

    RCC_CFGR &= ~((7UL << 10) | (7UL << 13)); // Clear PPRE1 and PPRE2
    RCC_CFGR |= RCC_CFGR_PPRE1_2; // Set PPRE1 to divide by 2 (APB1 clock = 42MHz)

    RCC_CR |= RCC_CR_PLLON; // Enable PLL
    while (!(RCC_CR & RCC_CR_PLLRDY)==0U)
        ;
    
    RCC_CFGR &= ~RCC_CFGR_SW_MASK; // Clear SW bits
    RCC_CFGR |= RCC_CFGR_SW_PLL; // Set PLL as system clock
    while ((RCC_CFGR & RCC_CFGR_SWS_MASK) != RCC_CFGR_SWS_PLL)
        ;
}
#endif

#ifdef USE_SYSCLK_42MHZ
static void set_sysclk_42mhz_pll_from_hsi(void)
{
    uint32_t pllcfgr;

    RCC_CR |= RCC_CR_HSION; // Enable HSI
    while (!(RCC_CR & RCC_CR_HSIRDY)==0U)
        ;
    
    RCC_APB1ENR |= RCC_APB1ENR_PWREN; // Enable PWR clock
    PWR_CR &= ~PWR_CR_VOS_MASK; // Clear VOS bits
    PWR_CR |= PWR_CR_VOS_SCALE2; // Set VOS to Scale 2 mode

    FLASH_ACR |= FLASH_ACR_LATERNCY_1WS | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN; // Set flash latency and enable caches

    RCC_CR &= ~RCC_CR_PLLON; // Disable PLL
    while ((RCC_CR & RCC_CR_PLLRDY) != 0U)
        ;
    
    /* HSI=16MHz, PLLM=16, PLLN=336, PLLP=4, PLLQ=7 */
    pllcfgr = (16U << 0);
    pllcfgr |= (336U << 6);
    pllcfgr |= (0U << 16); // PLLP=4 (01)
    pllcfgr |= (7U << 24); // PLLQ=7
    RCC_PLLCFGR = pllcfgr;

    RCC_CFGR &= ~((7UL << 10) | (7UL << 13)); // Clear PPRE1 and PPRE2
    RCC_CFGR |= RCC_CFGR_PPRE1_2; // Set PPRE1 to divide by 2 (APB1 clock = 21MHz)

    RCC_CR |= RCC_CR_PLLON; // Enable PLL
    while (!(RCC_CR & RCC_CR_PLLRDY)==0U)
        ;
    
    RCC_CFGR &= ~RCC_CFGR_SW_MASK; // Clear SW bits
    RCC_CFGR |= RCC_CFGR_SW_PLL; // Set PLL as system clock
    while ((RCC_CFGR & RCC_CFGR_SWS_MASK) != RCC_CFGR_SWS_PLL)
        ;
}
#endif

extern "C" void SystemInit(void)
{
#ifdef USE_SYSCLK_84MHZ
    set_sysclk_84mhz_pll_from_hsi();
#elif defined(USE_SYSCLK_42MHZ)
    set_sysclk_42mhz_pll_from_hsi();
#else
    set_sysclk_16mhz_hsi();
#endif
}