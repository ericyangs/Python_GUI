/*
 *  
 *  Rotating Test of the Front Wheels
 *
 *  Created: 2018-09-01
 *  Author : Jay Lee
 *
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


/*
 *  This is test for direction of wheels and RPM using by PWM signal.
 *  
 *  frequency : 1kHz  (1ms period)
 *
 *  ->
 */


void RightWheel()
{ 
    PORTB |= 0x01;           //  Turn on LED test
    
    // front direction           
    OCR3B = 0;
    OCR1B = 0;
    for(int i = 1 ; i<249; i++)
    {
        OCR3A = i;
        OCR1A = i;     
        _delay_ms(100);
    }
    
    PORTB &= 0x00;
}

void Leftwheel()
{
    PORTB |= 0x02;           //  Turn on LED test
    
    OCR3A = 0;
    OCR1A = 0;
    
    for(int i = 1 ; i<249; i++)
    {
        OCR3B = i;  
        OCR1B = i;    
        _delay_ms(100);
    }
    
    PORTB &= 0x00;
}


int main(void)
{
    DDRB |= 0x03;    // LED Test 
    
    DDRB |= (1 << PB5) + (1 << PB6) + (1 << PB7);	//   set PWM port at 'PB5' 'PB6' 'PB7' (OC1A , OC1B , OC1C)
    TCCR1A = 0xAA;									//   Set output Clear of 'A','B','C' of Timer/Counter 1, set Fast PWM
    TCCR1B = 0x1B;									//   Set Prescaler 64
    
    DDRE |= (1 << PE3) + (1 << PE4) + (1 << PE5);   //   set PWM port at 'PE3' 'PE4' 'PE5' (OC3A , OC3B, OC3C)
    TCCR3A = 0xAA;									//   Set output Clear of 'A','B','C' of Timer/Counter 3, set Fast PWM
    TCCR3B = 0x1B;									//   Set Prescaler 64

    OCR1A = 0;              //  initial value (any value)
    OCR1B = 0;
	OCR1C = 0;
    ICR1 = 249;

    OCR3A = 0;            
    OCR3B = 0;
	OCR3C = 0;    
    ICR3 = 249;             // (1/16)us * 64 * (1+249) = 1000us = 1ms
    
    
    while (1) 
    {
        RightWheel();
        _delay_ms(10);
        Leftwheel();
    }
}