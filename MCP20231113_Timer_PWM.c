/*
 * MCP20231113_Timer_PWM.c
 *
 * Created: 2023-11-13 오후 2:58:04
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
#if 0
ISR(TIMER0_OVF_vect){
	TCNT0=0xFF-0x4E;
	//to make 5ms
	
	TovVal++;
	
	if(TovVal==200){
		LED_BLINK();
		TovVal=0;
	}
	sei();
}


int main(void)
{
	unsigned char in;
	cli();
	DDRC=0xFF;
	PORTC=0xFF;
	
	TCCR0A=0x00;
	//NORMAL MODE
	
	TCCR0B |= (1<<CS02);
	//1024 prescale
	TCCR0B |=(1<<CS00);
	
	TCNT0=0xff-0x4E;
	//to generate OV at every 5ms

	TIMSK0 |= 1<<TOIE0;
	//timer0 overflow interrupt enable
	
	TIFR0 |=1<<TOV0;
	
	sei();
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

#endif

#if 01
ISR(TIMER0_OVF_vect){
	TCNT0=0xFF-0x4E;
	//to make 5ms
	
	TovVal++;
	
	if(TovVal==200){
		LED_BLINK();
		TovVal=0;
	}
	sei();
}


int main(void)
{
	unsigned char in;
	cli();
	DDRC=0xFF;
	PORTC=0xFF;
	
	TCCR0A=0x00;
	//NORMAL MODE
	
	TCCR0B |= (1<<CS02);
	//1024 prescale
	TCCR0B |=(1<<CS00);
	
	TCCR0A |= (1<<WGM01);
	
	OCR0A=0x4E;
	
	TCNT0=0xff-0x4E;
	//to generate OV at every 5ms

	TIMSK0 |= 1<<TOIE0;
	//timer0 overflow interrupt enable
	
	TIFR0 |=1<<TOV0;
	
	sei();
	
	/* Replace with your application code */
	while (1)
	{
	}
}
#endif