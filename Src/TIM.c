#include "game.h"
#include "segment.h"

// Define variables
extern volatile Ball ball;
extern volatile Paddle paddle1;
extern volatile Paddle paddle2;

extern volatile int modADC;

int updateFlag = 0;
volatile int scored = 0;

void TIM4_Init(void){
	// Enable the TIM4 clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
	// Select the pre-scalar value to be 999
	TIM4->PSC = 999;
	// Set the auto reload value to 179
	TIM4->ARR = 319;
	// Enable the Update Interrupt Enable (UIE) bit
	TIM4->DIER |= TIM_DIER_UIE;
	// Enable trigger generation.
	TIM4->EGR = TIM_EGR_UG;
	// Set priority and enable the interrupt
	NVIC_SetPriority (TIM4_IRQn, 1);
	NVIC_EnableIRQ(TIM4_IRQn);
	// Enable the timer
	TIM4->CR1 |= TIM_CR1_CEN;
}

void TIM4_IRQHandler(void) {
    // Check if the Update Interrupt Flag is set
    if (TIM4->SR & TIM_SR_UIF) {
        // Clear the interrupt flag immediately
        TIM4->SR &= ~TIM_SR_UIF;

        // Alter the direction
        // The direction only changes if the ball is touching an object

        // Define variables for if the paddle buttons are pressed
        int paddle1D = (GPIOC->IDR & (1U<<0));
        int paddle1U = (GPIOC->IDR & (1U<<1));
        int paddle2D = (GPIOC->IDR & (1U<<2));
        int paddle2U = (GPIOC->IDR & (1U<<3));

        // Update the position of paddle 1
        // And the scalers for the ball position

        // If paddle1 moving up
        if (!paddle1U){
        	// Change the cords of the paddle
        	if (paddle1.y2 < 62){
        		paddle1.y1 += 2;
                paddle1.y2 += 2;
            }
        	// Increment the scalars for the ball velocity
            paddle1.xADD += 10;
            paddle1.yADD += 20;

        // If paddle1 moving down
        } else if (!paddle1D){
        	// Change the cords of the paddle
        	if (paddle1.y1 > 2){
        		paddle1.y1 -= 2;
        		paddle1.y2 -= 2;
            }
        	// Increment the scalars for the ball velocity
            paddle1.xADD += 10;
            paddle1.yADD += 20;

        // If paddle1 is stationary
        } else {
            paddle1.xADD = 0;
            if (paddle1.yADD > 0){
            	paddle1.yADD -= 10;
            }
        }

        // Update the position of paddle 2
        // And the scalars for the ball position

        // If paddle2 moving up
        if (!paddle2U){
        	// Change the cords of the paddle
        	if (paddle2.y2 < 62){
        		paddle2.y1 += 2;
        		paddle2.y2 += 2;
        	}
        	// Increment the scalars for the ball velocity
        	paddle2.xADD += 10;
        	paddle2.yADD += 20;

        // If paddle2 moving down
        } else if (!paddle2D){
        	// Change the cords of the paddle
        	if (paddle2.y1 > 2){
        		paddle2.y1 -= 2;
        		paddle2.y2 -= 2;
        	}
        	// Increment the scalars for the ball velocity
        	paddle2.xADD += 10;
        	paddle2.yADD += 20;
        // If paddle2 is stationary
        } else {
        	paddle2.xADD = 0;
        	if (paddle2.yADD > 0){
        		paddle2.yADD -= 10;
        	}
        }

        // Cap multipliers to prevent speed from endlessly increasing
       	if (paddle1.xADD > 40) { paddle1.xADD = 40; }
      	if (paddle2.xADD > 40) { paddle2.xADD = 40; }
        // yADD is linear, increase it slightly more than x
        if (paddle1.yADD > 60) { paddle1.yADD = 60; }
      	if (paddle2.yADD > 60) { paddle2.yADD = 60; }

        // Make sure ball is always moving 1 pixel along the x axis
        if (ball.xV > 0 && ball.xV < 100) { ball.xV = 100; }
        if (ball.xV < 0 && ball.xV > -100) { ball.xV = -100; }

        // Cap the absolute maximum velocity pre ADC scaling
       	if (ball.xV > 400) { ball.xV = 400; }
       	if (ball.xV < -400) { ball.xV = -400; }
       	if (ball.yV > 400) { ball.yV = 400; }
       	if (ball.yV < -400) { ball.yV = -400; }

       	// Update the position of the ball
       	int curxV = ball.xV; // Create a temp variable for X and Y axis velocity
        int curyV = ball.yV;

       	// Apply modADC to BOTH velocities to preserve the angle
        curxV = (curxV * modADC / 100);
       	curyV = (curyV * modADC / 100); // Scale Y by the ADC

       	// Cap the final rendered X speed after ADC scaling
        if (curxV > 500) { curxV = 500; }
       	if (curxV < -500) { curxV = -500; }

      	// Cap the final rendered Y speed after ADC scaling
       	if (curyV > 500) { curyV = 500; }
       	if (curyV < -500) { curyV = -500; }

       	// Update the ball coordinates using the fully scaled velocities
      	ball.subX += curxV;
      	ball.subY += curyV;

      	// Update the actual screen cords of the ball
       	ball.x1 = ball.subX / 100;
      	ball.x2 = ball.x1 + 3;
      	ball.y1 = ball.subY / 100;
      	ball.y2 = ball.y1 + 3;

        // Top / Bottom collision
        if (ball.y2 >= 63){
        	ball.subY = 5900;
        	ball.yV *= -1;
        } else if (ball.y1 <= 1){
        	ball.subY = 200;
        	ball.yV *= -1;
        }


        // Left side
        if (ball.x1 <= 4){
        	// Check if the ball is within the paddle
        	if (ball.y1 <= paddle1.y2 && ball.y2 >= paddle1.y1){
        		// Find the absolute value of the x axis velocity
        		int xV = ball.xV;
        	    if (xV < 0) { xV *= -1; }
                xV = (xV * (100 + paddle1.xADD)) / 100;

                // Move the ball out of range of the paddles
                ball.subX = 500;
                // Assign to global variable
                ball.xV = xV;

                // Check if the paddle is moving when it connects
                if (!paddle1U){
                	ball.yV += paddle1.yADD;
                } else if (!paddle1D){
                	ball.yV -= paddle1.yADD;
                }
        	}
        }

        // Right side
        else if (ball.x2 >= 123){
        	// Check if the ball is within the paddle
        	if (ball.y1 <= paddle2.y2 && ball.y2 >= paddle2.y1){
        		// Find the absolute value of the x axis velocity
        		int xV = ball.xV;
        		if (xV < 0) { xV *= -1; }
        		xV = (xV * (100 + paddle2.xADD)) / 100;

        		// Move the ball out of range of the paddles
        		ball.subX = 11900;
        		// Assign to global variable
        		ball.xV = xV * -1;

        		// Check if the paddle is moving when it connects
        		if (!paddle2U){
        			ball.yV += paddle2.yADD;
        		} else if(!paddle2D){
        			ball.yV -= paddle2.yADD;
        		}
        	}
        }

        // Set flag so main knows to refresh screen
        updateFlag = 1;

        // Check to see if a player has scored
        // Player 1 scored
        if (ball.x2 >= 126){
        	scored = 1;
        // PLayer 2 scored
        } else if (ball.x1 <= 1){
        	scored = 2;
        }

   }
}
