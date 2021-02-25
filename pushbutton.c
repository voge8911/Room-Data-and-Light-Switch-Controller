// pushbutton interrupt module
// pushbutton toggles the servo on and off

#include <stm32f407xx.h>
#include "LCD.h"
#include "pushbutton.h"
#include "ServoPWM.h"

int toggle = 0x0; //allows button to toggle servo on and off

void pushbutton_init(void);
void EXTI3_IRQHandler(void);

#define Pushbutton_Pin 3

void pushbutton_init(void) {
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;								// turn on C clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	GPIOC->MODER &= ~(1<<6);									// set PC3 to Input [00]
	GPIOC->MODER &= ~(1<<7);
	GPIOC->MODER |= (1<<2);									// set PC1 to Output [01]
	GPIOC->MODER &= ~(1<<3);
	
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PC;
	EXTI->IMR |= (1<<Pushbutton_Pin);
	EXTI->RTSR |= (1<<Pushbutton_Pin);

	
	__enable_irq();
	NVIC_ClearPendingIRQ(EXTI3_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
}

void EXTI3_IRQHandler(void) {
	EXTI->PR |= (1<<Pushbutton_Pin);
	NVIC_ClearPendingIRQ(EXTI3_IRQn);
	delay(500); //debounce
	if(toggle==0x0) {
		GPIOC->ODR |= (1<<1);
	} else {
		GPIOC->ODR &= ~(1<<1);
	}
	
	toggle ^= 0xF;
}
