#include "stm32l476xx.h"

void Button_Init(void) {
	// Enable clock for GPIOC
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

	// Configure PC0-PC3 as input with pull-up resistor
	GPIOC->MODER &= ~((3U<<0) | (3U<<2) | (3U<<4) | (3U<<6));
	GPIOC->PUPDR &= ~((3U<<0) | (3U<<2) | (3U<<4) | (3U<<6));
	GPIOC->PUPDR |= ((1U<<0) | (1U<<2) | (1U<<4) | (1U<<6));
}
