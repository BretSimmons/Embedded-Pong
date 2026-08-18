#ifndef SEGMENT_H
#define SEGMENT_H

#include "stm32l476xx.h"
#include <stdint.h>

// Segment pins on GPIOA
#define SEG_A (1U << 0) // PA0
#define SEG_B (1U << 1) // PA1
#define SEG_C (1U << 4) // PA4
#define SEG_D (1U << 6) // PA6
#define SEG_E (1U << 7) // PA7
#define SEG_F (1U << 8) // PA8
#define SEG_G (1U << 9) // PA9

// Configure segment GPIO and reset the score display
void segment_init(void);

// Refresh one multiplexed digit; called from SysTick_Handler
void segment_refresh_tick(void);

// player 1 increments P1, player 2 increments P2, any other value resets
void update_segment(int player);

void GPIO_init(void);
void all_digits_off(void);
void display_digit(uint8_t digit, uint8_t position);

#endif
