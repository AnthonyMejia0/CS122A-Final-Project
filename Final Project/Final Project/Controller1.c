#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "timer.h"
#include "keypad.h"
#include "usart_ATmega1284.h"

unsigned char array[4] = {'1', '2', '3', '4'};
unsigned char array_1[4] = {'9', '8', '7', '6'};
void getPin(int pass){
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
	if (pass == 0){
		for (int i = 0; i < 4; i++){
			if (array2[i] != array[i]){
				same = 0;
			}
		}
	}
	else if (pass == 1){
		for (int i = 0; i < 4; i++){
			if (array2[i] != array_1[i]){
				same = 0;
			}
		}
	}
	
	if (same == 1){
		PORTB ^= 0x01;
		LCD_DisplayString(1, "Access Granted!");
		PORTB &= 0xF7;
		_delay_ms(1500);
		PORTB |= 0x08;
	}
	else {
		LCD_DisplayString(1, "Access Denied!");
		PORTB &= 0xFB;
		_delay_ms(1500);
		PORTB |= 0x04;
	}
	
	_delay_ms(1000);
	
	LCD_DisplayString(1, "Scan a Card...");
}

unsigned char temp;
enum Follower_States{Recieve}follow_state;
void FollowerTick() {
	switch (follow_state) {
		case Recieve:
			if (USART_HasReceived(0)){
				temp = USART_Receive(0);
				if (temp == 255){
					LCD_DisplayString(1, "Scan a Card...");
				}
				else if (temp == 254){
					getPin(0);
				}
				else if (temp == 248){
					getPin(1);
				}
				else {
					LCD_DisplayString(1, "Tag not valid.");
					PORTB &= 0xFB;
					_delay_ms(1500);
					PORTB |= 0x04;
				}
				USART_Flush(0);
			}
			break;
			
		default:
			follow_state = Recieve;
			break;
	}
}


int main(void)
{
	DDRA = 0xF0; PORTA = 0x0F;
	DDRB = 0xFF; PORTB = 0x0C;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	initUSART(0);
	USART_Flush(0);
	LCD_init();
	TimerSet(10);
	TimerOn();
	follow_state = Recieve;
		
	while(1)
	{
		FollowerTick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
	
	return 0;
}

