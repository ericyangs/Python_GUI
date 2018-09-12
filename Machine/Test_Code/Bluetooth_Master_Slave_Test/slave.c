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
 *  BlueTooth slave mode
 *
 **/

char getValue[30];
char buffer[30];
volatile int i = 0;
volatile char recive_complete = FALSE;

// Receive the signal by interrupt from slave bluetooth
ISR(USART1_RX_vect)
{
    buffer[i] = UDR1;
    
    if(buffer[i++] == '\n' || buffer[i-1] == '\r')
    {
        buffer[i-1] = '\0';
        i = 0;
        recive_complete = TRUE;
    }

}

int main(void)
{
    DDRB = 0xFF;    // LED Test 

    USART1_init(BAUD_LOW);
    
    sei();
    
    while (TRUE) 
    {
        if(recive_complete == TRUE)
        {

			/*
            if(buffer[0]=='1')
            {
                PORTB = 0x01;
            }
            else if(buffer[0]=='2')
            {
                PORTB = 0x02;
            }
            else if(buffer[1]=='3')
            {
                PORTB = 0x04;
            }
            else if(buffer[2]=='4')
            {
                PORTB = 0x08;
            }
            else if(buffer[3]=='5')
            {
                PORTB = 0x10;
            }*/
			switch (buffer[0])
			{
			case '1':
				PORTB = 0x01;
				break;
			case '2':
				PORTB = 0x02;
				break;
			case '3':
				PORTB = 0x04;
				break;
			case '4':
				PORTB = 0x08;
				break;
			default:
				PORTB = 0xFF;
			}
            recive_complete = FALSE;
        }
    }
}