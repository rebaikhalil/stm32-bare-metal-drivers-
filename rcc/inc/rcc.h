#ifndef RCC_H
#define RCC_H

#include <stdint.h>
#include <stdbool.h>

#define BIT(x)  (1UL << x)
#define PIN(bank,num) ((((bank)- 'A')<<8) | (num))
#define PINON(pin)  ((pin) & 255)
#define PINBANK(pin) ((pin)>>8)

typedef struct {
    volatile uint32_t CR, PLLCFGR, CFGR, CIR, AHB1RSTR, AHB2RSTR, AHB3RSTR;
    volatile uint32_t RESERVED_1;
    volatile uint32_t APB1RSTR, APB2RSTR;
    volatile uint32_t RESERVED_2[2];
    volatile uint32_t AHB1ENR, AHB2ENR, AHB3ENR;
    volatile uint32_t RESERVED_3;
    volatile uint32_t APB1ENR, APB2ENR;
} rcc_t;
#define RCC ((rcc_t*)0x40023800)


#define RCC_CR_HSION            BIT(0)
#define RCC_CR_HSIRDY            BIT(1)
#define RCC_CR_HSEON             BIT(16)
#define RCC_CR_HSERDY            BIT(17)
#define RCC_CR_PLLON             BIT(24)
#define RCC_CR_PLLRDY            BIT(25)



#define RCC_CFGR_SW_Msk          (3UL << 0)
#define RCC_CFGR_SW_HSI          (0UL << 0)
#define RCC_CFGR_SW_HSE          (1UL << 0)
#define RCC_CFGR_SW_PLL          (2UL << 0)
#define RCC_CFGR_SWS_Msk         (3UL << 2)
#define RCC_CFGR_HPRE_DIV1       (0UL << 4)
#define RCC_CFGR_PPRE1_DIV4      (5UL << 10)
#define RCC_CFGR_PPRE2_DIV2      (4UL << 13)



#define RCC_PLLCFGR_PLLSRC_HSE   BIT(22)
#define RCC_PLLCFGR_PLLSRC_HSI   0U


#define RCC_AHB1ENR_GPIOA        BIT(0)
#define RCC_AHB1ENR_GPIOB        BIT(1)
#define RCC_AHB1ENR_GPIOC        BIT(2)
#define RCC_AHB1ENR_GPIOD        BIT(3)
#define RCC_AHB1ENR_GPIOE        BIT(4)
#define RCC_AHB1ENR_GPIOF        BIT(5)
#define RCC_AHB1ENR_GPIOG        BIT(6)
#define RCC_AHB1ENR_GPIOH        BIT(7)
#define RCC_AHB1ENR_GPIOI        BIT(8)
#define RCC_AHB1ENR_DMA1         BIT(21)
#define RCC_AHB1ENR_DMA2         BIT(22)


#define RCC_APB1ENR_TIM2         BIT(0)
#define RCC_APB1ENR_TIM3         BIT(1)
#define RCC_APB1ENR_TIM4         BIT(2)
#define RCC_APB1ENR_TIM5         BIT(3)
#define RCC_APB1ENR_SPI2         BIT(14)
#define RCC_APB1ENR_SPI3         BIT(15)
#define RCC_APB1ENR_USART2       BIT(17)
#define RCC_APB1ENR_USART3       BIT(18)
#define RCC_APB1ENR_I2C1         BIT(21)
#define RCC_APB1ENR_I2C2         BIT(22)
#define RCC_APB1ENR_I2C3         BIT(23)



#define RCC_APB2ENR_TIM1         BIT(0)
#define RCC_APB2ENR_USART1       BIT(4)
#define RCC_APB2ENR_USART6       BIT(5)
#define RCC_APB2ENR_ADC1         BIT(8)
#define RCC_APB2ENR_SPI1         BIT(12)
#define RCC_APB2ENR_SYSCFG       BIT(14)




#define RCC_CFGR_HPRE_Msk       (0xFUL << 4)

#define RCC_CFGR_HPRE_DIV1      (0x0UL << 4)
#define RCC_CFGR_HPRE_DIV2      (0x8UL << 4)
#define RCC_CFGR_HPRE_DIV4      (0x9UL << 4)
#define RCC_CFGR_HPRE_DIV8      (0xAUL << 4)
#define RCC_CFGR_HPRE_DIV16     (0xBUL << 4)
#define RCC_CFGR_HPRE_DIV64     (0xCUL << 4)
#define RCC_CFGR_HPRE_DIV128    (0xDUL << 4)
#define RCC_CFGR_HPRE_DIV256    (0xEUL << 4)
#define RCC_CFGR_HPRE_DIV512    (0xFUL << 4)


#define RCC_CFGR_PPRE_DIV1      (0x0UL)
#define RCC_CFGR_PPRE_DIV2      (0x4UL)
#define RCC_CFGR_PPRE_DIV4      (0x5UL)
#define RCC_CFGR_PPRE_DIV8      (0x6UL)
#define RCC_CFGR_PPRE_DIV16     (0x7UL)

#define RCC_CFGR_PPRE1_Msk      (7UL << 10)
#define RCC_CFGR_PPRE2_Msk      (7UL << 13)

#define RCC_CFGR_PPRE1_DIV1     (RCC_CFGR_PPRE_DIV1 << 10)
#define RCC_CFGR_PPRE1_DIV2     (RCC_CFGR_PPRE_DIV2 << 10)
#define RCC_CFGR_PPRE1_DIV4     (RCC_CFGR_PPRE_DIV4 << 10)
#define RCC_CFGR_PPRE1_DIV8     (RCC_CFGR_PPRE_DIV8 << 10)
#define RCC_CFGR_PPRE1_DIV16    (RCC_CFGR_PPRE_DIV16 << 10)

#define RCC_CFGR_PPRE2_DIV1     (RCC_CFGR_PPRE_DIV1 << 13)
#define RCC_CFGR_PPRE2_DIV2     (RCC_CFGR_PPRE_DIV2 << 13)
#define RCC_CFGR_PPRE2_DIV4     (RCC_CFGR_PPRE_DIV4 << 13)
#define RCC_CFGR_PPRE2_DIV8     (RCC_CFGR_PPRE_DIV8 << 13)
#define RCC_CFGR_PPRE2_DIV16    (RCC_CFGR_PPRE_DIV16 << 13)

#define RCC_CFGR_HPRE_Msk   (0xFUL << 4)
#define RCC_CFGR_PPRE1_Msk  (7UL << 10)
#define RCC_CFGR_PPRE2_Msk  (7UL << 13)

#define RCC_APB1ENR_PWREN   BIT(28)

#define PWR_CR              (*(volatile uint32_t *)0x40007000U)
#define PWR_CR_VOS_Msk      (3UL << 14)
#define PWR_CR_VOS_SCALE1   (3UL << 14)

#define RCC_TIMEOUT         100000U

typedef enum {
    RCC_BUS_AHB1 = 0,
    RCC_BUS_AHB2,
    RCC_BUS_AHB3,
    RCC_BUS_APB1,
    RCC_BUS_APB2
} rcc_bus_t;

typedef enum {
    RCC_CLK_HSI = 0,
    RCC_CLK_HSE,
    RCC_CLK_PLL
} rcc_sysclk_source_t;

typedef enum {
    RCC_PLL_SRC_HSI = 0,
    RCC_PLL_SRC_HSE
} rcc_pll_source_t;




void rcc_clock_enable(rcc_bus_t bus, uint32_t bit);
void rcc_clock_disable(rcc_bus_t bus, uint32_t bit);


void rcc_peripheral_reset(rcc_bus_t bus, uint32_t bit);


void rcc_enable_gpio_clock(uint16_t bank);
void rcc_enable_uart2_clock(void);
void rcc_enable_spi2_clock(void);



bool rcc_system_clock_config(
    rcc_sysclk_source_t source,
    rcc_pll_source_t pll_source,
    uint32_t hse_hz,
    uint32_t hpre,
    uint32_t ppre1,
    uint32_t ppre2
);



#endif
