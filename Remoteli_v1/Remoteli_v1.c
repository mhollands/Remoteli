/*
 * Remoteli_v1.c
 *
 * Created: 29/12/2015 16:25:51
 * Author : Matt
 */ 

//*(PA2) *(PC4)
//*(PA3)*(PB1)*(PC1)
//*(PA7)*(PA1)!(PB3)
//*(PB0)*(PC5)*(PB2)7

//PC0 = LED
//PA6 = Receiver power
//PA5 = Receiver Signal

#include "header.h"
#include <util/delay.h>

void doRead(char button);

/* Replace with your library code */
int main(void)
{
	sei(); //enable global interrupts
	WDTCSR |= (1 << WDIE); //enable watchdog so PB3 works!
	WDTCSR |= (1 << WDP3) | (0 << WDP2) | (0 << WDP1) | (1 << WDP0); //set watchdog to be slow
	watchDog = 1; 
	DDRA = 0b01000000; // set all port a as inputs
	DDRB = 0b00000000; // set all port b as inputs
	DDRC = 0b00000001; //set all inputs on PORTC except 0 for the LED
	PUEB = 0b00001111; //set all pullup resistors on portb
	PUEA = 0b10011111; //set all pullup resistors on porta except those for the IR receiver
	PUEC = 0b11111110; //set all pullup resistors on portc except for the led
	
	PORTA = 0b01000000; //turn on IR receiver
	my_delay(1000);
	doRead(keyPressedS()); //if a button is pressed read command save in correct location
	
	char key = 0;
	while(1 == 1)
	{
		key = keyPressedS();
		if(key != 0)
		{
			doSend(key);
		}
	}
	return 0;
}

void fastFlash()
{
	for(int x = 0; x < 2; x++)
	{
		PORTC |= 0b00000001;
		my_delay(100);
		PORTC &= 0b11111110;
		my_delay(100);
	}
}

void doRead(char button)
{
	if(button > 0 && button <= 11) //if the button number is valid
	{
		initialisePulseRead(); //initialise timers etc
		while(!isPulseReadCompleted()){} //wait for read to be done
		uint16_t* pulses = uninitialisePulseRead(); //get pules and uninitialise timers
		boot_program_4pages(6 + button, pulses);// save pulses
		fastFlash(); //indicate that the code has been learnt	
	}
}

void doSend(char code)
{
	uint16_t* pulses = boot_read_4pages((6 + code) * 8);
	initialisePulseWrite(pulses);
	startPulseWrite();
	while(!isPulseWriteCompleted()){} //wait for write to be done
	uninitialisePulseWrite();
	my_delay(100);
}

ISR(WDT_vect)
{
	WDTCSR |= (1 << WDIE); //put WDT back into interrupt mode instead of reset mode!
}

