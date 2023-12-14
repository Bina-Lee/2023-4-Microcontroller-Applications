/*
 * MCP20231030_middle.c
 *
 * Created: 2023-10-30 오후 5:59:05
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

unsigned char arr[4]={
	~0x81,
	~0x18,
	~0x42,
	~0x24
};

unsigned char set[3]={
	0xFF,
	0xF0,
	0x0F	
};
unsigned int set_num=0;

unsigned char led_data =0x01;
unsigned char led_data_ifinverse;
unsigned char inverse=0;

int delay_time=0;

unsigned char prev_in=0xFF;

int check1=0;
int check2=0;
int check3=0;

void set_led(){
	//led_data=led_data | set[set_num];
	led_data_ifinverse=led_data;
	if(inverse==0)led_data_ifinverse=~led_data_ifinverse;
	
	if(check2==2)led_data_ifinverse=~led_data_ifinverse;
	if(check3==2)led_data_ifinverse=~led_data_ifinverse;
	
	led_data_ifinverse=led_data_ifinverse & set[set_num];
	led_data_ifinverse=~led_data_ifinverse;
	
	PORTA=led_data_ifinverse;
	PORTD=led_data_ifinverse>>4;
}

ISR(PCINT2_vect){
	cli();
	unsigned char in=PINK & 0x07;
	sei();
	
	if(!(in & 0x01) && (prev_in & 0x01)){
		inverse=inverse+1;
		inverse=inverse%3;
		//if(inverse==2)inverse=0;
	}
	if((in & 0x02) && !(prev_in & 0x02)){
		set_num=2;
	}
	if((in & 0x04) && !(prev_in & 0x04)){
		set_num=1;
	}
	if(!(prev_in & 0x02) && (prev_in & 0x04)){
		if(!(in & 0x02) && !(in & 0x04)){
			set_num=0;
		}
	}
	if(inverse==2){
		unsigned char prev=in;
		while(check1!=2){
		//while(!(!(prev_in & 0x01) && (in & 0x01))){
			in=PINK & 0x07;
			if(check2==2){
				set_num=2;
				set_led();
				check2=0;
			}
			if(check3==2){
				set_num=1;
				set_led();
				check3=0;
			}
			if(( (!(in & 0x01)) && (prev & 0x01) )){
				check1=1;
			}
			if(( (in & 0x01) && (!(prev & 0x01)) )){
				if(check1==1){
					inverse=0;
					//check=0;
					check1=2;
				}
				
			}
			if(( (!(in & 0x02)) && (prev & 0x02) )){
				check2=1;
			}
			if(( (in & 0x02) && (!(prev & 0x02)) )){
				if(check2==1){
					check2=2;
				}
			}
			if(( (!(in & 0x04)) && (prev & 0x04) )){
				check3=1;
			}
			if(( (in & 0x04) && (!(prev & 0x04)) )){
				if(check3==1){
					check3=2;
				}
			}
			////if((in & 0x02) && !(prev & 0x02)){
			//if(((in & 0x02)) && (!(prev & 0x02))){
				//set_num=2;
				//set_led();
			//}
			////if((in & 0x04) && !(prev & 0x04)){
			//if(((in & 0x04)) && (!(prev & 0x04))){
				//set_num=1;
				//set_led();
			//}
			//_delay_ms(50);
			//set_led();
			prev=in;
		}
		//inverse=0;
	}
	
	//if((in & 0x02) && !(prev_in & 0x02)){
		//set_num=2;
	//}
	//if((in & 0x04) && !(prev_in & 0x04)){
		//set_num=1;
	//}
	//if(!(prev_in & 0x02) && (prev_in & 0x04)){
		//if(!(in & 0x02) && !(in & 0x04)){
			//while(!(in & 0x02))in=PINK & 0x07;
			//set_num=0;
		//}
	//}
	//
	prev_in=in;
	
	PCIFR |= 0x04;
}

int main(void)
{
	cli();
	PCICR=0x04;
	PCIFR=0x04;
	PCMSK2=0x07;
	//////////////////////////////////
	DDRD=0xFF; //0~4
	DDRA=0xFF; //3~7
	
	DDRK &= ~(0x07);
	
	set_led();
	
	sei();
	
	/* Replace with your application code */
	while (1)
	{
		for(int i=0;i<4;i++){
			led_data=arr[i];
			set_led();
			custom_delay(250);
		}
		for(int i=0;i<3;i++){
			led_data=arr[3-i];
			set_led();
			custom_delay(250);
		}
	}
}