/* Lab 4 - ENGR331
 * The goal of this lab is help you figure out
 * (on your own) how to interface to a peripheral
 * that you have never used before based on the
 * documentation you find (either datasheet or other
 * available sources/examples online).
 * ENJOY!!! (and I really mean it!)
 */

#include "stm32f4xx.h" 
#include <string.h>
#include "LCD.h"

/*******************************
 * LCD pins connections to PORTD
 *******************************
 */
#define RS 7
#define EN 6 

#define DB7 3
#define DB6 2
#define DB5 1
#define DB4 0

/*******************************
 * FUNCTION PROTOTYPES
 *******************************
 */
// Delay Functions
void tim6_delay(void);
void delay(int ms);

// LCD related functions
void LCD_port_init(void);
void LCD_init(void);
void LCD_write(unsigned char data);
void LCD_write_string(unsigned char data[]);
void place_lcd_cursor(unsigned char lineno);
void LCD_SendCmd(unsigned char c);
void LCD_SendData(unsigned char d);
void LCD_Clear_Display(void);
void LCD_write_num(double num);
void ImmediateMessage(unsigned char arr[]);
// END Functions

/*******************************
 * tim6_delay(void)
 * Inputs: NONE
 * Outputs: NONE
 * Based on PSC=0 and ARR=21000; 
 * we get delay of approximately 1.33ms
 *******************************
 */
void tim6_delay(void){
	// enable APB1 bus clock
	RCC->APB1ENR|=RCC_APB1ENR_TIM6EN;
	//TIM6 prescaler set at default to 0 for now
	TIM6->PSC=0; // prescalar
	TIM6->ARR = 21000;  //auto reload register 
	TIM6->CNT=0;   //clear counter register
	TIM6->CR1|=TIM_CR1_CEN;
	//WHEN COUNTER IS DONE THE TIM6_SR REG UIF FLAG IS SET
	while(TIM6->SR==0);
	TIM6->SR=0; //CLEAR uIF FLAG
}

/*******************************
 * delay(int ms)
 * Inputs: delay in milliseconds
 * Outputs: NONE
 * An approximate delay because  
 * call of tim6_delay() creates about 1.33ms
 *******************************
 */
void delay(int ms){
int i;
	for (i=ms; i>0; i--)
	{
		tim6_delay();
	}
}


/*******************************
 * LCD_port_init()
 * Inputs: NONE
 * Outputs: NONE
 * Port Initialization
 * Refer to the #define statements at top to
 * see what ports are used to connect
 * the STMicro Board with the HD44780 LCD driver
 * Set appropriate pins as digital input/outputs
 *******************************
 */
void LCD_port_init(){
//STEP 1: Enable GPIOD in RCC AHB1ENR register
	RCC->AHB1ENR = RCC_AHB1ENR_GPIODEN;
//STEP 2: Set MODER of GPIOD Pins 7, 6, 3, 2, 1 & 0 as outputs
	GPIOD->MODER |= 0x00005055; 
//STEP 3: Set OTYPER of GPIOD Pins 7, 6, 3, 2, 1 & 0 as push-pull 
  GPIOD->OTYPER &= ~(0x000000CF); 
//Done with LCD port Initialization
}

/*******************************
 * LCD_init()
 * Inputs: NONE
 * Outputs: NONE
 * LCD Initialization
 * Read the manual carefully
 * We are doing initialization by instruction
 * Don't rush it.
 *******************************
 */

void LCD_init(){

// STEP 1: Wait for 100ms for power-on-reset to take effect
	delay(100);
// STEP 2: Set RS pin LOW to send instructions
	GPIOD->ODR &= ~(1<<RS);
// Send instructions using following format:
// Set EN=HIGH; Send 4-bit instruction; Set EN=low; delay 20ms;
// STEP 3a-3d: Set 4-bit mode (takes a total of 4 steps)
	GPIOD->ODR |= (1<<EN); //EN=HIGH
	GPIOD->ODR &= ~(0x0000000F); // disable all
	GPIOD->ODR |= (1<<DB4)|(1<<DB5); // enable DB4 DB5
	GPIOD->ODR &= ~(1<<EN); //EN=LOW
	delay(20);
	GPIOD->ODR |= (1<<EN);
	GPIOD->ODR &= ~(0x0000000F);
	GPIOD->ODR |= (1<<DB4)|(1<<DB5); // enable DB4 DB5
	GPIOD->ODR &= ~(1<<EN);
	delay(20);
	GPIOD->ODR |= (1<<EN);
	GPIOD->ODR &= ~(0x0000000F);
	GPIOD->ODR |= (1<<DB4)|(1<<DB5); // enable DB4 DB5
	GPIOD->ODR &= ~(1<<EN);
	delay(20);
	GPIOD->ODR |= (1<<EN);
	GPIOD->ODR &= ~(0x0000000F);
	GPIOD->ODR |= (1<<DB5); // enable DB5
	GPIOD->ODR &= ~(1<<EN);
	delay(20);
// STEP 4: Set 2 line display -- treats 16 char as 2 lines
	GPIOD->ODR |= (1<<EN);
	GPIOD->ODR &= ~(0x0000000F);
	GPIOD->ODR |= (1<<DB5); // enable DB5
	GPIOD->ODR &= ~(1<<EN);
	delay(20);
	GPIOD->ODR |= (1<<EN);
	GPIOD->ODR &= ~(0x0000000F);
	GPIOD->ODR |= (1<<DB7); // enable DB7
	GPIOD->ODR &= ~(1<<EN);
	delay(20);
// STEP 5: Set DISPLAY to OFF
	GPIOD->ODR |= (1<<EN);
	GPIOD->ODR &= ~(0x0000000F); // 0000
	GPIOD->ODR &= ~(1<<EN);
	delay(20);
	GPIOD->ODR |= (1<<EN);
	GPIOD->ODR |= (1<<DB7); // enable DB7
	GPIOD->ODR &= ~(1<<EN);
	delay(20);
// STEP 6: CLEAR DISPLAY
	GPIOD->ODR |= (1<<EN);
	GPIOD->ODR &= ~(0x0000000F);
	GPIOD->ODR &= ~(1<<EN);
	delay(20);
	GPIOD->ODR |= (1<<EN);
	GPIOD->ODR |= (1<<DB4); // enable DB4
	GPIOD->ODR &= ~(1<<EN);
	delay(20);
// STEP 7: SET ENTRY MODE - Auto increment; no scrolling
	GPIOD->ODR |= (1<<EN);
	GPIOD->ODR &= ~(0x0000000F);
	GPIOD->ODR &= ~(1<<EN);
	delay(20);
	GPIOD->ODR |= (1<<EN);
	GPIOD->ODR |= (1<<DB5)|(1<<DB6); // enable DB5 DB6
	GPIOD->ODR &= ~(1<<EN);
	delay(20);
// STEP 8: Set Display to ON with Cursor and Blink.
	GPIOD->ODR |= (1<<EN);
	GPIOD->ODR &= ~(0x0000000F);
	GPIOD->ODR &= ~(1<<EN);
	delay(20);
	GPIOD->ODR |= (1<<EN);
	GPIOD->ODR &= ~(0x0000000F);
	GPIOD->ODR |= (1<<DB4)|(1<<DB5)|(1<<DB6)|(1<<DB7); // enable DB4 DB5 DB6 DB7
	GPIOD->ODR &= ~(1<<EN);
	delay(20);
}

/*******************************
 * place_lcd_cursor()
 * Inputs: unsigned character
 * Outputs: NONE
 * sets Cursor position to
 * Line 1, character 1 (hex address 0x80)
 * or Line 2, character 1 (hex addres 0xC0)
 *
 *******************************
 */

void place_lcd_cursor(unsigned char lineno){
	int line = lineno;
	if (line==49) {
		//set DDRAM address to Line 1, character 1 (hex address 0x80)
		LCD_SendCmd(0x80); //1000 0000
	} else if (line==50) {
		//set DDRAM address to Line 2, character 1 (hex address 0xC0)
		LCD_SendCmd(0xC0); //1100 0000
	} else {
		LCD_SendCmd(lineno);
	}	
}



/*******************************
 * LCD_write()
 * Inputs: unsigned character data (8-bit)
 * Outputs: NONE
 * writes the character to LCD.
 *
 *******************************
 */

void LCD_write(unsigned char data){
	GPIOD->ODR |= (1<<RS); // set RS to high to send data
	GPIOD->ODR |= (1<<EN);
	GPIOD->ODR &= ~(0x0000000F);
	GPIOD->ODR |= (data>>4); // send upper nibble of data
	GPIOD->ODR &= ~(1<<EN);
	delay(20);
	GPIOD->ODR |= (1<<EN);
	GPIOD->ODR &= ~(0x0000000F);
	GPIOD->ODR |= (data & 0x0F); // send lower nibble of data
	GPIOD->ODR &= ~(1<<EN);
	delay(20);
	GPIOD->ODR &= ~(1<<RS); // set RS pin to low, done sending data
}

/*******************************
 * LCD_write_string()
 * Inputs: unsigned character array data (8-bit)
 * Outputs: NONE
 * writes an array of characters to the LCD.
 *
 *******************************
 */
void LCD_write_string(unsigned char data[]){
	for(unsigned int i=0;i<strlen(data);i++)
	{
		LCD_write(data[i]);
	}
}
/*******************************
 * LCD_write_num()
 * Inputs: int num
 * Outputs: NONE
 * writes an integer to the LCD.
 *
 *******************************
 */
void LCD_write_num(double num){
	char str[16];
	sprintf(str, "%.1f", num);
	LCD_write_string(str);
}
/*******************************
 * ImmediateMessage()
 * Inputs: unsigned character array (8-bits)
 * Outputs: NONE
 * Displays a message on the LCD all at once
 *******************************
 */
void ImmediateMessage(unsigned char arr[]){
	place_lcd_cursor('1');
	LCD_SendCmd(0x08); //turn display off
	LCD_write_string(arr); // write string to LCD
	LCD_SendCmd(0x0C); //turn display on
}
/*******************************
 * LCD_SendCmd()
 * Inputs: unsigned 8-bit char
 * Outputs: NONE
 * sends a command to the LCD
 *
 *******************************
 */
void LCD_SendCmd(unsigned char c){
	GPIOD->ODR &= ~(1<<RS);
	GPIOD->ODR |= (1<<EN);
	GPIOD->ODR &= ~(0x0000000F); //0000
	GPIOD->ODR |= (c>>4);
	GPIOD->ODR &= ~(1<<EN);
	delay(20);
	GPIOD->ODR |= (1<<EN);
	GPIOD->ODR &= ~(0x0000000F); // 0000
	GPIOD->ODR |= (c & 0xF);
	GPIOD->ODR &= ~(1<<EN);
	delay(20);
}



/*******************************
 * LCD_SendCmd()
 * Inputs: unsigned 8-bit char
 * Outputs: NONE
 * sends data to the LCD
 *
 *******************************
 */
void LCD_SendData(unsigned char d){
	GPIOD->ODR |= (1<<RS);
	GPIOD->ODR |= (1<<EN);
	GPIOD->ODR &= ~(0x0000000F); //0000
	GPIOD->ODR |= (d>>4);
	GPIOD->ODR &= ~(1<<EN);
	delay(20);
	GPIOD->ODR |= (1<<EN);
	GPIOD->ODR &= ~(0x0000000F); // 0000
	GPIOD->ODR |= (d & 0xF);
	GPIOD->ODR &= ~(1<<EN);
	delay(20);
	GPIOD->ODR &= ~(1<<RS);
}
/*******************************
 * LCD_SendCmd()
 * Inputs: NONE
 * Outputs: NONE
 * Waits 1s then clears the display
 *
 *******************************
 */
void LCD_Clear_Display(void){
	delay(300);
	LCD_SendCmd(0x01); //clear display
}

