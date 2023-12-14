/*
 * MCP20231023AS.c
 *
 * Created: 2023-10-23 오후 11:14:45
 * Author : Herb
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#define FOSC 16000000UL
#include <util/delay.h>

// #include <avr/interrupt.h>

unsigned char val;

void set_led(){
	PORTC=val | 0xF0;
	PORTA=val | 0x0F;
}

void custom_delay(unsigned int _ms){
	for(int i=0;i<_ms;i++){
		_delay_ms(1);
	}
}

void led_shift(){
	for(int i=1;i<8;i++){
		val=~(0x01<<i);
		set_led();
		custom_delay(100);
	}
	for(int i=1;i<8;i++){
		val=~(0x01<<(7-i));
		set_led();
		custom_delay(100);
	}
}
void led_all_on_off(){
	val=0x00;
	set_led();
	custom_delay(500);
	val=0xFF;
	set_led();
	custom_delay(500);
}
void led_alternating(){
	val=0x55;
	set_led();
	custom_delay(500);
	val=~0x55;
	set_led();
	custom_delay(500);
}

int main(void)
{
	DDRA=0xFF;
	DDRC=0xFF;
	
	DDRL=~0x07;
	
	unsigned char in;
	val=0xFF;
	set_led();
	custom_delay(100);
    /* Replace with your application code */
    while (1) 
    {
		in=(~(PINL) & 0x07);
		if((in & 0x01)){
			for(int i=0;i<3;i++){
				led_shift();
			}
		}
		else if(in & 0x02){
			for(int i=0;i<3;i++){
				led_all_on_off();
			}
		}
		else if(in & 0x04){
			for(int i=0;i<3;i++){
				led_alternating();
			}
		}
    }
}

