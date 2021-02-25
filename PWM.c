/*
* Servo PWM control
*/
#include "stm32f407xx.h"
#include "LCD.h"
#include "PWM.h"

#define PWM_PC9 9
//USER LEDs are in PortD
//#define LED_ORANGE 13
//#define LED_GREEN 12
//#define LED_RED 14
//#define LED_BLUE 15


void PWM_init(void);
void led_brightness(int percent);

void PWM_init(void) {
	
	// Enable GPIOC and TIM3 clocks
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	
	// Set GPIOC pin 9 for Alternate Function TIM3_CH4
	GPIOC->MODER |= (1<<19);
	GPIOC->MODER &= ~(1<<18);
	GPIOC->AFR[1] |= 0x00000020; // AF2 (TIM3...5) 
	
	// Configure the output pin: 
	// Select the output mode by writing CCS bits in CCMR2 register
	TIM3->CCMR2 &= ~(1<<9);
	TIM3->CCMR2 &= ~(1<<8); // output
	
	// Select the polarity by writing the CC4P bit in CCER register.
	TIM3->CCER &= ~TIM_CCER_CC3P; // active high
	
	// Select the PWM mode (PWM1 or PWM2) by writing OC4M bits in CCMR2 register
	TIM3->CCMR2 |= 0x6000; // PWM mode 1
	
	// Program the period and the duty cycle respectively in ARR and CCR4 registers. 
	TIM3->PSC = 25;
	TIM3->ARR = 16000; // period
	TIM3->CCR4 = 8000; // duty cycle
	
	// SET the preload bit in CCMR2 register and the ARPE bit in the CR1 register.
	TIM3->CCMR2 |= TIM_CCMR2_OC4PE;
	TIM3->CR1 |= TIM_CR1_ARPE;
	
	TIM3->CR1 &= ~TIM_CR1_CMS; // edge-aligned mode
	TIM3->CR1 |= TIM_CR1_DIR; // upcounter
	
	// Enable capture compare
	TIM3->CCER |= TIM_CCER_CC4E;
	// Enable counter
	TIM3->CR1 |= TIM_CR1_CEN;
	
}

//=============================================================================
// LED Brightness change
// Inputs: takes integer value
// Outputs: 
// Affect: changes LED Brightness
//=============================================================================
void led_brightness(int percent) 
{
	if (percent < 400) {
		TIM3->CCR4 = 0;
	} else {
		TIM3->CCR4 = round(16000*(percent/1000.0));
	}
	
}


