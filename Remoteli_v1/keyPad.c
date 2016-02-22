/*
 * keyPad.c
 *
 * Created: 15/01/2016 14:22:03
 *  Author: Matt
 */ 

#include "header.h"

char keyPressedS()
{
	for(int x = 0; x < 100; x++)
	{
		char k = keyPressed();
		if(k != 0)
		{
			return k;
		}
	}
	return 0;
}

char keyPressed()
{
	//*(PA2) *(PC4)
	//*(PA3)*(PB1)*(PC1)
	//*(PA7)*(PA1)!(PB3)
	//*(PB0)*(PC5)*(PB2)7
	
	if((PINA & 0b00000100) == 0) { return 1; }
	if((PINC & 0b00010000) == 0) { return 2; }
	if((PINA & 0b00001000) == 0) { return 3; }		
	if((PINB & 0b00000010) == 0) { return 4; }		
	if((PINC & 0b00000010) == 0) { return 5; }		
	if((PINA & 0b10000000) == 0) { return 6; }		
	if((PINA & 0b00000010) == 0) { return 7; }		
	if((PINB & 0b00001000) == 0) { return 8; }		
	if((PINB & 0b00000001) == 0 && watchDog == 1) { return 9; }		
	if((PINC & 0b00100000) == 0) { return 10; }		
	if((PINB & 0b00000100) == 0) { return 11; }		
	
	return 0;
}
