/*
 *      author : Jay Lee
 *
 *      L298N motor driver module control
 *      
 */


#define F_CPU                     16000000UL
#define CLOCKWISE                 PORTA = 0b01010000
#define WAIT                      PORTA = 0x00;
#define COUNTERCLOCKWISE          PORTA = 0b10100000
#define TRUE                      1
#define FALSE                     0


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>




int main(void)
{
    // using Port A
    DDRA = 0xFF;

    
    /*
            ※ Notice
            
          1.  Port "OUT1, OUT2, OUT3, OUT4" are connected with DC motors ( you don't care of connection of VCC, GND. these are decided just direction of rotating )
          2.  You have to connect the extern source on port "+12V" or "+5V" with "GND". and Extern Ground have to connect with board's ground (Equalize the board ground with Extern ground)
          3.  You should enable port "ENA" and "ENB" by bridge connector
          4.  You can connect maximum 2 DC motor's VCC and GND at each port "IN1, IN2" and "IN3, IN4".
              Refer to my bit setting of PORTA which are for rotating 2 direction (clockwise and counter clockwise)
              You have to focus on bit 4 to 7 of PORTA (clockwise : [1010 0000], counter clockwise : [0101 0000])
          
          예시)
          왼쪽모터
          IN1, IN2 가 1,0 일때, 시계방향 회전.
          IN1, IN2 가 0,1 일때, 반시계방향 회전.
          
          오른쪽모터
          IN3, IN4 가 1,0 일때, 시계방향 회전.
          IN3, IN4 가 0,1 일때, 반시계방향 회전.
    */
    
    
    while (1) 
    {

        
        CLOCKWISE;
        _delay_ms(3000);
        WAIT;
        _delay_ms(1000);
        
        COUNTERCLOCKWISE;
        _delay_ms(3000);
        WAIT;
        _delay_ms(1000);
    }
}