/*
 *  
 *  Control Slave BlueTooth connected with wheels by Master bluetooth which is connected with PC
 *
 *  Created: 2018-09-07
 *  Author : Jay Lee
 *
 */ 

#define F_CPU 16000000UL
#define TRUE 1
#define FALSE 0
#define BAUD_HIGH 115200
#define BAUD_LOW 9600

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART.h"

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
        recive_complete = TRUE;
    }

}

int main(void)
{

    // Receive the data from PC
    USART0_init(BAUD_LOW);
    // Transmit the date to Master Bluetooth
    USART1_init(BAUD_LOW);
    
    sei();
    
    // Send Mac address for connecting with Slave Bluetooth
    USART1_puts("AT+CON3CA30896A09A\r\n");
    
    while (1) 
    {
        if(recive_complete == TRUE)
        {
            USART1_puts(buffer);
            USART1_puts("\r\n");
            recive_complete = FALSE;
        }
    }
}