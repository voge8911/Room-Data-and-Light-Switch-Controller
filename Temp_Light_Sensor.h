#include "stm32f4xx.h" 
#include <string.h>

extern int TEMP_DATA; // Global variable to store the ADC conversion
extern int LIGHT_DATA;

// FUNCTION PROTOTYPES

extern void ADC_IRQHandler(void);
void temp_init(void);
