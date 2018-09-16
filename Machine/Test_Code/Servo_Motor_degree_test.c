/*
 *  
 *  Servo Motor Degree tuning of our Swing Machine.
 *
 *  Created: 2018-09-14
 *  Author : Jay Lee, YB
 *
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART.h"

unsigned int cnt = 0;
volatile int hit_flag = 0;

// Interrupt per 0.1ms
ISR(TIMER0_COMP_vect)
{
	cnt++;
	
	// 0 degree of the hitting machine
    if(hit_flag == 0)
    {
        // Minimum value is 6. 작동할 수 있는 가장 최소값 6.
        if(cnt >= 6)    //  <<== Need Tuning value ***
        {
            PORTE &= ~(1 << PE0);
        }
        else
        {
            PORTE |= (1 << PE0);
        }
    }
    
	//	180 degree of the hitting machine
	if(hit_flag == 1)
	{
		// Maximum value is 24, 값이 24일때, 180도 돌아감.
        if(cnt >= 24)   // <<== Need Tuning value ***
		{
			PORTE &= ~(1 << PE0);
		}
		else
		{
			PORTE |= (1 << PE0);
		}
	}
	// 0.1 * 200 = 20ms
	if(cnt == 200)
		cnt = 0;

}

int main(void)
{
    // For controlling the wheels of forward or backward by all of Port C
	USART1_init(9600);

	DDRC = 0xFF;
    
	// For frequency of Hitting machine using Timer/counter 0 Interrupt
	DDRE |= (1 << PE0);
	//PORTE |= (1 << PE0\);	//	default 'HIGH'
					
    //Timer/counter0 interrupt for Servo motor
    // CTC mode, 8 prescaler, to make 0.1ms
    // (1/16)us * 8 * (1 + 199) = 100us = 0.1ms
    TCCR0 = 0x0A;
    OCR0 = 199;
    TIMSK = 0x02;   //  Timer/Counter0 Comparative match interrupt enable
    
    // Global Interrupt Enable
    sei();
    
    while (1) 
    {
      hit_flag = 1;
      _delay_ms(2000);
      hit_flag = 0;
      _delay_ms(2000);
    }
}
