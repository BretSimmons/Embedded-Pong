#include "segment.h"

// Define all segments of display
#define SEGMENT_REFRESH_MS 2U
#define DIGIT_1 (1U << 0) // PB0
#define DIGIT_2 (1U << 1) // PB1
#define DIGIT_3 (1U << 2) // PB2
#define DIGIT_4 (1U << 3) // PB3
#define ALL_DIGITS (DIGIT_1 | DIGIT_2 | DIGIT_3 | DIGIT_4)
#define ALL_SEGMENTS (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)

static const uint16_t segment_map[10] = {
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,         // 0
    SEG_B | SEG_C,                                         // 1
    SEG_A | SEG_B | SEG_D | SEG_E | SEG_G,                 // 2
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_G,                 // 3
    SEG_B | SEG_C | SEG_F | SEG_G,                         // 4
    SEG_A | SEG_C | SEG_D | SEG_F | SEG_G,                 // 5
    SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,         // 6
    SEG_A | SEG_B | SEG_C,                                 // 7
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G, // 8
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G          // 9
};

static volatile uint8_t score_digits[4] = {0U, 0U, 0U, 0U};
volatile uint8_t p1_score = 0U;
volatile uint8_t p2_score = 0U;
static uint8_t active_digit = 0U;
static uint8_t refresh_ms = 0U;

void segment_init(void) {
    // Enable clocks for segment and digit-select GPIO ports
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN;
    (void)RCC->AHB2ENR;

    // Configure segment pins PA0, PA1, PA4, PA6, PA7, PA8, PA9 as outputs
    GPIOA->MODER &= ~((3U << (0U * 2U)) | (3U << (1U * 2U)) |
                      (3U << (4U * 2U)) | (3U << (6U * 2U)) |
                      (3U << (7U * 2U)) | (3U << (8U * 2U)) |
                      (3U << (9U * 2U)));
    GPIOA->MODER |=  ((1U << (0U * 2U)) | (1U << (1U * 2U)) |
                      (1U << (4U * 2U)) | (1U << (6U * 2U)) |
                      (1U << (7U * 2U)) | (1U << (8U * 2U)) |
                      (1U << (9U * 2U)));

    // Use push-pull outputs and no pulls on segment pins
    GPIOA->OTYPER &= ~ALL_SEGMENTS;
    GPIOA->PUPDR &= ~((3U << (0U * 2U)) | (3U << (1U * 2U)) |
                      (3U << (4U * 2U)) | (3U << (6U * 2U)) |
                      (3U << (7U * 2U)) | (3U << (8U * 2U)) |
                      (3U << (9U * 2U)));

    // Configure digit-select pins PB0-PB3 as outputs
    GPIOB->MODER &= ~((3U << (0U * 2U)) | (3U << (1U * 2U)) |
                      (3U << (2U * 2U)) | (3U << (3U * 2U)));
    GPIOB->MODER |=  ((1U << (0U * 2U)) | (1U << (1U * 2U)) |
                      (1U << (2U * 2U)) | (1U << (3U * 2U)));

    // Use push-pull outputs and no pulls on digit-select pins
    GPIOB->OTYPER &= ~ALL_DIGITS;
    GPIOB->PUPDR &= ~((3U << (0U * 2U)) | (3U << (1U * 2U)) |
                      (3U << (2U * 2U)) | (3U << (3U * 2U)));

    // Turn everything off before enabling periodic refresh
    all_digits_off();
    GPIOA->ODR &= ~ALL_SEGMENTS;
    update_segment(0);
}

	void GPIO_init(void) {
		segment_init();
}

void all_digits_off(void) {
    // Digit-select pins are active-low, so logic high turns them off
    GPIOB->ODR |= ALL_DIGITS;
}

void display_digit(uint8_t digit, uint8_t position) {
    // Guard against invalid display requests
    if (digit > 9U || position > 3U) {
        all_digits_off();
        return;
    }

    // Disable all digits before changing segment lines to avoid ghosting
    all_digits_off();

    // Output the segment pattern and enable the requested digit
    GPIOA->ODR = (GPIOA->ODR & ~ALL_SEGMENTS) | segment_map[digit];
    GPIOB->ODR &= ~(1U << position);
}

void segment_refresh_tick(void) {
    // Slow the 1 ms SysTick down to the desired multiplex interval
    refresh_ms++;
    if (refresh_ms < SEGMENT_REFRESH_MS) {
        return;
    }

    // Refresh one digit per call so all digits appear continuously lit
    refresh_ms = 0U;
    display_digit(score_digits[active_digit], active_digit);
    active_digit = (uint8_t)((active_digit + 1U) % 4U);
}

void update_segment(int player) {
    // Increment the requested player, or reset both scores for any other value
    if (player == 1) {
        p1_score++;
    } else if (player == 2) {
        p2_score++;
    } else {
        p1_score = 0U;
        p2_score = 0U;
    }

    uint32_t current_score = ((p1_score % 100U) * 100U) + (p2_score % 100U);

	uint32_t irq_state = __get_PRIMASK();
	__disable_irq();
	score_digits[0] = (current_score / 1000U) % 10U; // P1 tens
    score_digits[1] = (current_score / 100U)  % 10U; // P1 ones
    score_digits[2] = (current_score / 10U)   % 10U; // P2 tens
    score_digits[3] =  current_score          % 10U; // P2 ones
    if (irq_state == 0U) {
        __enable_irq();
    }
}
