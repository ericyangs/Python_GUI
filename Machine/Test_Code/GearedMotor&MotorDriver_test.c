/*
 *  
 *  Using Geared Motor Module 'L298N'
 *  Timer/Counter 3 Multi-PWM Test
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
 *
 *  this is test for Multi-PWM using by Timer/Counter 3 A and B.
 *  The RPM of DC motor depends on the PWM width.
 *  Also, you can change the motor direction as shifting the signal A and B.
 *  
 *  pin information (left side)
 *      1. IN1 - PE3 (OC3A)
 *      2. IN2 - PE4 (OC3B)
 *      
 *  If you want to add the one more motor at the right side, you need to add more 2 PWM signal.
 *  
 *  사용: 기어드모터, 모터드라이버 모듈(L298N)
 *  
 *  기어드모터 포트 OUT1, OUT2에 기어드모터 VCC, GND 연결.
 *  IN1, IN2 를 PE3, PE4와 연결. (서로 극성을 바꿔가며 회전방향을 바꿈, PWM을 바꿔가며 속도를 변화시킴)
 *  
 */




// Clockwise direction, increase the rotating speed using by PWM (2000 ~ 20000)
void rightwheel()
{ 
    PORTB = 0xFF;           //  Turn on LED test
               
    OCR3B = 0;

    for(int i = 2000 ; i<20000; i++)
    {
        OCR3A = i;      
        _delay_ms(1);
    }  
}

void leftwheel()
{
    PORTB = 0x00;           //  Turn off LED test
    
    OCR3A = 0;

    for(int i = 2000 ; i<20000; i++)
    {
        OCR3B = i;      
        _delay_ms(1);
    }
}

int main(void)
{
    DDRB = 0xFF;    // LED Test 
    
    DDRE |= 0x18;           //   set PWM port at 'PE3' 'PE4' (OC3A , OC3B)
    TCCR3A = 0xA2;          //   Set output Clear of 'A' & 'B' of Timer/Counter 3, set Fast PWM
    TCCR3B = 0x1A;          //   Set Prescaler 8

    OCR3A = 999;            // inital value, (any value)
    OCR3B = 999;
    ICR3 = 39999;           // (1/16)us * 8 * (1+39999) = 20000us = 20ms
    
    
    while (1) 
    {
        rightwheel();
        leftwheel();
    }
}