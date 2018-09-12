/*
 *  Bluetooth with Motor test
 *  
 *
 *  Created: 2018-08-28 10:39:42 PM
 *  Author : Jay Lee
 *
 *  <atmega128> pin      <Bludtooth>
 *  PD2(RXD 1)      ---->   TXD
 *  PD3(TXD 1)      ---->   RXD
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

char getValue[30];
char buffer[30];
volatile int i = 0;
volatile char recive_complete = FALSE;

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
    DDRB = 0xFF;        //  Set Port B as output for LED test
    PORTB = 0x00;       //  LED off  
    
    DDRE |= 0x08;           //   set PWM port at 'PE3'
    TCCR3A = 0x82;          //   Set output Clear of 'A' of Timer/Counter 3, set Fast PWM
    TCCR3B = 0x1A;          //  Set Prescaler 8
    OCR3A = 999;            //  initial "0 degree" of Motor     0 ~ 180 degree (range 999 ~ 4999)
    ICR3 = 39999;           // (1/16)us * 8 * (1+39999) = 20000us = 20ms*/
    
    USART0_init(BAUD_LOW);    //  initializing UART 0 for checking value at Monitor
    USART1_init(BAUD_LOW);    //  initializing UART 1 for BlueTooth
    sei();                //  Interrupt Enable
    
    while (TRUE) 
    {
        if(recive_complete == TRUE)
        {
            // get value (to see the data by Serial Monitor)
            sprintf(getValue,"[0]: %c, [1]: %c, [2]: %c, [3]: %c, [4]: %c",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
            USART0_puts(getValue);
            USART0_puts("\n\r");
            
            recive_complete = FALSE;
        }
        
        // led for debug
        switch(buffer[0])
        {
            case '0':
                PORTB = 0x00;
                OCR3A = 0;
                break;

            case '1':
                PORTB = 0x01;
                OCR3A = 15999;
                _delay_ms(100);
                OCR3A = 7999;
                break;

            case '2':
                PORTB = 0x02;
                OCR3A = 9999;
                break;
            case '3':
                PORTB = 0x04;
                OCR3A = 15999;
                break;
            case '4':
                PORTB = 0x08;
                OCR3A = 18999;
                break;
            case '5':
                PORTB = 0x08;
                OCR3A = 24000;
                break;
        }
        /*
        // LED ON Test after getting value
        if(buffer[0] == '0')
        {
            PORTB = 0x00;
            OCR3A = 0;
        }
        else if(buffer[0] == '1')
        {
            PORTB = 0x01;
            OCR3A = 15999;
            _delay_ms(100);
            OCR3A = 7999;
        }
        else if(buffer[0] == '2')
        {
            PORTB = 0x02;
            OCR3A = 9999;
        }
        else if(buffer[0] == '3')
        {
            PORTB = 0x04;
            OCR3A = 15999;
        }
        else if(buffer[0] == '4')
        {
            PORTB = 0x08;
            OCR3A = 18999;
        }
        else if(buffer[0] == '5')
        {
            PORTB = 0x08;
            OCR3A = 24000;
        }
        */
    }
}

