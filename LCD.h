#include "stm32f4xx.h" 
#include <string.h>

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
