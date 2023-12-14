/*
 * MCP20231010.c
 *
 * Created: 2023-10-10 오전 9:08:37
 * Author : Herb
 */ 

#include <avr/io.h>

//import for using delay
#define F_CPU 16000000UL
#define FOSC 16000000UL
#include <util/delay.h>

//void led_all_onoff(int a,int _ms){
//	for(int i=0;i<a;i++){
//		PORTC=~(0xFF);
//		_delay_ms(_ms/2);
//		PORTC=~(0x00);
//		_delay_ms(_ms/2);
//	}
//}
void custom_delay(unsigned int _ms){
	for(int i=0;i<_ms;i++){
		_delay_ms(1);
	}
}

void led_all_onoff(unsigned int n,unsigned int _ms){
	for(unsigned int i=0;i<n;i++){
		PORTC=~(0xFF);
		custom_delay(_ms/2);
		PORTC=(0xFF);
		custom_delay(_ms/2);
	}
}

void led_alternation_onoff(unsigned char init,unsigned int n,unsigned int _ms){
	for(unsigned int i=0;i<n;i++){
		PORTC=~(init);
		custom_delay(_ms/2);
		PORTC=(init);
		custom_delay(_ms/2);
	}
}

//0x55 : 3번
//0x0F : 4번
//0x3C : 5번

int main(void)
{
	DDRC=0xFF;
	
    /* Replace with your application code */
    //while (1) {
	//	for(int i=0;i<8;i++){
	//		PORTC=~(0x01<<i);
	//		_delay_ms(300);	
	//	}
    //}
	//while(1){
	//for(int i=0;i<5;i++){
	//	PORTC=~(0xFF);
	//	_delay_ms(500);
	//	PORTC=~(0x00);
	//	_delay_ms(500);
	//}
// 	while(1){
// 		for(int i=0;i<3;i++){
// 			PORTC=(0x55);
// 			_delay_ms(500);
// 			PORTC=~(0x55);
// 			_delay_ms(500);
// 		}
// 		for(int i=0;i<4;i++){
// 			PORTC=(0x0F);
// 			_delay_ms(500);
// 			PORTC=~(0x0F);
// 			_delay_ms(500);
// 		}
// 		for(int i=0;i<5;i++){
// 			PORTC=(0x3C);
// 			_delay_ms(500);
// 			PORTC=~(0x3C);
// 			_delay_ms(500);
// 		}
// 	}
	led_all_onoff(2,1000);
	led_alternation_onoff(0x5C,2,1000);
}
//PINC : 입력값 읽는 레지스터
//PORTC
//DDRC : 0:In / 1:Out

//DDR -> PORT / PIN

//VCC -> 저항 -> LED - PIN
//LOW : ON / HIGH : OFF
