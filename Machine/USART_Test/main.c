/*
 *  Bluetooth Signal Test
 *
 *  Created: 2018-08-27 10:00:42 PM
 *  Author : Jay Lee
 *
 *  <atmega128> pin      <Bludtooth>
 *  PD2(RXD 1)      ---->   TXD
 *  PD3(TXD 1)      ---->   RXD
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART.h"

char getValue[30];
char buffer[30];
volatile int i = 0;
volatile char recive_complete = 0;

ISR(USART1_RX_vect)
{
    buffer[i] = UDR1;
    
    if(buffer[i++] == '\n' || buffer[i-1] == '\r')
    {
        buffer[i-1] = '\0';
        i = 0;
        recive_complete = 1;
    }
    PORTB = 0xFF;   //  LED Test ON
}

int main(void)
{
    DDRB = 0xFF;        //  Set Port B as output for LED test
    PORTB = 0x00;       //  LED off  
    
    USART0_init(9600);    //  initializing UART 0 for checking value at Monitor
    USART1_init(9600);    //  initializing UART 1 for BlueTooth
    sei();                //  Interrupt Enable
    
    while (1) 
    {
        PORTB = 0x00;   //  LED Test ON
        if(recive_complete == 1)
        {
            // get value (to see the data by Serial Monitor)
            sprintf(getValue,"[0]: %c, [1]: %c, [2]: %c, [3]: %c, [4]: %c",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
            USART0_puts(getValue);
            USART0_puts("\n\r");
            
            recive_complete = 0;
        }
    }
}

