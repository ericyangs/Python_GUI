/*
 * PWM test.c
 *
 * Created: 2018-08-13 오후 5:33:42
 * Author : Jay Lee
 
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int main(void)
{
    DDRB = 0xFF;    // LED Test 
    
    DDRE |= 0x08;           //   set PWM port at 'PE3'
    TCCR3A = 0x82;          //   Set output Clear of 'A' of Timer/Counter 3, set Fast PWM
    TCCR3B = 0x1A;          //  Set Prescaler 8
    OCR3A = 999;            //  initial "0 degree" of Motor     0 ~ 180 degree (range 999 ~ 4999)
    ICR3 = 39999;           // (1/16)us * 8 * (1+39999) = 20000us = 20ms*/
    
    /*
            ※ Notice
            
            you should set the extern Voltage up to 7.6V for running Servo Motor
			(The voltage level depends on the weight)
    */
    
    
    while (1) 
    {
      
        PORTB = 0xFF;           //  Turn on LED @ 0 degree
        OCR3A = 1140;           //  value: 1140~1790  (0~180 degree),  value : 1150 ~ 1900 (0~230 degree) 7.6V설정.
        _delay_ms(5000);
        PORTB = 0x00;           //  Turn off LED @ target degree
        OCR3A = 1790;           //  
        _delay_ms(5000);
    }
}

