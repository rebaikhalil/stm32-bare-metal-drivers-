#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"


void gpio_set_mode(uint16_t pin,uint8_t mode){
    gpio_t *gpio=GPIO(PINBANK(pin));
    int n =PINON(pin);
    gpio->MODER &= ~(3UL << 2*n);
    gpio->MODER |=  ((uint32_t)mode <<2 *n);
};
void gpio_set_pull(uint16_t pin,uint8_t pull_mode){
    gpio_t *gpio=GPIO(PINBANK(pin));
    int n =PINON(pin);
    gpio->PUPDR &=~(3UL << 2*n);
    gpio->PUPDR |= ((uint32_t)pull_mode << 2*n);
};

void gpio_set_speed(uint16_t pin, uint8_t speed){
    gpio_t *gpio=GPIO(PINBANK(pin));
    int n=PINON(pin);
    gpio->OSPEEDR &= ~(3UL << 2*n);
    gpio->OSPEEDR |= ((uint32_t)speed << 2*n);
};

void gpio_set_otype(uint16_t pin, uint8_t otype){
    gpio_t *gpio=GPIO(PINBANK(pin));
    int n=PINON(pin);
    gpio->OTYPER &= ~(1UL << n);
    gpio->OTYPER |= ((uint32_t)otype << n);
};

void gpio_write(uint16_t pin,bool value){
    gpio_t *gpio=GPIO(PINBANK(pin));
    int n=PINON(pin);
    gpio->BSRR |=(1UL << (n + (value ? 0: 16)));
};

bool gpio_read(uint16_t pin){
    gpio_t *gpio=GPIO(PINBANK(pin));
    int n=PINON(pin);
    return ((gpio->IDR&(1UL<<n)) ? true : false);
};

void gpio_set_af(uint16_t pin,uint8_t af_num){
    gpio_t *gpio=GPIO(PINBANK(pin));
    int n=PINON(pin);
    gpio->AFR[n >> 3] &=~(15UL<<((n & 7)*4));
    gpio->AFR[n >> 3] |= ((uint32_t)af_num<<((n & 7)*4));
};
