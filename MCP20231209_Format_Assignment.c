#if 0

/*
 * MCP20231209_Format_Assignment.c
 *
 * Created: 2023-12-09 오후 3:40:59
 * Author : Herb
 */ 

#include <avr/io.h>

#include "mc_ws2812.h"

#define F_CPU 16000000UL
#include<util/delay.h>
#include<avr/interrupt.h>
#define BAUD_9600 103
#define MAX_BUFF_SIZE 128
unsigned char _buff[MAX_BUFF_SIZE];

unsigned char sint_flag=0;
unsigned char com_size;

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define NEOPIXEL_ON 0x01
#define NEOPIXEL_OFF 0x02
#define NEOPIXEL_BRIGHTNESS 0x04

#define NEOPIXEL_R 0x06
#define NEOPIXEL_G 0x08
#define NEOPIXEL_B 0x10

#define STX 0x02
#define ETX 0x03

#define LED_ONOFF 0x01
#define LED_ALT 0x02
#define LED_SHIFT 0x04

#define LED_COUNT 16

rgbw_color tempColors[LED_COUNT];

char tempBool[16][4]={0,};

unsigned char brightness=16;

void set_led(){
	for(int i=0;i<16;i++){
		tempColors[i].green=tempBool[i][0]*brightness;
		tempColors[i].red  =tempBool[i][1]*brightness;
		tempColors[i].blue =tempBool[i][2]*brightness;
		tempColors[i].white=tempBool[i][3]*brightness;
	}
}

void led_all_on(){
	for(int i=0;i<16;i++){
		tempBool[i][0]=0;
		tempBool[i][1]=0;
		tempBool[i][2]=0;
		tempBool[i][3]=1;
	}
	set_led();
	ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
}
void led_all_off(){
	for(int i=0;i<16;i++){
		tempBool[i][0]=0;
		tempBool[i][1]=0;
		tempBool[i][2]=0;
		tempBool[i][3]=0;
	}
	set_led();
	ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
}

//void led_brightness(){
	//for(int i=0;i<16;i++){
		//if(tempColors[i].blue !=0)tempColors[i].blue =brightness;
		//if(tempColors[i].green!=0)tempColors[i].green=brightness;
		//if(tempColors[i].red  !=0)tempColors[i].red  =brightness;
		//if(tempColors[i].white!=0)tempColors[i].white=brightness;
	//}
//}


void led_green_n(int n){
	tempBool[n][0]=1;
	tempBool[n][1]=0;
	tempBool[n][2]=0;
	tempBool[n][3]=0;
	set_led();
	ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
}
void led_red_n(int n){
	tempBool[n][0]=0;
	tempBool[n][1]=1;
	tempBool[n][2]=0;
	tempBool[n][3]=0;
	set_led();
	ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
}
void led_blue_n(int n){
	tempBool[n][0]=0;
	tempBool[n][1]=0;
	tempBool[n][2]=1;
	tempBool[n][3]=0;
	set_led();
	ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
}

void led_shift(unsigned int t){
	for(int j=0;j<t;j++){
		for(int i=0;i<7;i++){
			PORTC=~(0x01<<i);
			_delay_ms(100);
		}
		for(int i=0;i<7;i++){
			PORTC=~(0x01<<(7-i));
			_delay_ms(100);
		}
	}
}
void led_all_onoff(unsigned int t){
	for(int i=0;i<t;i++){
		PORTC=0x00;
		_delay_ms(100);
		PORTC=0xFF;
		_delay_ms(100);
	}
}
void led_alternating(unsigned int t){
	for(int i=0;i<t;i++){
		PORTC=0x55;
		_delay_ms(100);
		PORTC=~0x55;
		_delay_ms(100);
	}
}

void tx_char(unsigned char txChar){
	while(!(UCSR1A & (1<<UDRE1)));
	UDR1 = txChar;
}

void tx_str(unsigned char *txStr, int len){
	int i;
	for(i=0;i<len;i++)tx_char(txStr[i]);
}

void Printf(char *fmt, ... ){
	va_list arg_ptr;
	uint8_t i,len;
	char sText[128];
	
	for(i=0;i<128;i++){
		sText[i]=0;
	}
	va_start(arg_ptr, fmt);
	vsprintf(sText,fmt,arg_ptr);
	va_end(arg_ptr);
	
	len=strlen(sText);
	for(i=0;i<len;i++){
		tx_char(sText[i]);
	}
}


int decode =0;
//0: normal
//1: data reception
//2: data(brightness)
//3: data(red)
//4: DATA(green)
//5: data(blue)

//3: termination state

ISR(USART1_RX_vect){
	cli();
	unsigned char _usart_rcv=0;
	_usart_rcv=UDR1;
	if(decode==0 && _usart_rcv==STX){
		decode=1;
		
	}else if(decode==1 && _usart_rcv==NEOPIXEL_ON){
		led_all_on();
	}else if(decode==1 && _usart_rcv==NEOPIXEL_OFF){
		led_all_off();
	}else if(decode==1 && _usart_rcv==NEOPIXEL_BRIGHTNESS){
		decode=2;
	}else if(decode==2){
		brightness=_usart_rcv;
		decode=1;
	}else if(decode==1 && _usart_rcv==NEOPIXEL_R){
		decode=3;
	}else if(decode==3){
		led_red_n(_usart_rcv);
		decode=1;
	}else if(decode==1 && _usart_rcv==NEOPIXEL_G){
		decode=4;
	}else if(decode==4){
		led_green_n(_usart_rcv);
		decode=1;
	}else if(decode==1 && _usart_rcv==NEOPIXEL_B){
		decode=5;
	}else if(decode==5){
		led_blue_n(_usart_rcv);
		decode=1;
		
	}else if(decode==1 && _usart_rcv==ETX){
		sint_flag=1;
		decode=0;
	}else{
		Printf("RXerror\n");
	}
	//}else if(decode==1){
		//_command=_usart_rcv;
		//decode=2;
	//}else if(decode==2){
		//_command_data=_usart_rcv;
		//decode=3;
	//}else if(decode==3){
		//if(_usart_rcv==ETX){
			//sint_flag=1;
		//}else{
			//Printf("RXerror\n");
		//}
		//decode=0;
	//}
	sei();
}

void hex_command(uint8_t cd,uint8_t cd_t){
	if(cd==LED_ONOFF){
		led_all_onoff(cd_t);
		}else if(cd==LED_ALT){
		led_alternating(cd_t);
		}else if(cd==LED_SHIFT){
		led_shift(cd_t);
		}else{
		Printf("Invalid Command\n");
	}
}

int main(void)
{
	unsigned char rx_char=0;
	cli();
	DDRC=0xFF;
	DDRE &= ~(0x30);
	EICRB = 0xAA;
	EIMSK = 0x30;
	EIFR |= 0x30;
	
	UBRR1H = (unsigned char)(BAUD_9600>>8);
	UBRR1L = (unsigned char)BAUD_9600;
	UCSR1A=0x00;
	UCSR1B = (1<<TXEN1) | (1<<RXEN1);
	UCSR1B |=(1<<RXCIE1);
	
	UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);
	
	sei();
	
	PORTC=0xFF;
	
	DDRB |= 0x01<<6;
	
	led_all_on();
	for(int i=0;i<16;i++){
		led_red_n(i);
		_delay_ms(50);
	}
	for(int i=0;i<16;i++){
		led_green_n(i);
		_delay_ms(50);
	}
	for(int i=0;i<16;i++){
		led_blue_n(i);
		_delay_ms(50);
	}
	//led_all_on();
	
	for(int i=0;i<5;i++){
		brightness=0;
		set_led();
		ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
		_delay_ms(500);
		brightness=16;
		set_led();
		ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
		_delay_ms(500);
	}
	brightness=16;
	led_all_on();
	
	//Printf("System Init\nUART Init\n");
	
	while (1)
	{
		;
		//if(sint_flag){
			//hex_command(_command,_command_data);
			//sint_flag=0;
			//_command=0;
			//_command_data=0;
		//}
	}
}

#endif

#if 01

/*
 * MCP20231209_Format_Assignment.c
 *
 * Created: 2023-12-09 오후 3:40:59
 * Author : Herb
 */ 

#include <avr/io.h>

#include "mc_ws2812.h"

#define F_CPU 16000000UL
#include<util/delay.h>
#include<avr/interrupt.h>
#define BAUD_9600 103
#define MAX_BUFF_SIZE 128
unsigned char _buff[MAX_BUFF_SIZE];

unsigned char sint_flag=0;
unsigned char com_size;

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define NEOPIXEL_ON 0x01
#define NEOPIXEL_OFF 0x02
#define NEOPIXEL_BRIGHTNESS 0x04

#define NEOPIXEL_R 0x06
#define NEOPIXEL_G 0x08
#define NEOPIXEL_B 0x10

#define STX 0x02
#define ETX 0x03

#define LED_ONOFF 0x01
#define LED_ALT 0x02
#define LED_SHIFT 0x04

#define LED_COUNT 16

rgbw_color tempColors[LED_COUNT];

char tempBool[16][4]={0,};

unsigned char brightness=16;


uint8_t _command=0;
uint8_t _command_data=0;

void set_led(){
	for(int i=0;i<16;i++){
		tempColors[i].green=tempBool[i][0]*brightness;
		tempColors[i].red  =tempBool[i][1]*brightness;
		tempColors[i].blue =tempBool[i][2]*brightness;
		tempColors[i].white=tempBool[i][3]*brightness;
	}
}

void led_all_on(){
	for(int i=0;i<16;i++){
		tempBool[i][0]=0;
		tempBool[i][1]=0;
		tempBool[i][2]=0;
		tempBool[i][3]=1;
	}
	set_led();
	ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
}
void led_all_off(){
	for(int i=0;i<16;i++){
		tempBool[i][0]=0;
		tempBool[i][1]=0;
		tempBool[i][2]=0;
		tempBool[i][3]=0;
	}
	set_led();
	ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
}

//void led_brightness(){
	//for(int i=0;i<16;i++){
		//if(tempColors[i].blue !=0)tempColors[i].blue =brightness;
		//if(tempColors[i].green!=0)tempColors[i].green=brightness;
		//if(tempColors[i].red  !=0)tempColors[i].red  =brightness;
		//if(tempColors[i].white!=0)tempColors[i].white=brightness;
	//}
//}


void led_green_n(int n){
	tempBool[n][0]=1;
	tempBool[n][1]=0;
	tempBool[n][2]=0;
	tempBool[n][3]=0;
	set_led();
	ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
}
void led_red_n(int n){
	tempBool[n][0]=0;
	tempBool[n][1]=1;
	tempBool[n][2]=0;
	tempBool[n][3]=0;
	set_led();
	ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
}
void led_blue_n(int n){
	tempBool[n][0]=0;
	tempBool[n][1]=0;
	tempBool[n][2]=1;
	tempBool[n][3]=0;
	set_led();
	ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
}

void led_shift(unsigned int t){
	for(int j=0;j<t;j++){
		for(int i=0;i<7;i++){
			PORTC=~(0x01<<i);
			_delay_ms(100);
		}
		for(int i=0;i<7;i++){
			PORTC=~(0x01<<(7-i));
			_delay_ms(100);
		}
	}
}
void led_all_onoff(unsigned int t){
	for(int i=0;i<t;i++){
		PORTC=0x00;
		_delay_ms(100);
		PORTC=0xFF;
		_delay_ms(100);
	}
}
void led_alternating(unsigned int t){
	for(int i=0;i<t;i++){
		PORTC=0x55;
		_delay_ms(100);
		PORTC=~0x55;
		_delay_ms(100);
	}
}

void tx_char(unsigned char txChar){
	while(!(UCSR1A & (1<<UDRE1)));
	UDR1 = txChar;
}

void tx_str(unsigned char *txStr, int len){
	int i;
	for(i=0;i<len;i++)tx_char(txStr[i]);
}

void Printf(char *fmt, ... ){
	va_list arg_ptr;
	uint8_t i,len;
	char sText[128];
	
	for(i=0;i<128;i++){
		sText[i]=0;
	}
	va_start(arg_ptr, fmt);
	vsprintf(sText,fmt,arg_ptr);
	va_end(arg_ptr);
	
	len=strlen(sText);
	for(i=0;i<len;i++){
		tx_char(sText[i]);
	}
}


int decode =0;
//0: normal
//1: data reception
//2: data(brightness)
//3: data(red)
//4: DATA(green)
//5: data(blue)


//10: termination state
//3: termination state

ISR(USART1_RX_vect){
	cli();
	unsigned char _usart_rcv=0;
	_usart_rcv=UDR1;
	if(decode==0 && _usart_rcv==STX){
		decode=1;
		
	}else if(decode==1){
		_command=_usart_rcv;
		if(_command==0x01 || _command==0x02){
			decode=3;
		}else{
			decode=2;
		}
	}
	
	else if(decode==2){
		_command_data=_usart_rcv;
		decode=3;
	
		
	//}else if(decode==1 && _usart_rcv==NEOPIXEL_ON){
		//led_all_on();
		//decode=10;
	//}else if(decode==1 && _usart_rcv==NEOPIXEL_OFF){
		//led_all_off();
		//decode=10;
	//}else if(decode==1 && _usart_rcv==NEOPIXEL_BRIGHTNESS){
		//decode=2;
	//}else if(decode==2){
		//brightness=_usart_rcv;
		//decode=10;
	//}else if(decode==1 && _usart_rcv==NEOPIXEL_R){
		//decode=3;
	//}else if(decode==3){
		//led_red_n(_usart_rcv);
		//decode=10;
	//}else if(decode==1 && _usart_rcv==NEOPIXEL_G){
		//decode=4;
	//}else if(decode==4){
		//led_green_n(_usart_rcv);
		//decode=10;
	//}else if(decode==1 && _usart_rcv==NEOPIXEL_B){
		//decode=5;
	//}else if(decode==5){
		//led_blue_n(_usart_rcv);
		//decode=10;
		
	//}else if(decode==1 && _usart_rcv==ETX){
		//sint_flag=1;
		//decode=0;
	//}else{
		//Printf("RXerror\n");
	}else if(decode==3){
		if(_usart_rcv==ETX){
			sint_flag=1;
		}else{
			Printf("RXerror\n");
		}
		decode=0;
	}
	//}else if(decode==1){
		//_command=_usart_rcv;
		//decode=2;
	//}else if(decode==2){
		//_command_data=_usart_rcv;
		//decode=3;
	//}else if(decode==3){
		//if(_usart_rcv==ETX){
			//sint_flag=1;
		//}else{
			//Printf("RXerror\n");
		//}
		//decode=0;
	//}
	sei();
}

void hex_command(uint8_t cd,uint8_t cd_t){
	if(cd==NEOPIXEL_ON)led_all_on();
	else if(cd==NEOPIXEL_OFF)led_all_off();
	else if(cd==NEOPIXEL_BRIGHTNESS){
		brightness=cd_t;
		set_led();
	}
	else if(cd==NEOPIXEL_R)led_red_n(cd_t);
	else if(cd==NEOPIXEL_G)led_green_n(cd_t);
	else if(cd==NEOPIXEL_B)led_blue_n(cd_t);
	//if(cd==LED_ONOFF){
		//led_all_onoff(cd_t);
	//}else if(cd==LED_ALT){
		//led_alternating(cd_t);
	//}else if(cd==LED_SHIFT){
		//led_shift(cd_t);
	else{
		Printf("Invalid Command\n");
	}
}

int main(void)
{
	unsigned char rx_char=0;
	cli();
	DDRC=0xFF;
	DDRE &= ~(0x30);
	EICRB = 0xAA;
	EIMSK = 0x30;
	EIFR |= 0x30;
	
	UBRR1H = (unsigned char)(BAUD_9600>>8);
	UBRR1L = (unsigned char)BAUD_9600;
	UCSR1A=0x00;
	UCSR1B = (1<<TXEN1) | (1<<RXEN1);
	UCSR1B |=(1<<RXCIE1);
	
	UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);
	
	sei();
	
	PORTC=0xFF;
	
	DDRB |= 0x01<<6;
	
	led_all_on();
	for(int i=0;i<16;i++){
		led_red_n(i);
		_delay_ms(50);
	}
	for(int i=0;i<16;i++){
		led_green_n(i);
		_delay_ms(50);
	}
	for(int i=0;i<16;i++){
		led_blue_n(i);
		_delay_ms(50);
	}
	//led_all_on();
	
	for(int i=0;i<5;i++){
		brightness=0;
		set_led();
		ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
		_delay_ms(500);
		brightness=16;
		set_led();
		ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
		_delay_ms(500);
	}
	brightness=16;
	led_all_on();
	
	//Printf("System Init\nUART Init\n");
	
	while (1)
	{
		;
		if(sint_flag){
			hex_command(_command,_command_data);
			sint_flag=0;
			_command=0;
			_command_data=0;
		}
	}
}

#endif