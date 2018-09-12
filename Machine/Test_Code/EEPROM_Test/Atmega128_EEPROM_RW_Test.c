/*
 *  Test of RW on EEPROM
 *  
 *  Created: 2018-08-29 10:39:42 PM
 *  Author : Jay Lee
 *  Technical Writing / Debug : Seungan Woo
 *  
 *  EEPROM Memory capacity: Max 4Kb, if overflows, fatal err arises
 *  
 *  
 *  <Internal System of the memory in AVR>
 *  
 *  1. Flash : [.test] -> code composed / [.bss, .data] -> global, local vars & function
 *  2. SRAM : volatile when power off, store [.bss], [.data] from flash, allocate the area of stack and heap
 *  3. EEPROM: non_volatile, MAX 4Kb(Atmega128), erasable when up-/downloading
 *  4. ALU: implement arithmetic calculation
 *  
 *  <Simulator Operation for debug in Atmel Studion7>
 *  1. Execute "Device Programming" (Ctrl + Shift + P)
 *  2. Select "simulator" instead of ISP down-loader
 *  3. Hit "Alt + F5" to enter debug mode
 *  4. Eliminate the mode by pressing "Ctrl + Shift + F5"
 */ 

#define F_CPU 16000000UL
#define TRUE 1
#define FALSE 0

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "USART.h"

// Write the data on EEPROM. Refer to the data sheet P.23
void EEPROM_write(unsigned int uiAddress, unsigned int ucData)
{
    while(EECR & (1 << EEWE));   //p.21: EECR(EEPROM Control Register), Check if EEWE is set as 1EEWE(EEPROM Write Enable)
    
    EEAR = uiAddress;           //  EEPROM Address Register
    EEDR = ucData;              //  EEPROM Data Register
    EECR |= (1 << EEMWE);       //  EEPROM Master Write Enable
    EECR |= (1 << EEWE);        //  Start EEPROM write by setting EEWE
}

// read the data from EEPROM
unsigned char EEPROM_read(unsigned int uiAddress)
{
    while(EECR & (1 << EEWE)); // p.24: wait until previous writing completes
    
    EEAR = uiAddress;           //  setup address register
    EECR |= (1 << EERE);        //  when enable, start to read data from EEROM
    
    return EEDR; // return the data read
}

int main()
{
    unsigned char i = 0;
    
    DDRB = 0xFF;    // LED Test
    PORTB = 0x00;   // initial LED off
    
    // Store the value to the EEPROM
	// Even if commenting, the data keeps remaining on the EEPROM

    //EEPROM_write(0x100, 0xAA);
    
    i = EEPROM_read(0x100);
    
    while(TRUE)
    {
        PORTB = i;
    }
    
    
    return 0;
}