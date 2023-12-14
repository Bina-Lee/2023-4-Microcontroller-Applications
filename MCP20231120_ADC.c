/*
 * MCP20231120_ADC.c
 *
 * Created: 2023-11-20 오후 3:00:14
 * Author : Herb
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#define FOSC 16000000UL
#include <util/delay.h>

#include <avr/interrupt.h>

#define ON 1
#define OFF 0
int on_off=0;

int current_buzzer=0;

void play(unsigned int y){
	DDRB |= 0x02;
	TCCR1A |= 0x04;
	TCCR1B |= 0x1A;
	ICR1=y;
}
void stop(){
	TCCR1A=0x00;
	TCCR1B=0x00;
	TCNT1=0;
	ICR1=0;
	DDRB=~(0x02);
}


ISR(INT0_vect){
	cli();
	EIFR |=(0x01);
	sei();
	current++;
	if(current_buzzer>6)current_buzzer=6;
	_delay_ms(100);
}

ISR(INT1_vect){
	cli();
	EIFR |=(0x02);
	sei();
	current--;
	if(current_buzzer<0)current_buzzer=0;
	_delay_ms(100);
}

ISR(INT2_vect){
	cli();
	on_off^=1;
	EIFR |=(0x04);
	sei();
	_delay_ms(100);
}

int main(void)
{
	unsigned int note_freq[7]={
		2093,
		2349,
		2637,
		2794,
		3136,
		3520,
		3951
	};
	
	cli();
    /* Replace with your application code */
	//EICRA=0x30;
	//EICRB=0x0F;
	//EIMSK=0x34;
	//EIFR=0x34;
	
	DDRD=0x00;
	DDRB=0x80;
	
	PORTD=0x00;
	PORTB=0x00;
	
	EICRA=0x2A;
	
	EIMSK=0x07;
	
	EIFR |= 0x07;
	
	sei();
    while (1) 
    {
		if(on_off==ON){
			play(note_freq[current_buzzer]);
		}
		else{
			stop();
		}
    }
}

