/*
 * MCP20231017.c
 *
 * Created: 2023-10-17 오전 9:08:08
 * Author : Herb
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#define FOSC 16000000UL
#include <util/delay.h>

#include <avr/interrupt.h>


#if 01
void custom_delay(unsigned int _ms){
	for(int i=0;i<_ms;i++){
		_delay_ms(1);
	}
}

void led_shift(){
	
	for(int i=0;i<7;i++){
		PORTC=~(0x01<<i);
		_delay_ms(100);
	}
	for(int i=0;i<7;i++){
		PORTC=~(0x01<<(7-i));
		_delay_ms(100);
	}
	PORTC=~(0x01);
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

ISR(INT2_vect){
	cli();	//clear global interrupt
	EIFR |= (0x01<<2);
	sei();	//set enable global interrupt
	
	led_shift();
	
}
ISR(INT4_vect){
	cli();
	EIFR |= (0x01<<4);
	sei();
	led_all_onoff(3,1000);
}
ISR(INT5_vect){
	cli();
	EIFR |= (0x01<<5);
	sei();
	led_alternation_onoff(0x55,3,1000);
}
#endif

#if 0
int main(){
	DDRC=0xFF;
	DDRL=0x00;
	
	unsigned char in;
	
	while(1){
		in=PINL;
		if((~in & (0x01))){
			PORTC=0x55;
			_delay_ms(500);
		}
		for(int i=0;i<8;i++){
			PORTC=~(0x01<<i);
			_delay_ms(100);
		}
	}
}
#endif

#if 01
int main(){
	cli();
	
	EICRA=0x30;	//init2 rising
	EICRB=0x0F;	//init4,5 rising
	EIMSK=0x34;	//mask
	EIFR=0x34;
	
	DDRC=0xFF;
	PORTC=0xFF;
	
	DDRD &= ~(0x04);
	DDRE &= ~(0x30);
	
	sei();
	
	while(1);
}
#endif

/*
void custom_delay(unsigned int _ms){
	for(int i=0;i<_ms;i++){
		_delay_ms(1);
	}
}

void led_shift(){
	for(int i=0;i<7;i++){
		DDRC=~(0x01<<i);
		custom_delay(100);
	}
	for(int i=0;i<7;i++){
		DDRC=~(0x01<<(7-i));
		custom_delay(100);
	}
	DDRC=~(0x01);
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
*/

/*
int main(void)
{
	DDRC= (0xFF);
	DDRL=~(0x07);
	
	unsigned char in;
	unsigned char prev_in;
	
	PORTC=0xFF;
	*/
    /* Replace with your application code */
    /*
	while (1) 
    {
		in=(PINL & 0x07);
		//PORTC=in | ~0x07;
		//if((in & 0x01)){
			if((in & 0x01) && (~prev_in & 0x01)){
				for(int i=0;i<3;i++){
					led_shift();
				}
				custom_delay(100);
			}
		//}
		//if((in & 0x02)){
			if((in & 0x02) && (~prev_in & 0x02)){
				led_all_onoff(3,1000);
				custom_delay(100);
			}
		//}
		//if((in & 0x04)){
			if((in & 0x04) && (~prev_in & 0x04)){
				led_alternation_onoff(0x55,3,1000);
				custom_delay(100);
			}
		//}
		prev_in=in;
    }
}

*/