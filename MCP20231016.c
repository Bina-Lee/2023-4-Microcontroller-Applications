/*
 * MCP20231016.c
 *
 * Created: 2023-10-16 오후 3:02:23
 * Author : Herb
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#define FOSC 16000000UL
#include <util/delay.h>

#define delay_ms_value 100

void custom_delay(unsigned int _ms){
	for(int i=0;i<_ms;i++){
		_delay_ms(1);
	}
}

void shiftFunction(int shiftV,unsigned int _ms){
	for(int i=0;i<shiftV-1;i++){
		PORTC=~(0x01<<i);
		custom_delay(_ms);
	}
	for(int i=8-shiftV;i<7;i++){
		PORTC=~(0x01<<(7-i));
		custom_delay(_ms);
	}
}


int main(void)
{
	DDRC=0xFF;
	DDRL=~0x07;
	
	unsigned char in;
	unsigned char prev_in;
	
	unsigned char now=0x01;
	
	PORTC=~now;
    /* Replace with your application code */
    while (1) {
//    	for(int i=0;i<7;i++){
//    		PORTC=~(0x01<<i);
//			//PORTC=0xFE<<i;
//    		_delay_ms(delay_ms_value);
//   	}
//		for(int i=0;i<7;i++){
//			PORTC=~(0x01<<(7-i));
//			_delay_ms(delay_ms_value);
//		}
//		for(int i=1;i<8;i++)shiftFunction(i,delay_ms_value);
		//if(!(PINL & 0x01)){
		//	shiftFunction(4,delay_ms_value);
		//}
		in=(PINL & 0x07);
		//PORTC=in | ~0x07;
		if(~(in & 0x01)){
			if((in & 0x01) && (~prev_in & 0x01)){
				now=now<<1;
				if(now==0x00)now=0x01;
				PORTC=~now;
				_delay_ms(200);
			}
		}
		if(~(in & 0x02)){
			if((in & 0x02) && (~prev_in & 0x02)){
				now=now>>1;
				if(now==0x00)now=0x80;
				PORTC=~now;
				_delay_ms(200);
			}
		}
		prev_in=in;
    }
}

