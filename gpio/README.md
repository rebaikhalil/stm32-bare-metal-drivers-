
# STM32 Bare-Metal GPIO Driver

A lightweight, register-level GPIO driver for the **STM32F407VET6**, written in C without STM32 HAL or LL libraries. The driver provides direct access to GPIO configuration and data registers, making it suitable for learning STM32 hardware, bare-metal programming, and embedded systems development.

## Features

- Direct memory-mapped register access.
- GPIO pin identification using port and pin number.
- Pin mode configuration: input, output, alternate function, and analog.
- Internal pull-up and pull-down configuration.
- Output speed configuration.
- Push-pull and open-drain output configuration.
- Digital output control using the `BSRR` register.
- Digital input reading through the `IDR` register.
- Alternate-function selection through the `AFR` registers.
- No dependency on STM32 HAL or LL libraries.

## Hardware

- **Microcontroller:** STM32F407VET6
- **Architecture:** ARM Cortex-M4
- **Language:** C
- **Peripheral:** General-Purpose Input/Output (GPIO)

The driver uses the STM32F4 GPIO register layout and GPIO peripheral base addresses.

## Project Structure

```text
gpio/
├── inc/
│   └── gpio.h
└── src/
    └── gpio.c
```

Adapt the directory names to match your repository structure.

## Pin Encoding

The driver identifies a GPIO pin using a 16-bit value that combines the port letter and pin number.

```c
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINON(pin)     ((pin) & 255)
#define PINBANK(pin)   ((pin) >> 8)
```

For example:

```c
#define LED_PIN PIN('D', 12)
#define BUTTON_PIN PIN('A', 0)
```

`PIN('D', 12)` identifies pin 12 of GPIO port D. The encoding allows the driver to determine the GPIO peripheral and pin index from a single argument.

## GPIO Configuration

### Pin Modes

| Constant | Value | Description |
|---|---:|---|
| `GPIO_MODE_INPUT` | 0 | Digital input |
| `GPIO_MODE_OUTPUT` | 1 | General-purpose output |
| `GPIO_MODE_AF` | 2 | Alternate-function mode |
| `GPIO_MODE_ANALOG` | 3 | Analog mode |

Usage:

```c
gpio_set_mode(LED_PIN, GPIO_MODE_OUTPUT);
gpio_set_mode(BUTTON_PIN, GPIO_MODE_INPUT);
```

### Pull-Up and Pull-Down

| Constant | Value | Description |
|---|---:|---|
| `GPIO_PULL_NONE` | 0 | No pull-up or pull-down |
| `GPIO_PULL_UP` | 1 | Internal pull-up |
| `GPIO_PULL_DOWN` | 2 | Internal pull-down |

Usage:

```c
gpio_set_pull(BUTTON_PIN, GPIO_PULL_UP);
```

The pull configuration is useful for defining a stable input level when an external circuit does not otherwise drive the pin.

### Output Speed

| Constant | Value | Description |
|---|---:|---|
| `GPIO_SPEED_LOW` | 0 | Low speed |
| `GPIO_SPEED_MEDIUM` | 1 | Medium speed |
| `GPIO_SPEED_FAST` | 2 | Fast speed |
| `GPIO_SPEED_HIGH` | 3 | High speed |

Usage:

```c
gpio_set_speed(LED_PIN, GPIO_SPEED_HIGH);
```

The speed setting controls the GPIO output driver's transition characteristics. It does not directly set a software switching frequency.

### Output Type

| Constant | Value | Description |
|---|---:|---|
| `GPIO_OTYPE_PUSHPULL` | 0 | Push-pull output |
| `GPIO_OTYPE_OPENDRAIN` | 1 | Open-drain output |

Usage:

```c
gpio_set_otype(LED_PIN, GPIO_OTYPE_PUSHPULL);
```

Push-pull actively drives the output both high and low. Open-drain drives the output low or releases it, typically requiring an external pull-up or an enabled internal pull-up.

## API Reference

| Function | Description |
|---|---|
| `gpio_set_mode(pin, mode)` | Configures the pin's operating mode. |
| `gpio_set_pull(pin, pull_mode)` | Configures the input pull resistors. |
| `gpio_set_speed(pin, speed)` | Configures output speed. |
| `gpio_set_otype(pin, otype)` | Configures push-pull or open-drain operation. |
| `gpio_set_af(pin, af_num)` | Selects an alternate-function number from 0 to 15. |
| `gpio_write(pin, value)` | Sets the output high (`true`) or low (`false`). |
| `gpio_read(pin)` | Returns `true` if the corresponding `IDR` bit is set; otherwise, returns `false`. |

The configuration functions modify the corresponding GPIO registers using bit masks to update the selected pin's fields without intentionally changing other pins' configuration.

## Usage Example: LED and Button

The following example configures PD12 as an output and PA0 as an input with a pull-down resistor. The LED follows the button input.

```c
#include "gpio.h"

#define LED_PIN    PIN('D', 12)
#define BUTTON_PIN PIN('A', 0)

int main(void)
{
    gpio_set_mode(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_otype(LED_PIN, GPIO_OTYPE_PUSHPULL);
    gpio_set_speed(LED_PIN, GPIO_SPEED_LOW);
    gpio_set_pull(LED_PIN, GPIO_PULL_NONE);

    gpio_set_mode(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull(BUTTON_PIN, GPIO_PULL_DOWN);

    while (1)
    {
        bool button_state = gpio_read(BUTTON_PIN);
        gpio_write(LED_PIN, button_state);
    }
}
```

**Hardware note:** This example assumes the LED is connected to PD12 and the button to PA0, as on some STM32F4 Discovery boards. Check your actual board and circuit before using these pins.

## Alternate-Function Configuration

GPIO pins can be connected to internal peripherals such as USART, SPI, I²C, and timers by selecting the appropriate alternate-function number.

For example, the configuration sequence is:

```c
gpio_set_mode(PIN('A', 2), GPIO_MODE_AF);
gpio_set_af(PIN('A', 2), 7);
```

On the STM32F407, PA2 AF7 is commonly used for USART2_TX. Peripheral clock configuration, peripheral initialization, and pin-specific alternate-function verification must be handled separately.

## Register-Level Implementation

The driver defines a `gpio_t` structure matching the GPIO register layout:

```c
typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
} gpio_t;
```

The GPIO base address is calculated using:

```c
#define GPIO(bank) \
    ((gpio_t *)(0x40020000UL + (0x400UL * (bank))))
```

This maps ports A through I to the STM32F407 GPIO peripheral address range. The driver accesses registers directly through `volatile` pointers so that register reads and writes are performed as hardware accesses.

### Important Registers

- **MODER:** Selects the operating mode for each pin.
- **OTYPER:** Selects push-pull or open-drain output.
- **OSPEEDR:** Configures output speed.
- **PUPDR:** Configures pull-up and pull-down resistors.
- **IDR:** Reads the input state of each pin.
- **BSRR:** Sets or resets output pins through atomic bit operations.
- **AFR[0] / AFR[1]:** Selects alternate functions for pins 0–7 and 8–15, respectively.

The `BSRR` register allows output set/reset operations without a read-modify-write sequence on `ODR`.

## Build and Integration

1. Add `gpio.c` to your project.
2. Add the directory containing `gpio.h` to the compiler's include paths.
3. Include `gpio.h` in application source files.
4. Ensure the project startup code and linker script are configured for the STM32F407VET6.
5. Enable the relevant GPIO peripheral clock before accessing its registers.

For example, GPIO port D's AHB1 peripheral clock must be enabled through the RCC register before configuring PD12. RCC initialization is intentionally outside this driver's scope.

## Limitations and Future Improvements

- Add input validation for pin numbers and configuration values.
- Add explicit GPIO clock-enable integration or document the RCC prerequisite in a shared initialization layer.
- Add interrupt and EXTI configuration support.
- Add hardware validation examples for input, output, and alternate-function modes.
- Add unit tests or register-mock tests for configuration operations.


