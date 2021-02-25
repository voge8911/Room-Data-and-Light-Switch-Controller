// This module controls the PIR sensor

#include <stm32f407xx.h>
#include "PIR.h"
#include "LCD.h"
#include "ServoPWM.h"


int MotionSensed = 0;

void PIR_Init(void);
void EXTI0_IRQHandler(void);

#define PIR_Pin 0

void PIR_Init(void) {
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;								// turn on A clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;								// turn on D clock
	
	GPIOA->MODER &= ~(0x3<<(PIR_Pin+1));									// set PA0 to Input [00]
	GPIOD->MODER |= (1<<26);															// set PD13 to output [01]
	GPIOD->MODER &= ~(1<<27);
	
	SYSCFG->EXTICR[0] &= ~(0x000F);
	EXTI->IMR |= (1<<PIR_Pin);
	EXTI->RTSR |= (1<<PIR_Pin);
	
	
	__enable_irq();
	NVIC_SetPriority(EXTI0_IRQn,0);
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI0_IRQn);

}

	
void EXTI0_IRQHandler(void) {
	EXTI->PR |= (1<<PIR_Pin);
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
		
		MotionSensed = 1;
		place_lcd_cursor(0xC7); //line 2, block 7
		LCD_write_string("Sensed!!!");
		GPIOD->ODR |= (1<<13); // turn on Motion Sensed LED
		servo_degree(90);
		
		delay(500);
		MotionSensed = 0;
		GPIOD->ODR &= ~(1<<13); // turn off Motion Sensed LED
		place_lcd_cursor(0xC7); //line 2, block 7
	  LCD_write_string("Deadband ");	
		
		delay(1500);
		place_lcd_cursor(0xC7); //line 2, block 7
		LCD_write_string("Searching");	
	
}
