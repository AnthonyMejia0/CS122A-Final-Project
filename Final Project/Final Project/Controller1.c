

#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"
#include "lcd.h"
#include "timer.h"
#include "keypad.h"
#include "usart_ATmega1284.h"

enum Keypad_States { KP_wait, KP_press }key_state;
void keyTick() {
	unsigned char key_press;

	switch (key_state) {
		case KP_wait:
			key_press = GetKeypadKey();
			key_state = (key_press != '\0') ? KP_press: KP_wait;
			break;
		
		case KP_press:
			key_state = KP_wait;
			break;
		
		default:
			key_state = KP_wait;
			break;
	}
	

	switch(key_state) {
		case KP_wait:
			key_press = GetKeypadKey();
			break;
		
		case KP_press:
			LCD_Cursor(1);
			LCD_WriteData(key_press);
			break;
	}
}

enum states{Display}state;
unsigned char prevData;

void display_tick(){
	unsigned char data;
	
	switch(state){
		case Display:
		data = recievedData;
		if (data != prevData){
			LCD_ClearScreen();
		}
		
		unsigned char temp = data;
		if ((temp/100) != 0){
			LCD_Cursor(1);
			LCD_WriteData((temp/100) + '0');
			temp -= (100*(temp/100));
			LCD_Cursor(2);
			LCD_WriteData((temp/10) + '0');
			temp -= (10*(temp/10));
			LCD_Cursor(3);
			LCD_WriteData(temp + '0');
		}
		else if ((temp/10) != 0){
			LCD_Cursor(1);
			LCD_WriteData((temp/10) + '0');
			temp -= (10*(temp/10));
			LCD_Cursor(2);
			LCD_WriteData(temp + '0');
		}
		else {
			LCD_Cursor(1);
			LCD_WriteData(temp + '0');
		}
		prevData = data;
		state = Display;
		break;
		
		default:
		state = Display;
		break;
	}
}


int main(void)
{
	DDRA = 0xF0; PORTA = 0x0F;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	//initUSART(1);
	//USART_Flush(1);
	SPI_ServantInit();
	LCD_init();
	TimerSet(10);
	TimerOn();
	key_state = KP_wait;
	state = Display;
	
	while(1)
	{
		keyTick();
		//display_tick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
	
	return 0;
}

