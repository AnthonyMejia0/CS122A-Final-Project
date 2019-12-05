
#ifndef SG90_H
#define SG90_H

int currentPosition;

void setAngle(int value){
	OCR1A = value;
	currentPosition = value;
}

void initServo(){
	DDRD |= (1<<PD5);	
	TCNT1 = 0;		
	ICR1 = 2499;		

	TCCR1A = (1<<WGM11)|(1<<COM1A1);
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10)|(1<<CS11);
}


#endif //SG90_H
