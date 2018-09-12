/*
 *  
 *  Wheel control with bluetooth
 *
 *  Created: 2018-09-07
 *     Author    : Jay Lee
 *  Collaborator : Yu bin Park
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

char buf[30];
volatile int i = 0;
volatile char recive_complete = FALSE;

void ControlMotor(void);

// Receive the values by interrupt from slave bluetooth
// receive the value slave bluetooth
ISR(USART1_RX_vect)
{
    buf[i] = UDR1;
    
    if(buf[i++] == '\n' || buf[i-1] == '\r')
    {
        buf[i-1] = '\0';
        i = 0;
        recive_complete = TRUE;
    }

}

// define receive code from 'a' ~ 'k' in a sequencial order
enum
{
    UP = 'a',
    DOWN, // 'b'
    LEFT, // 'c'
    RIGHT, // 'd'

    // Clockwise Rotation, Counter Clockwise Rotation
    // rotate clockwise/counter clockwise
    CLKWISE_ROTATE, // 'e'
    COUNTER_CLKWISE_ROTATE, // 'f'
    
    UP_LEFT, // 'g'
    UP_RIGHT, // 'h'
    DOWN_LEFT, // 'i'
    DOWN_RIGHT, // 'j'
    
    // Hit a target
    HIT // 'k'
};

int main(void)
{
 /* 포트 c를 전후진 제어용 포트로 씀
	00 00 00 00 상위 비트부터 좌+전, 좌+후, 우+전, 우+후
 	각각 바퀴의 회전 방향을 조작할 때 01은 전진, 10은 후진, 00는 정지로 설정
 */
    // Control the wheels of forward or backward by all of Port C
    DDRC = 0xFF;
    
    DDRB |= (1 << PB6) + (1 << PB7);	//   set PWM port at 'PB6' 'PB7' (OC1B, OC1C)
    TCCR1A = 0x2A;						//   Set output Clear of 'B','C' of Timer/Counter 1, set Fast PWM
    TCCR1B = 0x1A;						//   Set Prescaler 8
        
    DDRE |= (1 << PE4) + (1 << PE5);   //   set PWM port at 'PE4' 'PE5' (OC3B, OC3C)
    TCCR3A = 0x2A;						//   Set output Clear of 'B','C' of Timer/Counter 3, set Fast PWM
    TCCR3B = 0x1A;						//   Set Prescaler 8

    //  initial value (any value)
    //  1/16 * 8 * (1 + 1999) = 1000us = 1ms = 1kHz 주기로 파형이 나감
    OCR1B = 0;                   
    OCR1C = 0;
    ICR1 = 1999;

    OCR3B = 0;
    OCR3C = 0;
    ICR3 = 1999;
    
    USART1_init(BADU_LOW);
    
    sei();
    
    while (TRUE) 
    {
        if(recive_complete == TRUE)
        {
            ControlMotor();
            recive_complete = FALSE;
        }
    }
}

void ControlMotor(void)
{
    //  check data validation
    if (buf[0] >= 'a' && buf[0] <= 'f') // a~f 사이의 문자가 시리얼로 들어올 경우 -> 모든 모터를 구동해야 제어가 가능하므로 하나로 묶음
    {
        OCR1B = 1999; // 1차 테스트 : 최고 속도로 회전시켜 사용자가 제어가 가능한지 확인해보기
        OCR1C = 1999;
        
        OCR3B = 1999;
        OCR3C = 1999;

        // 아래에는 각 바퀴의 방향 제어만
        
        switch(buf[0])
        {
            case 0x61: //'a'
                PORTC = 0x55;
                break;
            case 0x62: //'b'
                PORTC = 0xAA;
                berak;
            case 0x63: //'c'
                PORTC = 0x96;
                break;
            case 0x64: // 'd'
                PORTC = 0x69;
                break;
            case 0x65: // 'e'
                PORTC = 0x5A;
                break;
            case 0x66: // 'f'
                PORTC = 0xA5;
                break;
        }
        /*
        if (buf[0] == up)      // ↑
        PORTC = 0x55;           // 모두 앞방향으로 회전

        else if (buf[0] == down) // ↓
        PORTC = 0xaa;           // 모두 뒷방향으로 회전

        else if (buf[0] == left) // ←
        PORTC = 0x96;           // 좌+전은 뒤로, 좌+후는 앞으로, 우+전은 앞으로, 우+후는 뒤로 회전

        else if (buf[0] == right) // →
        PORTC = 0x69;           // 좌+전은 앞으로, 좌+후는 뒤로, 우+전은 뒤로, 우+후는 앞으로 회전

        else if (buf[0] == ClkWise_Rotate)  // 시계 방향 회전
        PORTC = 0x5a;                       // 좌측 바퀴는 모두 앞으로, 우측 바퀴는 모두 뒤로 회전

        else if (buf[0] == countor_ClkWise_Rotate)  // 반시계 방향 회전
        PORTC = 0xa5;                               // 좌측 바퀴는 모두 뒤로, 우측 바퀴는 모두 앞으로 회전
        */
    }
    else if (buf[0] == upLeft)     // ↖
    {
        // 좌+후, 우+전만 앞으로 회전
        OCR1C = 1999;
        OCR3B = 1999;
        PORTC = 0x14;
    }
    else if(buf[0] == downLeft)      // ↙
    {
        // 좌+전, 우+후만 뒤로 회전
        OCR1B = 1999;
        OCR3C = 1999;
        PORTC = 0x82;
    }
    else if (buf[0] == upRight)     // ↗
    {
        // 좌+전, 우+후만 앞으로 회전
        OCR1B = 1999;
        OCR3C = 1999;
        PORTC = 0x41;
    }
    else if (buf[0] == downRight)     // ↘
    {
        // 좌+후, 우+전만 뒤로 회전
        OCR1C = 1999;
        OCR3B = 1999;
        PORTC = 0x28;
    }
    // exception handling
    else // 정해진 패킷 내의 문자가 들어오지 않거나 입력이 중단되었을 경우
    {
        // 모든 모터의 PWM 값을 0으로 바꿔주고
        OCR1B = 0; // PB6
        OCR1C = 0; // PB7
        OCR3B = 0; // PE4
        OCR1C = 0; // PE5

        PORTC = 0;// 회전 방향을 정지로 바꿔줌
    }
}