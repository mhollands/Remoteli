/*
 * Pulse_Read.c
 *
 * Created: 29/12/2015 16:29:19
 *  Author: Matt
 */ 

#include "header.h"

uint16_t pulses_in[150];
char begun = 0;
int count = 0;
char complete = 0;

void initialisePulseRead()
{
	WDTCSR &= ~(1 << WDIE); //disable watchdog
	watchDog = 0;
	begun = 0;
	count = 0;
	complete = 0;
	
	TIFR |= (1 << TOV1); //clear overflow interrupt flag
	TIMSK |= (1 << TOIE1); // enable overflow interrupt
	GIFR |= (1 << PCIF0); //clear interrupt flag
	PCMSK0 |= (1 << PCINT5); // enable PCINT5
	GIMSK |= (1 << PCIE0); // enable Pin change interrupt 0
}

int isPulseReadCompleted()
{
	return (complete == 1);	
}

uint16_t* uninitialisePulseRead()
{
	TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10)); // stop clock
	TIMSK &= ~(1 << TOIE1); // disable overflow interrupt
	PCMSK0 &= ~(1 << PCINT5); // disable PCINT5
	GIMSK &= ~(1 << PCIE0); // disable Pin change interrupt 0
	TCNT1 = 0; //reset timer counter
	WDTCSR |= (1 << WDIE); //enable watchdog!
	watchDog = 1;
	return pulses_in;
}

ISR(PCINT0_vect)
{
	if(begun == 0)
	{
		begun = 1;
		count = 0;
		TCNT1 = 0;
		TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10); // set clock prescaler
		
		return;
	}
	
	pulses_in[count] = TCNT1;
	TCNT1 = 0;
	count++;
}

ISR(TIM1_OVF_vect)
{
	TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10)); // stop clock
	
	for(int x = count; x < 150; x++)
	{
		pulses_in[x] = 0;
	}
	begun = 0;
	count = 0;
	complete = 1;
}
