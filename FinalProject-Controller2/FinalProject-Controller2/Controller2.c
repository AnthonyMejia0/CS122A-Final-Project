
#define F_CPU 8000000UL		/* Define CPU Frequency e.g. here its 8MHz */
#include <avr/io.h>		/* Include AVR std. library file */
#include <stdio.h>		/* Include std. library file */
#include <util/delay.h>		/* Include Delay header file */
#include "sg90.h"
#include "timer.h"


void initPCInt() {
	SREG = 0x01; // Enable global interrupts
	PCICR = 0x01; // Enable Pin Change Interrupt 0 (Pins 7...0)
	PCMSK0 = 0x01; // Enable Pin Change Interrupt on PCINT0
}

ISR(PCINT0_vect){
	if (~PINA&0x01){
		PORTB ^= 0x01;
		if (currentPosition == 175){
			setAngle(300);
		}
		else {
			setAngle(175);
		}
	}
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	initPCInt();
	initServo();
	TimerSet(50);
	TimerOn();
	
	while(1)
	{
		while(!TimerFlag);
		TimerFlag = 0;
	}
}


