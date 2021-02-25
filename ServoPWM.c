/*
* Servo PWM control
*/
#include "stm32f407xx.h"
#include "LCD.h"
#include "ServoPWM.h"

#define PWM_PD15 15
//USER LEDs are in PortD
//#define LED_ORANGE 13
//#define LED_GREEN 12
//#define LED_RED 14
//#define LED_BLUE 15

void servo_degree(int deg); 
void servo_init(void);

void servo_init(void) {
	
	// Enable GPIOD, TIM4, Blue Button clocks
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	
	// Set GPIOD pin 15 for Alternate Function
	GPIOD->MODER |= (1<<31);
	GPIOD->MODER &= ~(1<<30);
	GPIOD->AFR[1] |= 0x20000000; // AF2 (TIM3...5) 
	
	// Configure the output pin: 
	// Select the output mode by writing CCS bits in CCMR2 register
	TIM4->CCMR2 &= ~(1<<9);
	TIM4->CCMR2 &= ~(1<<8); // output
	
	// Select the polarity by writing the CC4P bit in CCER register.
	TIM4->CCER &= ~TIM_CCER_CC4P; // active high
	
	// Select the PWM mode (PWM1 or PWM2) by writing OC4M bits in CCMR2 register
	TIM4->CCMR2 |= 0x6000; // PWM mode 1
	
	// Program the period and the duty cycle respectively in ARR and CCR4 registers. 
	TIM4->PSC = 25;
	TIM4->ARR = 16000; // period
	TIM4->CCR4 = 1600; // duty cycle
	
	// SET the preload bit in CCMR2 register and the ARPE bit in the CR1 register.
	TIM4->CCMR2 |= TIM_CCMR2_OC4PE;
	TIM4->CR1 |= TIM_CR1_ARPE;
	
	TIM4->CR1 &= ~TIM_CR1_CMS; // edge-aligned mode
	TIM4->CR1 |= TIM_CR1_DIR; // upcounter
	
	// Enable capture compare
	TIM4->CCER |= TIM_CCER_CC4E;
	// Enable counter
	TIM4->CR1 |= TIM_CR1_CEN;

}

//=============================================================================
// Servo Turn in Degrees
// Inputs: takes integer value 180 or 90
// Outputs: NONE
// Affect: Turns Servo 180 or 90 degrees
//=============================================================================
void servo_degree(int deg) 
{
	if (deg==90) {
		TIM4->PSC = 15;
	} else if (deg==180) {
		if ((TIM4->PSC & 0x0000FFFF) == 5) {
			TIM4->PSC = 25;
		} else if ((TIM4->PSC & 0x0000FFFF) == 25) {
			TIM4->PSC = 5;
		} else if ((TIM4->PSC & 0x0000FFFF) == 15) {
			TIM4->PSC = 5;
		}
	}
	
}
