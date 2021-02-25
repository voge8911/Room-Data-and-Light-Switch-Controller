#include "stm32f4xx.h" 
#include "LCD.h"
#include "ServoPWM.h"
#include "PIR.h"
#include "TempSensor.h"
#include "LightSensorADC.h"

void display_temp(void);

int main(void){
	PIR_Init();
	LCD_port_init();
	LCD_init();
	
	temp_init();
	
	
	while (1) {
		delay(500);
		display_temp();
		LCD_Clear_Display();
	}
}

void display_temp(void) {
	temp_init();
	delay(500);
	double temp_celcius = (TEMP_DATA-0.5)/10.0;
	LCD_write_string("Temp ");
	LCD_write_num(temp_celcius);
	LCD_SendData(0xDF); // ° degree symbol
	LCD_write_string("C");
}
