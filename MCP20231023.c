/*
 * MCP20231023.c
 *
 * Created: 2023-10-23 오후 3:26:30
 * Author : Herb
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#define FOSC 16000000UL
#include <util/delay.h>

#include <avr/interrupt.h>

void led_shift(){
	for(int i=0;i<7;i++){
		PORTC=~(0x01<<i);
		_delay_ms(100);
	}
	for(int i=0;i<7;i++){
		PORTC=~(0x01<<(7-i));
		_delay_ms(100);
	}
}
void led_all_on_off(){
	for(int i=0;i<5;i++){
		PORTC=0x00;
		_delay_ms(100);
		PORTC=0xFF;
		_delay_ms(100);
	}
}
void led_alternating(){
	for(int i=0;i<5;i++){
		PORTC=0x55;
		_delay_ms(100);
		PORTC=~0x55;
		_delay_ms(100);
	}
}

ISR(PCINT2_vect){
	_delay_ms(100);
	cli();
	unsigned char in = PINK & 0x07;
	sei();
	if(!(in & 0x01)){
		led_shift();
	}
	else if(!(in & 0x02)){
		led_all_on_off();
	}
	else if(!(in & 0x04)){
		led_alternating();
	}
	
	PCIFR |= 0x04;
	
}


int main(void)
{
	cli();
	
	PCICR = 0x04;
	PCIFR = 0x04;
	PCMSK2 = 0x07;
	
	DDRC = 0xFF;
	PORTC = 0xFF;
	
	DDRK &= ~(0x07);
	
	sei();
    /* Replace with your application code */
    while (1) 
    {
		;
    }
}