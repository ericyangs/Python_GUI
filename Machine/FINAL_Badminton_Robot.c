/*
 *  
 *  Final Combined Coding Test
 *
 *  Created: 2018-09-14
 *  Author : Jay Lee, YB
 *
 */ 


/*
	추가해야할 사항
	(디버깅 테스트 필요)
	
	1. hit_flag 각도 튜닝 필요.
    2. Wheels 속도 튜닝 필요. -> 테스트 결과 최고 속도로 움직여도 충분히 제어 가능, 다만 바퀴간 동일 PWM에 회전 속도가 다르므로 조정이 필요
    3. Master/Slave 통신으로 PWM제어 실험 필요.
	4. 부품조립 필요 (드릴, 볼트, 너트 필요)
    5. 주의사항, 바퀴 모터드라이버, 서보모터에 12V인가. 두꺼운 wire 사용할것.
*/



//#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART.h"

char buf[10];
volatile int i = 0;
volatile char recive_complete = 0;
unsigned int cnt = 0;
volatile int hit_flag = 0;

unsigned int stop_cnt = 0;
volatile int stop_flag = 0;

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
		if(cnt >= 26)   // <<== Need Tuning value ***
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

/*	if (stop_flag == 1) // 1 ~ 9 사이의 컨트롤 입력이 UART 통신을 통해 들어간 후에
	{
		if (++stop_cnt == 200) // 1초가 지나면
		{
			stop_flag = 0; // 플래그를 0으로 초기화 -> 이 카운터 구문의 동작을 멈춤
			stop_cnt = 0; // 카운터 변수를 초기화
			buf[0] = '0'; // buf[0]에 0을 집어넣고
			recive_complete = 1; // 송신이 완료되었다는 변수를 이용하여  while 문의 조건을 만족시켜 buf[0] = '0' 로 모든 바퀴를 정지
		}
	}
    */
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
	if (buf[0] >= '1' && buf[0] <= '9')
	{
		stop_flag = 1;
		// 여기서부터 시간을 세어 일정 시간이 지난후에 buf[0]를 0으로 바꾸어 정지하게 함 -> 만약 계속 조작하여 1~9 사이의 신호가 계속 들어오고 있다면 0이 바로 해당 신호로 바뀌게 됨

	if (buf[0] >= '1' && buf[0] <= '4') // a~f 사이의 문자가 시리얼로 들어올 경우 -> 모든 모터를 구동해야 제어가 가능하므로 하나로 묶음
	{
			OCR1B = 1999; // 1차 테스트 : 최고 속도로 회전시켜 사용자가 제어가 가능한지 확인해보기
			OCR1C = 1999;

			OCR3B = 1999;
			OCR3C = 1999;


			// 아래에는 각 바퀴의 방향 제어만

			if (buf[0] == '1')      // ↑
				PORTC = 0x55;           // 모두 앞방향으로 회전

			else if (buf[0] == '2') // ↓
				PORTC = 0xaa;           // 모두 뒷방향으로 회전

			else if (buf[0] == '3') // ←
				PORTC = 0x96;           // 좌+전은 뒤로, 좌+후는 앞으로, 우+전은 앞으로, 우+후는 뒤로 회전

			else if (buf[0] == '4') // →
				PORTC = 0x69;           // 좌+전은 앞으로, 좌+후는 뒤로, 우+전은 뒤로, 우+후는 앞으로 회전
		/*
				else if (buf[0] == ClkWise_Rotate)  // 시계 방향 회전
				PORTC = 0x5a;                       // 좌측 바퀴는 모두 앞으로, 우측 바퀴는 모두 뒤로 회전

				else if (buf[0] == countor_ClkWise_Rotate)  // 반시계 방향 회전
				PORTC = 0xa5;       // 좌측 바퀴는 모두 뒤로, 우측 바퀴는 모두 앞으로 회전
		*/
		}
		

		else if (buf[0] == '5')     // ↖
		{
			// 좌+후, 우+전만 앞으로 회전
			OCR1C = 1999;
			OCR3B = 1999;
			PORTC = 0x14;
		}

		else if (buf[0] == '6')      // ↙
		{
			// 좌+전, 우+후만 뒤로 회전
			OCR1B = 1999;
			OCR3C = 1999;
			PORTC = 0x82;
		}

		else if (buf[0] == '7')     // ↗
		{
			// 좌+전, 우+후만 앞으로 회전
			OCR1B = 1999;
			OCR3C = 1999;
			PORTC = 0x41;
		}

		else if (buf[0] == '8')     // ↘
		{
			// 좌+후, 우+전만 뒤로 회전
			OCR1C = 1999;
			OCR3B = 1999;
			PORTC = 0x28;
		}
		// 9는 여기다 추가
		// 여기에 buf[0]를 날리는 코드를 추가(timer 0 쓰기)
		_delay_ms(500);
	}
	else // 정해진 패킷 내의 문자가 들어오지 않거나 입력이 중단되었을 경우(0)
	{
		// 모든 모터의 PWM 값을 0으로 바꿔주고
		OCR1B = 0; // PB6
		OCR1C = 0; // PB7
		OCR3B = 0; // PE4
		OCR1C = 0; // PE5

		PORTC = 0;// 회전 방향을 정지로 바꿔줌
	}
}
