

#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"
#include "lcd.h"
#include "timer.h"
#include "keypad.h"
#include "usart_ATmega1284.h"
<<<<<<< Updated upstream

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
=======

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
				}
				USART_Flush(0);
			}
			break;
			
		default:
			follow_state = Recieve;
			break;
>>>>>>> Stashed changes
	}
}


int main(void)
{
	DDRA = 0xF0; PORTA = 0x0F;
<<<<<<< Updated upstream
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
=======
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
>>>>>>> Stashed changes
		while(!TimerFlag);
		TimerFlag = 0;
	}
	
	return 0;
}

