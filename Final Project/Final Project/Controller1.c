#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "timer.h"
#include "keypad.h"
#include "usart_ATmega1284.h"
//#include "scheduler.h"

unsigned char array[4] = {'1', '2', '3', '4'};
void getPin(){
	unsigned char currentKey = '\0';
	unsigned char lastKey;
	signed char array2[4] = {'\0', '\0', '\0', '\0'};
	LCD_DisplayString(1, "Enter Pin: ");
	for (int i = 0; i < 4; i++)
	{
		do{
			lastKey = currentKey;
			currentKey = GetKeypadKey();
			if (currentKey != lastKey){
				array2[i] = currentKey;
				LCD_Cursor(12 + i);
				LCD_WriteData('*');
			}
		}
		while(currentKey == '\0' || lastKey == currentKey);
	}
		
	unsigned char same = 1;
	for (int i = 0; i < 4; i++){
		if (array2[i] != array[i]){
			same = 0;
		}
	}
	
	if (same == 1){
		PORTB ^= 0x01;
		LCD_DisplayString(1, "Access Granted!");
	}
	else {
		LCD_DisplayString(1, "Access Denied!");
	}
	
	_delay_ms(1000);
	
	LCD_DisplayString(1, "Scan a Card...");
}

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
unsigned char temp;

void display_tick(){
	switch(state){
		case Display:
		
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
			state = Display;
			break;
		
			default:
				state = Display;
				break;
	}
}


enum Follower_States{Recieve}follow_state;

void FollowerTick() {
	switch (follow_state) {
		case Recieve:
			break;
		
		default:
			follow_state = Recieve;
			break;
	}
	
	switch (follow_state) {
		case Recieve:
			if (USART_HasReceived(0)){
				temp = USART_Receive(0);
				//LCD_ClearScreen();
				//display_tick();
				USART_Flush(0);
				LCD_DisplayString(1, "Enter Pin.");
				getPin();
			}
			break;
	}
}



int main(void)
{
	DDRA = 0xF0; PORTA = 0x0F;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	initUSART(0);
	USART_Flush(0);
	LCD_init();
	TimerSet(10);
	TimerOn();
	key_state = KP_wait;
	follow_state = Recieve;
	
	LCD_DisplayString(1, "Scan a Card...");
		
	while(1)
	{
		//keyTick();
		FollowerTick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
	
	return 0;
}

