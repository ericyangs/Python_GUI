/*
 *  
 *  Final Combined Coding Test
 *
 *  Created: 2018-09-10
 *  Author : Jay Lee
 *
 */ 


/*
	추가해야할 사항
	(디버깅 테스트 필요)
	
	1. hit_flag 각도 튜닝 필요.
    2. Wheels 속도 튜닝 필요.
    3. Master/Slave 통신으로 PWM제어 실험 필요.
	4. 부품조립 필요 (드릴, 볼트, 너트 필요)
    5. 주의사항, 바퀴 모터드라이버, 서보모터에 12V인가. 두꺼운 wire 사용할것.

*/



#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

char buf[10];
volatile int i = 0;
volatile char recive_complete = 0;
unsigned int cnt = 0;
volatile int hit_flag = 0;

// define receive code from 'a' ~ 'k'
enum
{
    up = 'a',
    down,
    left,
    right,

    // Clockwise Rotation, Counter Clockwise Rotation
    ClkWise_Rotate,
    countor_ClkWise_Rotate,
    
    upLeft,
    upRight,
    downLeft,
    downRight,
    
    hit
};

void ControlMotor(void);

// Receive the values by interrupt from slave Bluetooth
ISR(USART1_RX_vect)
{
    buf[i] = UDR1;
    
    if(buf[i++] == '\n')
    {
        buf[i-1] = '\0';
        i = 0;
        recive_complete = 1;
    }
}

// Interrupt per 0.5ms
ISR(TIMER0_COMP_vect)
{
	cnt++;
	
	// 0 degree of the hitting machine
	
    if(hit_flag == 0)
    {
        if(cnt >= 7)    //  <<== Need Tuning value ***
        {
            PORTB &= ~(1 << PB3);
        }
        else
        {
            PORTB |= (1 << PB3);
        }
    }
    
	//	180 degree of the hitting machine
	if(hit_flag == 1)
	{
		if(cnt >= 26)   // <<== Need Tuning value ***
		{
			PORTB &= ~(1 << PB3);
		}
		else
		{
			PORTB |= (1 << PB3);
		}
	}
	// 0.1 * 200 = 20ms
	if(cnt == 200)
		cnt = 0;
    
}

int main(void)
{
    // For controlling the wheels of forward or backward by all of Port C
	DDRC = 0xFF;
    
	// For frequency of Hitting machine using Timer/counter 0 Interrupt
	DDRB |= (1 << PB3);
	//PORTB |= (1 << PB3);	//	default 'HIGH'
					
    //Timer/counter0 interrupt for Servo motor
    // CTC mode, 8 prescaler, to make 0.1ms
    // (1/16)us * 8 * (1 + 199) = 100us = 0.1ms
    TCCR0 = 0x0A;
    OCR0 = 199;
    TIMSK = 0x02;   //  Timer/Counter0 Comparative match interrupt enable
    
    
    DDRB |= (1 << PB6) | (1 << PB7);	//   set PWM port at 'PB6' 'PB7' (OC1B , OC1C)
    TCCR1A = 0x2A;						//   Set output Clear of 'B','C' of Timer/Counter 1, set Fast PWM
    TCCR1B = 0x1A;						//   Set Prescaler 64
    
    DDRE |= (1 << PE4) + (1 << PE5);   //   set PWM port at 'PE4' 'PE5' (OC3B, OC3C)
    TCCR3A = 0x2A;						//   Set output Clear of 'B','C' of Timer/Counter 3, set Fast PWM
    TCCR3B = 0x1A;						//   Set Prescaler 64

    //  initial value (any value)
    //  1/16 * 8 * (1 + 1999) = 1000us = 1ms = 1kHz
    OCR1B = 0;
    OCR1C = 0;
    ICR1 = 1999;

    OCR3B = 0;
    OCR3C = 0;
    ICR3 = 1999;
    
    // Global Interrupt Enable
    sei();
    
    while (1) 
    {
        if(recive_complete == 1)
        {
            ControlMotor();
            recive_complete = 0;
        }       
    }
}

void ControlMotor(void)
{
    if (buf[0] >= 'a' && buf[0] <= 'f') // a~f 사이의 문자가 시리얼로 들어올 경우 -> 모든 모터를 구동해야 제어가 가능하므로 하나로 묶음
    {
        OCR1B = 1999; // 1차 테스트 : 최고 속도로 회전시켜 사용자가 제어가 가능한지 확인해보기
        OCR1C = 1999;
        
        OCR3B = 1999;
        OCR3C = 1999;

        // 아래에는 각 바퀴의 방향 제어만
        
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