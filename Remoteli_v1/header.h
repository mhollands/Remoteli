/*
 * header.h
 *
 * Created: 29/12/2015 16:27:17
 *  Author: Matt
 */ 
#ifndef HEADER_H_
#define HEADER_H_

#define F_CPU 1000000

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/boot.h>
#include <avr/pgmspace.h>

void initialisePulseWrite(uint16_t*);
void startPulseWrite();
int isPulseWriteCompleted();
void stopPulseWrite();
void uninitialisePulseWrite();

void inistialisePulseRead();
int isPulseReadCompleted();
uint16_t* uninitialisePulseRead();

void boot_program_4pages(uint32_t memoryLocation, uint16_t *buf);
uint16_t* boot_read_4pages(uint32_t page);

void my_delay(uint16_t milliseconds);

char keyPressed();
char keyPressedS();

int watchDog;

#endif /* HEADER_H_ */