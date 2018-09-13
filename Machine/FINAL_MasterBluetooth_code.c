/*
 *  
 *  Test for Master/Slave mode communication
 *
 *  Created: 2018-09-05
 *  Author : Jay Lee
 *
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART.h"

/*
 *
 *  BlueTooth Master mode
 *  
 *  
 *  <information of used Bluetooth HM-10>
 *  Master MAC address: 3CA308A10C6A
 *  Slave  MAC address: 3CA30896A09A
 */

char buffer[30];
volatile int i = 0;
volatile char recive_complete = 0;

// Receive the data from Serial communication of PC
ISR(USART0_RX_vect)
{
    buffer[i] = UDR0;
    
    // 통신에서 '새줄, New Line'으로 보내야함.
    if(buffer[i++] == '\n' )
    {
        buffer[i-1] = '\0';
        i = 0;
        recive_complete = 1;
    }

}

int main(void)
{

    // Receive the data from PC
    USART0_init(9600);
    // Transmit the date to Master Bluetooth
    USART1_init(9600);
    
    sei();
    
    // Send Mac address for connecting with Slave Bluetooth
    USART1_puts("AT+CON3CA30896A09A\r\n");
    
    while (1) 
    {
        if(recive_complete == 1)
        {
            // check the data at PC
            USART0_puts(buffer);
            USART0_puts("\r\n");
            
            USART1_puts(buffer);
            USART1_puts("\r\n");
            recive_complete = 0;
        }
    }
}