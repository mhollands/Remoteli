/*
 * flash_io.c
 *
 * Created: 15/01/2016 13:14:06
 *  Author: Matt
 */ 

#include "header.h"

uint16_t pulses_in[150];

//a memory location is 8 pages. one page is 16 words. one word is 2 bytes. one byte is 8 bits.
void boot_program_4pages(uint32_t memoryLocation, uint16_t *buf)
{
	uint32_t page = memoryLocation * 256;
	uint16_t i;
	uint8_t sreg;
	// Disable interrupts.
	sreg = SREG;
	cli();
	eeprom_busy_wait ();
	boot_page_erase (page); //erase 4 pages
	boot_spm_busy_wait (); // Wait until the memory is erased.
	eeprom_busy_wait ();
	boot_page_erase (page + 128); //erase 4 pages
	boot_spm_busy_wait (); // Wait until the memory is erased.
	for(int x = 0; x < 8; x++)
	{
		for (i=0; i<SPM_PAGESIZE; i+=2)
		{
			// Set up little-endian word.
			uint16_t w = *buf++;
			//w += (*buf++) << 8;
			boot_page_fill (page + i, w);
		}
		boot_page_write (page + x * SPM_PAGESIZE); // Store buffer in flash page.
		boot_spm_busy_wait(); // Wait until the memory is written.
		my_delay(100);
	}

	// Reenable RWW-section again. We need this if we want to jump back
	// to the application after bootloading.
	//boot_rww_enable ();
	// Re-enable interrupts (if they were ever enabled).
	SREG = sreg;
}

uint16_t* boot_read_4pages(uint32_t memoryLocation)
{
	uint32_t page = memoryLocation;
	uint8_t sreg;
	// Disable interrupts.
	sreg = SREG;
	//cli();
	eeprom_busy_wait ();
	uint16_t i;
	uint16_t mybyte;
	for (i=0; i<SPM_PAGESIZE*4; i+=1) {
		//mybyte = pgm_read_byte((void *)(i+ page*SPM_PAGESIZE));
		mybyte = pgm_read_word((void *)((i*2) +page*SPM_PAGESIZE));
		pulses_in[i] = mybyte;
	}
	SREG = sreg;
	return pulses_in;
}