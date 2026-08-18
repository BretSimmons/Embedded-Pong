#include <button.h>
#include "ssd1306_fonts.h"
#include "game.h"
#include "segment.h"

// Initialize ball and paddles
volatile Ball ball;
volatile Paddle paddle1;
volatile Paddle paddle2;

// Initialize global variables
extern volatile int scored;
int ballDir = -100;
int p1score = 0;
int p2score = 0;

// Struct holds initial y position and velocity for serve
typedef struct {
    int startY;
    int startYV;
} ServeValues;

// Num of starting serves and index
#define NUM_SERVES 5
int serveIndex = 0;

// Lookup table stores values for struct
const ServeValues servePatterns[NUM_SERVES] = {
    {31, 0},    // center
    {15, 100},  // High on screen, moving down
    {45, -100}, // Low on screen, moving up
    {20, 50},   // Mid-high, moving shallow down
    {40, -50}   // Mid-low, moving shallow up
};

void Ball_Init(void){
	ServeValues val = servePatterns[serveIndex];
	// x-axis
	ball.x1 = 63;
	ball.x2 = 64;
	ball.subX = ball.x1 * 100;
	ball.xV = ballDir;
	// y-axis
	ball.y1 = val.startY;
	ball.y2 = val.startY + 1; // 2-pixel ball height
	ball.subY = ball.y1 * 100;
	ball.yV = val.startYV;

	// Increment the serve index
	serveIndex++;
	if (serveIndex >= NUM_SERVES) {
		serveIndex = 0; // Loop back to the start
	}
}

void Paddle_Init(void){
	// Initialize paddle1
	paddle1.x1 = 2;
	paddle1.x2 = 3;
	paddle1.y1 = 27;
	paddle1.y2 = 37;
	paddle1.xADD = 100;
	paddle1.yADD = 100;

	// Initialize paddle2
	paddle2.x1 = 124;
	paddle2.x2 = 125;
	paddle2.y1 = 27;
	paddle2.y2 = 37;
	paddle2.xADD = 100;
	paddle2.yADD = 100;
}

static void Draw_Point_Overlay(uint8_t player){
	// Assign the X and Y coords where the "+1" is displayed
    uint8_t x;
    if (player == 1U){ x = 13U; }
    else { x = 77U; }
    uint8_t y = 17U;

    char message[] = "+1";

    // Write the "+1" to the OLED
    ssd1306_SetCursor(x, y);
    ssd1306_WriteString(message, Font_11x18, White);
    ssd1306_UpdateScreen();
}

void Score_Update(void){
	// Pause the game physics timer
	TIM4->CR1 &= ~TIM_CR1_CEN;

    // Determine who scored and set the ball direction for the next serve
    if (scored == 1) {
        update_segment(1);
        ballDir = -100; // Player 1 scored, so ball starts by moving Left
        Draw_Point_Overlay(1U);
        delay_ms(1000U);
    }
    else if (scored == 2) {
        update_segment(2);
        ballDir = 100; // Player 2 scored, so ball starts by moving Right
        Draw_Point_Overlay(2U);
        delay_ms(1000U);
    }

    // Reset the ball
    scored = 0;
    Ball_Init();

    // Resume the game physics timer
    TIM4->CR1 |= TIM_CR1_CEN;
}
