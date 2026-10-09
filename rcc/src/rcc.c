#include <stdint.h>
#include <stdbool.h>
#include "rcc.h"

#define FLASH_ACR              (*(volatile uint32_t *)0x40023C00U)
#define FLASH_ACR_LATENCY_5WS  0x5U
#define FLASH_ACR_PRFTEN       BIT(8)
#define FLASH_ACR_ICEN         BIT(9)
#define FLASH_ACR_DCEN         BIT(10)

static volatile uint32_t *rcc_enr(rcc_bus_t bus)
{
    switch (bus)
    {
        case RCC_BUS_AHB1: return &RCC->AHB1ENR;
        case RCC_BUS_AHB2: return &RCC->AHB2ENR;
        case RCC_BUS_AHB3: return &RCC->AHB3ENR;
        case RCC_BUS_APB1: return &RCC->APB1ENR;
        case RCC_BUS_APB2: return &RCC->APB2ENR;
        default:           return &RCC->AHB1ENR;
    }
}

static volatile uint32_t *rcc_rstr(rcc_bus_t bus)
{
    switch (bus)
    {
        case RCC_BUS_AHB1: return &RCC->AHB1RSTR;
        case RCC_BUS_AHB2: return &RCC->AHB2RSTR;
        case RCC_BUS_AHB3: return &RCC->AHB3RSTR;
        case RCC_BUS_APB1: return &RCC->APB1RSTR;
        case RCC_BUS_APB2: return &RCC->APB2RSTR;
        default:           return &RCC->AHB1RSTR;
    }
}

void rcc_clock_enable(rcc_bus_t bus, uint32_t bit)
{
    *rcc_enr(bus) |= bit;
}

void rcc_clock_disable(rcc_bus_t bus, uint32_t bit)
{
    *rcc_enr(bus) &= ~bit;
}

void rcc_peripheral_reset(rcc_bus_t bus, uint32_t bit)
{
    volatile uint32_t *rstr = rcc_rstr(bus);
    *rstr |= bit;
    *rstr &= ~bit;
}

void rcc_enable_gpio_clock(uint16_t bank)
{
    rcc_clock_enable(RCC_BUS_AHB1, BIT(PINBANK(bank)));
}

void rcc_enable_uart2_clock(void)
{
    rcc_clock_enable(RCC_BUS_APB1, RCC_APB1ENR_USART2);
}

void rcc_enable_spi2_clock(void)
{
    rcc_clock_enable(RCC_BUS_APB1, RCC_APB1ENR_SPI2);
}



static bool rcc_wait_set(volatile uint32_t *reg, uint32_t mask)
{
    uint32_t timeout = RCC_TIMEOUT;

    while ((*reg & mask) != mask)
    {
        if (--timeout == 0U)
            return false;
    }

    return true;
}

static bool rcc_wait_clear(volatile uint32_t *reg, uint32_t mask)
{
    uint32_t timeout = RCC_TIMEOUT;

    while ((*reg & mask) != 0U)
    {
        if (--timeout == 0U)
            return false;
    }

    return true;
}

static uint32_t rcc_hpre_divisor(uint32_t hpre)
{
    switch (hpre)
    {
        case RCC_CFGR_HPRE_DIV1:   return 1U;
        case RCC_CFGR_HPRE_DIV2:   return 2U;
        case RCC_CFGR_HPRE_DIV4:   return 4U;
        case RCC_CFGR_HPRE_DIV8:   return 8U;
        case RCC_CFGR_HPRE_DIV16:  return 16U;
        case RCC_CFGR_HPRE_DIV64:  return 64U;
        case RCC_CFGR_HPRE_DIV128: return 128U;
        case RCC_CFGR_HPRE_DIV256: return 256U;
        case RCC_CFGR_HPRE_DIV512: return 512U;
        default:                   return 0U;
    }
}

static uint32_t rcc_ppre_divisor(uint32_t encoding)
{
    switch (encoding & 7U)
    {
        case 0U:
        case 1U:
        case 2U:
        case 3U: return 1U;
        case 4U: return 2U;
        case 5U: return 4U;
        case 6U: return 8U;
        case 7U: return 16U;
        default: return 0U;
    }
}

bool rcc_system_clock_config(
    rcc_sysclk_source_t source,
    rcc_pll_source_t pll_source,
    uint32_t hse_hz,
    uint32_t hpre,
    uint32_t ppre1,
    uint32_t ppre2)
{
    uint32_t sysclk_hz;
    uint32_t hclk_div;
    uint32_t pclk1_div;
    uint32_t pclk2_div;
    uint32_t pllm;

    
    if (source != RCC_CLK_HSI &&
        source != RCC_CLK_HSE &&
        source != RCC_CLK_PLL)
        return false;

    if (pll_source != RCC_PLL_SRC_HSI &&
        pll_source != RCC_PLL_SRC_HSE)
        return false;

    
    hclk_div = rcc_hpre_divisor(hpre);

    if (hclk_div == 0U ||
        (hpre & ~RCC_CFGR_HPRE_Msk) != 0U ||
        (ppre1 & ~RCC_CFGR_PPRE1_Msk) != 0U ||
        (ppre2 & ~RCC_CFGR_PPRE2_Msk) != 0U)
        return false;

    pclk1_div = rcc_ppre_divisor(ppre1 >> 10);
    pclk2_div = rcc_ppre_divisor(ppre2 >> 13);

    if (pclk1_div == 0U || pclk2_div == 0U)
        return false;

    
    if (source == RCC_CLK_HSI)
    {
        sysclk_hz = 16000000U;
    }
    else if (source == RCC_CLK_HSE)
    {
        if (hse_hz < 4000000U || hse_hz > 26000000U)
            return false;

        sysclk_hz = hse_hz;
    }
    else
    {
        sysclk_hz = 168000000U;
    }

    if (source == RCC_CLK_PLL && pll_source == RCC_PLL_SRC_HSE)
    {
        if (hse_hz < 2000000U ||
            hse_hz > 26000000U ||
            (hse_hz % 1000000U) != 0U)
            return false;

        pllm = hse_hz / 1000000U;
    }
    else
    {
        
        pllm = 16U;
    }

    
    if ((sysclk_hz / hclk_div) > 168000000U ||
        ((sysclk_hz / hclk_div) / pclk1_div) > 42000000U ||
        ((sysclk_hz / hclk_div) / pclk2_div) > 84000000U)
        return false;

    
    RCC->CR |= RCC_CR_HSION;

    if (!rcc_wait_set(&RCC->CR, RCC_CR_HSIRDY))
        return false;

    
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW_Msk)
              | RCC_CFGR_SW_HSI;

    if (!rcc_wait_clear(&RCC->CFGR, RCC_CFGR_SWS_Msk))
        return false;

    
    RCC->CR &= ~RCC_CR_PLLON;

    if (!rcc_wait_clear(&RCC->CR, RCC_CR_PLLRDY))
        return false;

   
    if (source == RCC_CLK_HSE ||
        (source == RCC_CLK_PLL &&
         pll_source == RCC_PLL_SRC_HSE))
    {
        RCC->CR |= RCC_CR_HSEON;

        if (!rcc_wait_set(&RCC->CR, RCC_CR_HSERDY))
            return false;
    }

    
    if (source == RCC_CLK_PLL)
    {
        RCC->PLLCFGR = pllm
                     | (336U << 6)       
                     | (0U << 16)        
                     | (pll_source == RCC_PLL_SRC_HSE
                        ? RCC_PLLCFGR_PLLSRC_HSE
                        : RCC_PLLCFGR_PLLSRC_HSI)
                     | (7U << 24);       

        RCC->CR |= RCC_CR_PLLON;

        if (!rcc_wait_set(&RCC->CR, RCC_CR_PLLRDY))
            return false;
    }

    
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    (void)RCC->APB1ENR;

    PWR_CR = (PWR_CR & ~PWR_CR_VOS_Msk)
           | PWR_CR_VOS_SCALE1;

    
    FLASH_ACR = FLASH_ACR_LATENCY_5WS
              | FLASH_ACR_PRFTEN
              | FLASH_ACR_ICEN
              | FLASH_ACR_DCEN;

    
    RCC->CFGR = (RCC->CFGR
                 & ~(RCC_CFGR_HPRE_Msk
                   | RCC_CFGR_PPRE1_Msk
                   | RCC_CFGR_PPRE2_Msk))
              | hpre | ppre1 | ppre2;

    
    uint32_t sw;

    if (source == RCC_CLK_HSI)
        sw = RCC_CFGR_SW_HSI;
    else if (source == RCC_CLK_HSE)
        sw = RCC_CFGR_SW_HSE;
    else
        sw = RCC_CFGR_SW_PLL;

    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW_Msk) | sw;

    
    uint32_t expected_sws = sw << 2;

    if (!rcc_wait_set(&RCC->CFGR, expected_sws))
    {
        
        return false;
    }

    return (RCC->CFGR & RCC_CFGR_SWS_Msk) == expected_sws;
}
