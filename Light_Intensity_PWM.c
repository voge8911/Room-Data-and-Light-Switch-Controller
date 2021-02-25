/*
* LED PWM control
*/
#include "stm32f407xx.h"
#include "LCD.h"
#include "Light_Intensity_PWM.h"
#include "Temp_Light_Sensor.h"


void LDR_LED(int deg); 
void LDR_LED_init(void);

void LDR_LED_init(void) {
	
	// Enable GPIOA, TIM3	clocks
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	
	// Set GPIOA pin 6 for Alternate Function
	GPIOA->MODER |= (1<<13);
	GPIOA->MODER &= ~(1<<12);
	GPIOA->AFR[0] |= (1<<25);		// AF2 (TIM3...5)
	GPIOA->AFR[0] &= ~(1<<24);
	GPIOA->AFR[0] &= ~(1<<26);
	GPIOA->AFR[0] &= ~(1<<27);
	// Configure the output pin: 
	// Select the output mode by writing CCS bits in CCMR1 register
	TIM3->CCMR1 &= ~(1<<1);
	TIM3->CCMR1 &= ~(1<<0); // output[00]
	
	// Select the polarity by writing the CC4P bit in CCER register.
	TIM3->CCER &= ~TIM_CCER_CC1P; // active high
	
	// Select the PWM mode (PWM1 or PWM2) by writing OC1M bits in CCMR1 register
	TIM3->CCMR1 |= (1<<6); // PWM mode 1 [110]
	TIM3->CCMR1 |= (1<<5);
	TIM3->CCMR1 &= ~(1<<4);
	
	// Program the period and the duty cycle respectively in ARR and CCR4 registers. 
	
	TIM3->ARR = 16000; // period
	TIM3->CCR4 = 8000; // duty cycle
	
	// SET the preload bit in CCMR2 register and the ARPE bit in the CR1 register.
	TIM3->CCMR1 |= TIM_CCMR1_OC1PE;
	TIM3->CR1 |= TIM_CR1_ARPE;
	
	TIM3->CR1 &= ~TIM_CR1_CMS; // edge-aligned mode
	TIM3->CR1 |= TIM_CR1_DIR; // upcounter
	
	// Enable capture compare
	TIM3->CCER |= TIM_CCER_CC1E;
	// Enable counter
	TIM3->CR1 |= TIM_CR1_CEN;

}

//=============================================================================
// LDR Dependent LED
// Inputs: takes integer value X
// Outputs: NONE
// Affect: Turns on LED with Corresponding PWM
//=============================================================================
//void LDR_LED(int LIGHT_DATA) 
//{
//	if (LIGHT_DATA==90) {
//		TIM4->PSC = 15;
//	} else if (LIGHT_DATA==180) {
//		if ((TIM4->PSC & 0x0000FFFF) == 5) {
//			TIM4->PSC = 25;
//		} else if ((TIM4->PSC & 0x0000FFFF) == 25) {
//			TIM4->PSC = 5;
//		} else if ((TIM4->PSC & 0x0000FFFF) == 15) {
//			TIM4->PSC = 5;
//		}
//	}
//	
//}
