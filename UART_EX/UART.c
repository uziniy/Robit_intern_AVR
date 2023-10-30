/*
 * UART_EX.c
 *
 * Created: 2023-08-11 오후 3:40:46
 * Author : yjchoi
 */ 

#define  F_CPU 16000000L //속도와 관련된건데 int가 여기서 2바이트라서 Long형이라고 L붙여줌
#include <avr/io.h> //AVR환경의 input output
#include <avr/interrupt.h>
#include <util/delay.h> //시간 관련

unsigned char Uart_Getch(void); //시리얼로부터 1byte를 받는 함수
void Uart_Putch(unsigned char PutData);//시리얼로부터 1byte를 보내는 함수


int main(void)
{
   UBRR0L = 16; //속도 선언 하는것 이떄 16은 1바이트 안넘어가니깐 H까지 쓸필요 없음
   UBRR0H = 0; //따라서 그냥 0 넣은것
   
   UCSR0A = 0x20; //00100000 인데 A의 5번째를 1로 활성화 해줘야만 송수신 가능함. UDRE0 = 1이어야지 송신 버퍼가 비어 새로운 송신 데이터를 받은 준비가 된 상태임
   UCSR0B = 0x18; //00011000 인데 B의 3,4번째는 RX와 TX를 사용한다고 선언 한것임.
   UCSR0C = 0x06; //00000110 인데 UCSZO0, UCSZO1을 11로 넣어주므로써 데이터를 8byte 쓸거라고 선언해준것. 표 보고 설정하면됨. UCSZN2는 B에서 설정해야함.
   
   DDRE 0x02; //00000010 -> 1일때 출력 0일때 입력이니깐 E0:입력핀 설정, E1:출력핀 설정
   
   sei(); //모든 인터럽트 활성화
   
    while (1) 
    {
		char recvData = Uart_Getch();//LED 몇번째가 켜져있는지를 보냄 -> i로 몇번짼지 알게 했으니깐 i+1값을 보내서 통신
		Uart_Putch(recvData);
    }
}

unsigned char Uart_Getch(void){
	while(!(UCSR0A & (1<<7))); //A가 10000000이면 아직 안읽은 데이터가 존재한다는 의미니깐 A랑 10000000이랑 연산했는데 같으면 1이 나오게 되고 !이니깐 0이되어 반복이 끝남. 즉 이 while문은 읽을 데이터가 있을때까지 계속 멈춰있겠다는 의미.
	return UDR0; //읽은 데이터 UDR0은 송수신 둘 모드 다 사용하니깐 읽을 데이터를 리턴함 -> recvData에 저장
}
void Uart_Putch(unsigned char PutData){
	while(!(UCSR0A & (1<<5))); //똑같이 이번엔 UDRE0이 00100000일떄 송신 버퍼가 비어서 새로운 데이터를 받을 준비가 된거니깐 기다렸다가
	UDR0 = PutData; //보낼 데이터를 저장해줌.	
}

//이 코드들이 돌아가면서 송수신을 무한으로 할 수 있는 환경이 구현됨.