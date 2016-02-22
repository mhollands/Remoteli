/*
 * fake_delay.c
 *
 * Created: 15/01/2016 13:41:47
 *  Author: Matt
 */ 

#include "header.h"

void delay_one_ms();

void my_delay(uint16_t milliseconds)
{
	for(uint16_t x = 0; x < milliseconds; x++)
	{
		delay_one_ms();
	}
}

void delay_one_ms()
{
	volatile uint8_t x = 150;
	while(--x)
	{
		asm ("NOP");
	}
}

