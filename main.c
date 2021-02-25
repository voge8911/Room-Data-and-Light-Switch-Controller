#include "stm32f4xx.h" 
#include "LCD.h"
#include "ServoPWM.h"
#include "PIR.h"
#include "Temp_Light_Sensor.h"
#include "Light_Intensity_PWM.h"
#include "PWM.h"
#include "pushbutton.h"

void display_temp(void);

int main(void){
	PIR_Init();
	LDR_LED_init();
	LCD_port_init();
	LCD_init();
	servo_init();
	PWM_init();
	pushbutton_init();
	
	temp_init();
	place_lcd_cursor('1');
	LCD_write_string("Temp ");
	place_lcd_cursor('2');
	LCD_write_string("Motion:");
	
	
	
	while (1) {
		led_brightness(LIGHT_DATA);
		delay(3000);
		display_temp();
		
	}
}

void display_temp(void) {
	temp_init();
	delay(500);
	double temp_celcius = (TEMP_DATA-50)/10.0;
	double temp_fahrenhiet = (temp_celcius*(9/5.0) + 32);
	place_lcd_cursor(0x85);
	LCD_write_num(temp_fahrenhiet);
	LCD_SendData(0xDF); // ° degree symbol
	LCD_write_string("F     ");
}

