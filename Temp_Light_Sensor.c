//
// ADC Setup for temperature and light sensor (photoresistor)
//
#include "stm32f4xx.h"                  // Device header
#include "Temp_Light_Sensor.h"

int TEMP_DATA = 0;
int LIGHT_DATA = 0;

void temp_init(void);
void ADC_IRQHandler(void);

void temp_init(void) {
	
	// initialize clock for Port A and ADC
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;		
	RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;	
	
	// initialize Port A pin 1 and 2 as ADC pin
	// For the ADC and DAC, configure the desired I/O as analog in the GPIOx_MODER register
	GPIOA->MODER |= (1<<2);
	GPIOA->MODER |= (1<<3); // set in analog mode
	GPIOA->MODER |= (1<<4);
	GPIOA->MODER |= (1<<5); // set in analog mode
	
	// Set resolution to 10 bit in CR1 register (default is 12 bit)
	ADC1->CR1 |= 0x01000000; //ADC_CR1_RES_0;
	ADC2->CR1 |= 0x01000000;
	
	// set scan mode
	ADC1->CR1 |= ADC_CR1_SCAN;
	ADC2->CR1 |= ADC_CR1_SCAN;
	
	// Set Sampling time to 84 (100) cycles in SMPR2 register (default is 000 3 cycles)
	ADC1->SMPR2 |= 0x00000020; //ADC_SMPR2_SMP1_2;
	ADC2->SMPR2 |= 0x00000020; //ADC_SMPR2_SMP1_2;
	
	// Set Continuous mode ADC (CR2 register)
	//ADC1->CR2 |= ADC_CR2_CONT; // don't want temp sensor to run continuously
	ADC2->CR2 |= ADC_CR2_CONT;

	// Specify channel number 1 of the 1st conversion for ADC1
	// Specify channel number 2 of the 1st conversion for ADC2
	ADC1->SQR3 |= ADC_SQR3_SQ1_0; //0x00000001
	ADC2->SQR3 |= ADC_SQR3_SQ1_1; //0x00000002
	
	//Set end of conversion interrupt in CR1 register
	ADC1->CR1 |= ADC_CR1_EOCIE;
	ADC2->CR1 |= ADC_CR1_EOCIE;
	
	// Turn on ADC conversion 
	ADC1->CR2 |= ADC_CR2_ADON;
	ADC2->CR2 |= ADC_CR2_ADON;
	
	// Start Conversion
	ADC1->CR2 |= ADC_CR2_SWSTART;
	ADC2->CR2 |= ADC_CR2_SWSTART;
	
	// LET NVIC handle ADC Interrupts
	__enable_irq();
	NVIC_ClearPendingIRQ(ADC_IRQn);
	NVIC_SetPriority(ADC_IRQn,0);
	NVIC_EnableIRQ(ADC_IRQn);

}

// INERRUPT HANDLER FOR ADC

void ADC_IRQHandler(void) {
	NVIC_ClearPendingIRQ(ADC_IRQn);
	// read the converted value from Register DR into ADC_DATA variable.
	// reading the DR clears the interrupt flag.
	TEMP_DATA = (ADC1->DR & 0x0000FFFF);
	LIGHT_DATA = (ADC2->DR & 0x0000FFFF);
}
