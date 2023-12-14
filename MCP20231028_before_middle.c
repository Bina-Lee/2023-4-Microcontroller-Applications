/*
 * MCP20231028_before_middle.c
 *
 * Created: 2023-10-28 오후 2:29:37
 * Author : Herb
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#define FOSC 16000000UL
#include <util/delay.h>

#include <avr/interrupt.h>

void custom_delay(unsigned int t){
	for(int i=0;i<t;i++){
		_delay_ms(1);
	}
}

unsigned int led_data1=0x00000001;
unsigned int led_data2=0x00000100;
unsigned int led_data =0x00000001;
unsigned int led_data_ifinverse;
unsigned char inverse=1;

int delay_time=0;

unsigned char prev_in=0x00;

void change_speed_fast(){
	delay_time=delay_time+1;
	if(delay_time>8)delay_time=8;
}
void change_speed_slow(){
	delay_time=delay_time-1;
	if(delay_time<0)delay_time=0;
}

void set_led(){
	led_data= led_data1 | led_data2;
	led_data_ifinverse= led_data;
	if(inverse)led_data_ifinverse=~led_data_ifinverse;
	PORTC=led_data_ifinverse & 0x1F;
	PORTA=(led_data_ifinverse>>1) & 0xFF;
}

#if 0
void set_led_change_speed(){
	
}
#endif
void display_speed_fast(){
	led_data=(0x01<<delay_time);
	PORTC=led_data & 0x1F;
	PORTA=(led_data>>1) & 0xFF;
}
void display_speed_slow(){
	led_data=~(0x01<<delay_time);
	PORTC=led_data & 0x1F;
	PORTA=(led_data>>1) & 0xFF;
}

ISR(INT2_vect){
	cli();
	EIFR |=(0x01<<2);
	sei();
	
	inverse^=1;
}

ISR(PCINT2_vect){
	cli();
	unsigned char in=PINK & 0x06;
	sei();
	
	if(!(prev_in & 0x02) && (prev_in & 0x04)){
		if(!(in & 0x02) && !(in & 0x04)){
			display_speed_slow();
			prev_in=in;
			while((prev_in&0x06) == (in&0x06)){
				in=PINK & 0x06;
			}
			change_speed_slow();
		}
	}
	if((prev_in & 0x02) && !(prev_in & 0x04)){
		if(!(in & 0x02) && !(in & 0x04)){
			display_speed_fast();
			prev_in=in;
			while((prev_in&0x06) == (in&0x06)){
				in=PINK & 0x06;
			}
			change_speed_fast();
		}
	}
	
	//if(!(in & 0x02)){
		//display_speed_slow();
		//custom_delay(1000);
		//change_speed_slow();
	//}
	//else if(!(in & 0x04)){
		//display_speed_fast();
		//custom_delay(1000);
		//change_speed_fast();
	//}
	
	prev_in=in;
	
	PCIFR |= 0x04;
}

int main(void)
{
	cli();
	//////////////////////////////////
	EICRA=0x30;
	
	EIMSK=0x04;
	EIFR =0x04;
	//////////////////////////////////
	PCICR=0x04;
	PCIFR=0x04;
	PCMSK2=0x06;
	//////////////////////////////////
	DDRC=0xFF; //0~4
	DDRA=0xFF; //4~7
	
	DDRK &= ~(0x06);
	
	set_led();
	
	DDRD &= ~(0x04);
	
	sei();
	
    /* Replace with your application code */
    while (1) 
    {
		for(int i=0;i<8;i++){
			led_data1=0x01<<i;
			led_data2=0x01<<(8-i);	
			set_led();
			custom_delay((delay_time+1)*100);
		}
		for(int i=8;i>0;i--){
			led_data1=0x01<<i;
			led_data2=0x01<<(8-i);
			set_led();
			custom_delay((delay_time+1)*100);
		}
    }
}

