#ifndef GPIO_H
#define  GPIO_H


#include <stdint.h>
#include <stdbool.h>

#define BIT(x)  (1UL << x)
#define PIN(bank,num) ((((bank)- 'A')<<8) | (num))
#define PINON(pin)  ((pin) & 255)
#define PINBANK(pin) ((pin)>>8)


typedef struct {
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
} gpio_t;

#define GPIO(bank) ((gpio_t*)(0x40020000UL + (0x400UL * (bank))))




enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };
enum { GPIO_PULL_NONE , GPIO_PULL_UP , GPIO_PULL_DOWN };
enum { GPIO_SPEED_LOW, GPIO_SPEED_MEDIUM, GPIO_SPEED_FAST, GPIO_SPEED_HIGH };
enum { GPIO_OTYPE_PUSHPULL, GPIO_OTYPE_OPENDRAIN };



void gpio_set_mode(uint16_t pin,uint8_t mode);
void gpio_set_pull(uint16_t pin,uint8_t pull_mode);
void gpio_set_speed(uint16_t pin, uint8_t speed);
void gpio_set_otype(uint16_t pin, uint8_t otype);
void gpio_set_af(uint16_t pin,uint8_t af_num);
void gpio_write(uint16_t pin,bool value);
bool gpio_read(uint16_t pin); 


#endif
