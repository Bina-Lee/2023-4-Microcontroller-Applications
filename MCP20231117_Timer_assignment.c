/*
 * MCP20231117_Timer_assignment.c
 *
 * Created: 2023-11-17 오후 6:15:35
 * Author : Herb
 */ 


#include <avr/io.h>

#define F_CPU 16000000UL
#include <util/delay.h>
#include<avr/interrupt.h>
unsigned char TovVal=0;

void LED_BLINK(){
	PORTC=0x00;
	_delay_ms(300);
	PORTC=0xFF;
}
#if 01
ISR(TIMER1_OVF_vect){
	TCNT1=0xFF-0x3D08;
	//to make 5ms
	
	//TovVal++;
	//
	//if(TovVal==2){
		LED_BLINK();
		//TovVal=0;
	//}
	sei();
}


int main(void)
{
	//unsigned char in;
	cli();
	DDRC=0xFF;
	PORTC=0xFF;
	
	TCCR1A=0x00;
	//NORMAL MODE
	
	TCCR1B |= (1<<CS12);
	//1024 prescale
	TCCR1B |=(1<<CS10);
	
	TCNT1=0xffff-0x3D08;
	//to generate OV at every 5ms

	TIMSK1 |= 1<<TOIE1;
	//timer0 overflow interrupt enable
	
	TIFR1 |=1<<TOV1;
	
	sei();
	
	/* Replace with your application code */
	while (1)
	{
	}
}

#endif