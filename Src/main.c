#include <button.h>
#include "ssd1306_fonts.h"
#include "TIM.h"
#include "game.h"
#include "segment.h"
#include "ADC.h"


#define SYSTEM_CLOCK_HZ 16000000U

volatile uint32_t g_ms_ticks;

// Variable for ADC speed increase (100 = 1x speed)
volatile int modADC = 100;

// Create global ball and paddles
extern volatile Ball ball;
extern volatile Paddle paddle1;
extern volatile Paddle paddle2;

// Global variables for player scores
extern volatile uint8_t p1_score;
extern volatile uint8_t p2_score;

extern volatile int updateFlag;
extern volatile int scored;

void SysTick_Handler(void) {
    // Increment the millisecond counter used by delays and button debounce
    g_ms_ticks++;

    segment_refresh_tick();
}

// Basic millisecond delay
void delay_ms(uint32_t ms) {
    uint32_t start = g_ms_ticks;

    while ((g_ms_ticks - start) < ms) {
        __WFI();
    }
}

static void draw_game_board(void) {
	updateFlag = 0;

	// Clears OLED and draws border for game
    ssd1306_Fill(Black);
    ssd1306_DrawRectangle(0, 0, SSD1306_WIDTH - 1U, SSD1306_HEIGHT - 1U, White);

    // Draw middle rectangle line
    for (int i=0; i<SSD1306_HEIGHT - 1U; i+=2) {
    	ssd1306_DrawRectangle(64, i, 64, i, White);
    }

    // Draw ball
    ssd1306_FillRectangle(ball.x1, ball.y1, ball.x2, ball.y2, White);

    // Draw paddle1
    ssd1306_FillRectangle(paddle1.x1, paddle1.y1, paddle1.x2, paddle1.y2, White);

    // Draw paddle2
    ssd1306_FillRectangle(paddle2.x1, paddle2.y1, paddle2.x2, paddle2.y2, White);

    ssd1306_UpdateScreen();
}

void reset_game(void) {
    // Reset scores to 0 on the 7 seg display
    update_segment(0);

    // Reset positions of the ball and paddles
    Ball_Init();
    Paddle_Init();

    // Reset scored
    scored = 0;
}

int main(void) {
    // Initialize score display first so it can refresh from SysTick
    segment_init();
    update_segment(0);

    // Configure 1 ms SysTick timing
	SysTick_Config(SYSTEM_CLOCK_HZ / 1000U);

    // Configure button inputs for polling; EXTI can be added later here
    Button_Init();

    // Initialize game state before the first OLED draw
    Ball_Init();
    Paddle_Init();

    // Initialize the ADC
    ADC_Init();

    // Initialize OLED and draw the starting board if the display responds
    ssd1306_Init();
    if (ssd1306_GetLastError() != SSD1306_OK) {
        while (1) {
            delay_ms(100U);
        }
    }

    // Start periodic game updates
    TIM4_Init();

    while (1) {
    	// Read the ADC value
    	int numADC = 4095 - Read_ADC();
    	// 4095 / 20 is ~200 so the mod is ~100 to 304
    	modADC = 100 + (numADC / 20);

    	// Check if the component positions have been altered
    	if (updateFlag){
    		// Redraw the game board
      		draw_game_board();
      		// Check if a player scored
      		if (scored){
      			Score_Update();
      			// Check if a player won
      			// If so, print winning message
      			if (p1_score == 11U) {
      				ssd1306_Fill(Black);
      				ssd1306_SetCursor(20, 9);
      				ssd1306_WriteString("PLAYER 1", Font_11x18, White);
      				ssd1306_SetCursor(25, 36);
      				ssd1306_WriteString("WINS!!!", Font_11x18, White);
      				ssd1306_UpdateScreen();

      				delay_ms(4000U);
      				reset_game();
      		    }
      			if (p2_score == 11U) {
      				ssd1306_Fill(Black);
      				ssd1306_SetCursor(20, 9);
      				ssd1306_WriteString("PLAYER 2", Font_11x18, White);
      				ssd1306_SetCursor(25, 36);
      				ssd1306_WriteString("WINS!!!", Font_11x18, White);
      				ssd1306_UpdateScreen();

      				delay_ms(4000U);
      				reset_game();
      			}
      		}
        }
    }
}
