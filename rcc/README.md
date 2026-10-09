# Bare-Metal RCC Driver for STM32F4

A lightweight, register-level **Reset and Clock Control (RCC) driver** for STM32F4 microcontrollers, developed using direct register access without STM32 HAL.

The initial target is the **STM32F407VET6 (ARM Cortex-M4)**. The driver is intended to provide a reusable foundation for bare-metal embedded systems development.

## Features

* Enable and disable internal and external clock sources.
* Select the system clock source: HSI, HSE, or PLL.
* Configure AHB, APB1, and APB2 bus prescalers.
* Enable peripheral clocks through RCC registers.
* Reset peripherals using RCC reset registers.
* Configure clock sources and prescalers without STM32 HAL.

> **Status:** Educational project under development. Validate the implementation on hardware before using it in production.

## Target and Compatibility

The initial target is the STM32F407VET6, based on the ARM Cortex-M4 core.

Although other STM32F4 microcontrollers share similar RCC concepts, their registers, peripherals, clock limits, and reset values may differ. Porting this driver to another MCU requires checking its reference manual and updating the register definitions and peripheral mappings as necessary.

## Default Clock Configuration After Reset

After a normal system reset, the STM32F407 normally starts using its internal high-speed oscillator (HSI).

| Clock configuration | Reset value / state |
| ------------------- | ------------------- |
| HSI oscillator      | Enabled             |
| HSE oscillator      | Disabled            |
| Main PLL            | Disabled            |
| System clock source | HSI                 |
| SYSCLK              | 16 MHz nominal      |
| AHB prescaler       | `/1`                |
| APB1 prescaler      | `/1`                |
| APB2 prescaler      | `/1`                |

Therefore, the nominal clock frequencies immediately after reset are:

* `SYSCLK = 16 MHz`
* `HCLK = 16 MHz`
* `PCLK1 = 16 MHz`
* `PCLK2 = 16 MHz`

These are the normal reset defaults, not necessarily the state observed when application code begins executing. A bootloader, startup code, debugger, or earlier firmware may modify the RCC registers.

Refer to the STM32F407 reference manual for the exact register reset values and conditions.

## Peripheral Clock Management

Before using a peripheral, its clock usually needs to be enabled through the corresponding RCC enable register.

Examples for the STM32F407:

| Peripheral | RCC register  | Enable bit |
| ---------- | ------------- | ---------: |
| GPIOA      | `RCC_AHB1ENR` |          0 |
| GPIOB      | `RCC_AHB1ENR` |          1 |
| GPIOC      | `RCC_AHB1ENR` |          2 |
| SPI2       | `RCC_APB1ENR` |         14 |
| USART2     | `RCC_APB1ENR` |         17 |

Example API usage, assuming these functions are implemented in the driver:

```c
rcc_enable_gpio_clock('A');
rcc_enable_uart2_clock();
rcc_enable_spi2_clock();
```

### Extending the Driver

The application developer should add peripheral clock-enable functions for any additional peripherals required by their application that are not already supported by the driver.

Examples include:

* Additional GPIO ports.
* Other USART/UART instances.
* SPI and I2C peripherals.
* General-purpose and advanced-control timers.
* ADC and DAC peripherals.
* DMA controllers.

Each function should set the correct enable bit in the appropriate RCC register for the target MCU.

Enabling a peripheral clock does **not** configure the peripheral itself. GPIO modes, alternate functions, UART baud rates, SPI settings, timer parameters, and interrupt configuration must be handled separately by the corresponding driver or application.

## Example Application

The following illustrates the intended usage pattern. Adapt the function names to the actual API declared in `rcc.h`.

```c
int main(void)
{
    /* Keep the reset-default clock or configure SYSCLK. */

    /* Enable GPIOA before accessing its registers. */
    rcc_enable_gpio_clock('A');

    /* Configure GPIOA pins here. */

    while (1)
    {
    }
}
```

If the application keeps the default HSI clock, it does not necessarily need to call a system-clock configuration function. It must still enable the clock for each peripheral it uses.

## Clock Configuration Considerations

When configuring HSE or PLL, ensure that:

* The HSE frequency matches the external crystal or clock source on the board.
* PLL parameters produce a valid input, VCO, and output frequency.
* The maximum permitted SYSCLK, HCLK, PCLK1, and PCLK2 frequencies are respected.
* Flash wait states and power/voltage settings are appropriate for the selected frequency.
* Oscillator-ready flags are checked before switching clock sources.
* Timeout handling prevents indefinite waits if a clock source fails.
* Unrelated RCC register fields are preserved when modifying configuration registers.

For the STM32F407, the commonly specified maximum frequencies are 168 MHz for SYSCLK/HCLK, 42 MHz for PCLK1, and 84 MHz for PCLK2. Always verify the permitted operating conditions in the datasheet.

## Validation Checklist

Before considering the driver stable:

* [ ] Compile with warnings enabled.
* [ ] Verify reset-default RCC register values.
* [ ] Test HSI as the system clock.
* [ ] Test HSE with a known external clock source.
* [ ] Verify PLL configuration and output frequency.
* [ ] Validate AHB and APB prescaler combinations.
* [ ] Test each implemented peripheral clock-enable function.
* [ ] Test peripheral reset functions.
* [ ] Verify timeout and error handling.
* [ ] Document the hardware board and test procedure.

## References

* STM32F407xx datasheet.
* STM32F405/407 reference manual (RM0090).
* [STMicroelectronics](https://www.st.com/)

