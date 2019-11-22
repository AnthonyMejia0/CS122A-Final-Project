
#ifndef SG90_H
#define SG90_H

int currentPosition;

/*
	Values
	65  = -90 degrees
	175 = 0 degrees
	300 = 90 degrees
*/
void setAngle(int value){
	OCR1A = value;
	currentPosition = value;
}

void initServo(){
	DDRD |= (1<<PD5);	/* Make OC1A pin as output */
	TCNT1 = 0;		/* Set timer1 count zero */
	ICR1 = 2499;		/* Set TOP count for timer1 in ICR1 register */

	/* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/64 */
	TCCR1A = (1<<WGM11)|(1<<COM1A1);
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10)|(1<<CS11);
}


#endif //SG90_H
