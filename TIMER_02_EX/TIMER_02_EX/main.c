/*
 * TIMER_02_EX.c
 *
 * Created: 2023-08-11 오후 4:05:54
 * Author : yjchoi
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned int count8ms = 0;

int main(void)
{
	DDRA = 0xFF; //LED쓸건데 얘들이 A포트에 있으니깐 활성화 시킨겨 1이 출력모드니깐.
	
	TCNT0 = 131; //공식 : 주기 = prescaler/16000000 * (256 - TCNT)인데 주기가 8이니깐 대충 맞게 조율하면 prescaler = 1024, TCNT0 = 131 넣어야 맞음
	TCCR0 = 0x07; //TCCR0 register -> 00000111이면 1024를 의미 표 보면됨.
	TIMSK = 0x01; //오버플로우 인터럽트 활성화 얘를 해줘야 오버플로우 쓸 수 있음
	PORTA = 0xFF;
	sei();//인터럽트 활성화
	
    while (1) 
    {
    }
}

ISR(TIMER0_OVF_vect){ //오버플로우 날때마다 인터럽트 실행
	count8ms++; // 8ms 마다 인터럽트 실행되면서 1씩 늘려감
	if (count8ms == 125) //8*125=1000 -> 1초 되니깐 이렇게 1초를 표현할 수 있음.
	{
		if(PINA==0xFF){ //켜져있으면
			PORTA = 0x00; //끄기
		}
		else if (PINA == 0x00){ //꺼져 있으면
			PORTA = 0xFF; //켜기
		}
		TCNT0 = 131; //오버플로우 났다는건 top 찍었다는 거니간 다시 시작하도록 초기화 해줌.
	}
}