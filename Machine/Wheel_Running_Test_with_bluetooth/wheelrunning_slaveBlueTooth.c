/*
 *  
 *  Wheel control with bluetooth
 *
 *  Created: 2018-09-07
 *     Author    : Jay Lee
 *  Collaborator : Yu bin Pack
 *
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART.h"

char buf[30];
volatile int i = 0;
volatile char recive_complete = 0;

void ControlMotor(void);

// Receive the values by interrupt from slave bluetooth
// �����̺� ��������� ���� ����.
ISR(USART1_RX_vect)
{
    buf[i] = UDR1;
    
    if(buf[i++] == '\n' || buf[i-1] == '\r')
    {
        buf[i-1] = '\0';
        i = 0;
        recive_complete = 1;
    }

}

// define receive code from 'a' ~ 'k'
enum
{
    up = 'a',
    down,
    left,
    right,

    // Clockwise Rotation, Counter Clockwise Rotation
    // �ð����ȸ��, �ݽð����ȸ��
    ClkWise_Rotate,
    countor_ClkWise_Rotate,
    
    upLeft,
    upRight,
    downLeft,
    downRight,
    
    // Ÿ��
    hit
};

int main(void)
{
 /* ��Ʈ c�� ������ ����� ��Ʈ�� ��
	00 00 00 00 ���� ��Ʈ���� ��+��, ��+��, ��+��, ��+��
 	���� ������ ȸ�� ������ ������ �� 01�� ����, 10�� ����, 00�� ������ ����
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
    //  1/16 * 8 * (1 + 1999) = 1000us = 1ms = 1kHz �ֱ�� ������ ����
    OCR1B = 0;                   
    OCR1C = 0;
    ICR1 = 1999;

    OCR3B = 0;
    OCR3C = 0;
    ICR3 = 1999;
    
    USART1_init(9600);
    
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
    if (buf[0] >= 'a' && buf[0] <= 'f') // a~f ������ ���ڰ� �ø���� ���� ��� -> ��� ���͸� �����ؾ� ��� �����ϹǷ� �ϳ��� ����
    {
        OCR1B = 1999; // 1�� �׽�Ʈ : �ְ� �ӵ��� ȸ������ ����ڰ� ��� �������� Ȯ���غ���
        OCR1C = 1999;
        
        OCR3B = 1999;
        OCR3C = 1999;

        // �Ʒ����� �� ������ ���� ���
        
        if (buf[0] == up)      // ��
        PORTC = 0x55;           // ��� �չ������� ȸ��

        else if (buf[0] == down) // ��
        PORTC = 0xaa;           // ��� �޹������� ȸ��

        else if (buf[0] == left) // ��
        PORTC = 0x96;           // ��+���� �ڷ�, ��+�Ĵ� ������, ��+���� ������, ��+�Ĵ� �ڷ� ȸ��

        else if (buf[0] == right) // ��
        PORTC = 0x69;           // ��+���� ������, ��+�Ĵ� �ڷ�, ��+���� �ڷ�, ��+�Ĵ� ������ ȸ��

        else if (buf[0] == ClkWise_Rotate)  // �ð� ���� ȸ��
        PORTC = 0x5a;                       // ���� ������ ��� ������, ���� ������ ��� �ڷ� ȸ��

        else if (buf[0] == countor_ClkWise_Rotate)  // �ݽð� ���� ȸ��
        PORTC = 0xa5;                               // ���� ������ ��� �ڷ�, ���� ������ ��� ������ ȸ��
    }

    else if (buf[0] == upLeft)     // ��
    {
        // ��+��, ��+���� ������ ȸ��
        OCR1C = 1999;
        OCR3B = 1999;
        PORTC = 0x14;
    }

    else if(buf[0] == downLeft)      // ��
    {
        // ��+��, ��+�ĸ� �ڷ� ȸ��
        OCR1B = 1999;
        OCR3C = 1999;
        PORTC = 0x82;
    }

    else if (buf[0] == upRight)     // ��
    {
        // ��+��, ��+�ĸ� ������ ȸ��
        OCR1B = 1999;
        OCR3C = 1999;
        PORTC = 0x41;
    }

    else if (buf[0] == downRight)     // ��
    {
        // ��+��, ��+���� �ڷ� ȸ��
        OCR1C = 1999;
        OCR3B = 1999;
        PORTC = 0x28;
    }
    else // ������ ��Ŷ ���� ���ڰ� ������ �ʰų� �Է��� �ߴܵǾ��� ���
    {
        // ��� ������ PWM ���� 0���� �ٲ��ְ�
        OCR1B = 0; // PB6
        OCR1C = 0; // PB7
        OCR3B = 0; // PE4
        OCR1C = 0; // PE5

        PORTC = 0;// ȸ�� ������ ������ �ٲ���
    }
}