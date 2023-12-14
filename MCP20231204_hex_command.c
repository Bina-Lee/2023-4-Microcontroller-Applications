/*
 * MCP20231204_hex_command.c
 *
 * Created: 2023-12-04 오후 3:10:33
 * Author : Herb
 */ 


#if 0


#include <avr/io.h>

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

#define LED_ONOFF 0x01
#define LED_ALT 0x02
#define LED_SHIFT 0x04
#define STX 0x02
#define ETX 0x03

int decode =0;
//0: normal
//1: data reception
//2: termination state
uint8_t _command=0;

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
void led_all_onoff(){
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

ISR(USART1_RX_vect){
	cli();
	unsigned char _usart_rcv=0;
	_usart_rcv=UDR1;
	if(decode==0 && _usart_rcv==STX){
		decode=1;
	}else if(decode==1){
		_command=_usart_rcv;
		decode=2;
	}else if(decode==2){
		if(_usart_rcv==ETX){
			sint_flag=1;
		}else{
			Printf("RXerror\n");
		}
		decode=0;
	}
	sei();
}

void hex_command(uint8_t cd){
	if(cd==LED_ONOFF){
		led_all_onoff();
	}else if(cd==LED_ALT){
		led_alternating();
	}else if(cd==LED_SHIFT){
		led_shift();
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
	
	//Printf("System Init\nUART Init\n");
	
	while (1)
	{
		if(sint_flag){
			hex_command(_command);
			sint_flag=0;
			_command=0;
		}
	}
}

#endif

#if 01

#include <avr/io.h>

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

#define LED_ONOFF 0x01
#define LED_ALT 0x02
#define LED_SHIFT 0x04
#define STX 0x02
#define ETX 0x03

int decode =0;
//0: normal
//1: data reception
//2: data(count)
//3: termination state
uint8_t _command=0;
uint8_t _command_data=0;

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

ISR(USART1_RX_vect){
	cli();
	unsigned char _usart_rcv=0;
	_usart_rcv=UDR1;
	if(decode==0 && _usart_rcv==STX){
		decode=1;
		}else if(decode==1){
			_command=_usart_rcv;
			decode=2;
		}else if(decode==2){
			_command_data=_usart_rcv;
			decode=3;
		}else if(decode==3){
		if(_usart_rcv==ETX){
			sint_flag=1;
			}else{
			Printf("RXerror\n");
		}
		decode=0;
	}
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
	
	//Printf("System Init\nUART Init\n");
	
	while (1)
	{
		if(sint_flag){
			hex_command(_command,_command_data);
			sint_flag=0;
			_command=0;
			_command_data=0;
		}
	}
}

#endif