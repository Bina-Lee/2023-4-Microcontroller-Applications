/*
 * MCP20231128_UART_buffering.c
 *
 * Created: 2023-11-28 오전 9:03:45
 * Author : Herb
 */ 

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

unsigned char case1[]={0x02,0x01,0x03,0x00};
unsigned char case2[]={0x02,0x02,0x03,0x00};
unsigned char case3[]={0x02,0x04,0x03,0x00};


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

//void command(uint8_t *buff,uint8_t len){
	//uint8_t tmp[MAX_BUFF_SIZE];
	//uint8_t ret=0;
	//memset(tmp,'\0',MAX_BUFF_SIZE);
	//memcpy((uint8_t*)tmp,(uint8_t*)buff,len);
	//if(strstr((char*)tmp,"LED_ON_OFF")!=NULL){
		//tx_str(_buff,com_size);
		//led_all_onoff();
	//}else if(strstr((char*)tmp,"LED_ALT")!=NULL){
		//tx_str(_buff,com_size);
		//led_alternating();
	//}else if(strstr((char*)tmp,"LED_SHIFT")!=NULL){
		//tx_str(_buff,com_size);
		//led_shift();
	//}
//}

void command(uint8_t *buff,uint8_t len){
	uint8_t tmp[MAX_BUFF_SIZE];
	uint8_t ret=0;
	memset(tmp,'\0',MAX_BUFF_SIZE);
	memcpy((uint8_t*)tmp,(uint8_t*)buff,len);
	if(strstr((char*)tmp,case1)!=NULL){
		tx_str(_buff,com_size);
		led_all_onoff();
		}else if(strstr((char*)tmp,case2)!=NULL){
		tx_str(_buff,com_size);
		led_alternating();
		}else if(strstr((char*)tmp,case3)!=NULL){
		tx_str(_buff,com_size);
		led_shift();
	}
}

ISR(USART1_RX_vect){
	cli();
	unsigned char _usart_rcv=0;
	_usart_rcv=UDR1;
	if(_usart_rcv=='\n' || com_size>=MAX_BUFF_SIZE){
		//if(_usart_rcv==0A || _usart_rcv==0D){
		sint_flag=1;
	}else{
		_buff[com_size++]=_usart_rcv;
	}
	sei();
}

int main(void)
{
    unsigned char rx_char=0;
    cli();
    DDRE &= ~(0x30);
    EICRB = 0xAA;
    EIMSK = 0x30;
    EIFR |= 0x30;
    
    UBRR1H = (unsigned char)(BAUD_9600>>8);
    UBRR1L = (unsigned char)BAUD_9600;
    UCSR1B = (1<<TXEN1) | (1<<RXEN1);
    
    UCSR1B |=(1<<RXCIE1);
    
    UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);
	
	DDRC=0xFF;
	PORTC=0x55;
    sei();
    
    //Printf("System Init\nUART Init\n");
    
    while (1)
    {
	    if(sint_flag){
			_buff[com_size]='\0';
			command(_buff,com_size);
			com_size=0;
			memset(_buff,'\0',128);
			sint_flag=0;
	    }
    }
}

//HEX로 변환 후 끝에 0A 붙이면 계행문자, 줄바꿈 처리 가능함