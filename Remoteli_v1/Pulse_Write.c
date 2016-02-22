/*
 * Pulse_Write.c
 *
 * Created: 29/12/2015 16:26:19
 *  Author: Matt
 */ 

#include "header.h"

void setPulseLength();
void togglePulse();

int pulsePosition = 0;
uint16_t* pulses_out;
char completed = 0;

void initialisePulseWrite(uint16_t *pulses)
{	
	completed = 0;
	pulses_out = pulses;
	//SET UP CARRIER CLOCK
	OCR0A = 12;
	TCCR0A |= (1 << WGM01); //enable CTC mode

	//SET UP DATA CLOCK
	TIFR |= (1 << OCF1A); //clear output compare interrupt flag
	TIMSK |= (1 << OCIE1A); // enable timer compare interrupt
	TCCR1B |= (1 << WGM12); //enable CTC mode
	TCCR1A &= ~((1 << WGM11) | (1 << WGM10)); //enable CTC mode
}

void uninitialisePulseWrite()
{
	stopPulseWrite();
	TIMSK &= ~(1 << OCIE1A); // disable timer compare interrupt
	TCCR1B &= ~(1 << WGM12); //enable Normal mode
	TCCR1A &= ~((1 << WGM11) | (1 << WGM10)); //enable Normal mode
	TCNT1 = 0;
}

void startPulseWrite()
{
	pulsePosition = 0;
	TCNT1 = 0;
	setPulseLength();
	togglePulse();
	TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10); // set clock prescaler
}

int isPulseWriteCompleted()
{
	return (completed == 1);
}

void inline stopPulseWrite()
{
	TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10)); // stop data clock
	TCCR0A &= ~(1 << COM0A0); //disconnect from pin
	TCCR0B &= ~(1 << CS00); //stop carrier clock
	completed = 1;
}

void inline setPulseLength()
{
	OCR1A = pulses_out[pulsePosition]; // set timer TOP
}

void inline togglePulse()
{
	TCCR0A ^= (1 << COM0A0); //toggle connection to pin
	TCCR0B ^= (1 << CS00); //toggle carrier clock
}

ISR(TIM1_COMPA_vect)
{
	togglePulse();
	pulsePosition++;
	//TCNT1 = 0;
	if(pulsePosition == 128 || pulses_out[pulsePosition] == 0)
	{
		stopPulseWrite();
		return;
	}

	setPulseLength();
}