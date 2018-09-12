/*
 *  
 *  Test for Master/Slave mode communication
 *
 *  Created: 2018-09-05
 *  Author : Jay Lee, Yubin Park
 *
 */ 

#define F_CPU 16000000UL
#define BAUD_LOW 9600
#define BAUD_HIGH 115200
#define TRUE 1
#define FALSE 0

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART.h"

/*
 *
 *  BlueTooth Master mode
 *  Get the data from PC and toss to Atmega128 to forward the data to slave bluetooth
 *  
 */

char getValue[30];
char buffer[30];
volatile int i = 0;
volatile char recive_complete = FALSE;

// Receive the data from Serial communication of PC
ISR(USART0_RX_vect)
{
    buffer[i] = UDR0;
    
    if(buffer[i++] == '\n' || buffer[i-1] == '\r')
    {
        buffer[i-1] = '\0';
        i = 0;
        recive_complete = FALSE;
    }

}

int main(void)
{
    DDRB = 0xFF;    // LED Test 

    // receive the data from PC
    USART0_init(BAUD_LOW);
    // transmit the date to Master bluetooth
    USART1_init(BAUD_LOW);
    
    sei();
    
    while (TRUE) 
    {
        if(recive_complete == TRUE)
        {
            USART1_puts(buffer[0]);
            recive_complete = FALSE;
        }
    }
}