/*
 * MCP20231127_UART_loopback.c
 *
 * Created: 2023-11-27 오후 3:05:20
 * Author : Herb
 */ 
#if 0
#include <avr/io.h>

#define F_CPU 16000000UL
#include<util/delay.h>
#include<avr/interrupt.h>
#define BAUD_9600 103
#define MAX_TXBUF_SIZE 128
unsigned char tx_buf[MAX_TXBUF_SIZE];
int buf_len=0;

void tx_char(unsigned char txChar){
	while(!(UCSR1A & (1<<UDRE1)));
	UDR1 = txChar;
}

void tx_str(unsigned char *txStr, int len){
	int i;
	for(i=0;i<len;i++)tx_char(txStr[i]);
}

int main(void)
{
    /* Replace with your application code */
	unsigned char rx_char=0;
	cli();
	DDRE &= ~(0x30);
	EICRB = 0xAA;
	EIMSK = 0x30;
	EIFR |= 0x30;
	
	UBRR1H = (unsigned char)(BAUD_9600>>8);
	UBRR1L = (unsigned char)BAUD_9600;
	UCSR1B = (1<<TXEN1) | (1<<RXEN1);
	UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);
	sei();
	
    while (1) 
    {
		while(!(UCSR1A & (1<<RXC1)));
		rx_char=UDR1;
		
		while(!(UCSR1A & (1<<UDRE1)));
		UDR1 = rx_char;
    }
}

#endif



#if 0
#include <avr/io.h>

#define F_CPU 16000000UL
#include<util/delay.h>
#include<avr/interrupt.h>
#define BAUD_9600 103
#define MAX_TXBUF_SIZE 128
unsigned char tx_buf[MAX_TXBUF_SIZE];
int buf_len=0;

#include<stdlib.h>

void tx_char(unsigned char txChar){
	while(!(UCSR1A & (1<<UDRE1)));
	UDR1 = txChar;
}

void tx_str(unsigned char *txStr, int len){
	int i;
	for(i=0;i<len;i++)tx_char(txStr[i]);
}

ISR(INT4_vect){
	unsigned char *tmp;
	tmp=malloc(30);
	memset(tmp,'\0',30);
	tmp="INT4 PUSHED\n";
	buf_len=strlen(tmp);
	strncpy(tx_buf,tmp,buf_len);
	EIFR |= 0x10;
	sei();
}

ISR(INT5_vect){
	unsigned char *tmp;
	tmp=malloc(30);
	memset(tmp,'\0',30);
	tmp="INT5 PUSHED\n";
	buf_len=strlen(tmp);
	strncpy(tx_buf,tmp,buf_len);
	EIFR |= 0x10;
	sei();
}

int main(void)
{
    /* Replace with your application code */
	unsigned char rx_char=0;
	cli();
	DDRE &= ~(0x30);
	EICRB = 0xAA;
	EIMSK = 0x30;
	EIFR |= 0x30;
	
	UBRR1H = (unsigned char)(BAUD_9600>>8);
	UBRR1L = (unsigned char)BAUD_9600;
	UCSR1B = (1<<TXEN1) | (1<<RXEN1);
	UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);
	sei();
	
    while (1) 
    {
		if(buf_len>0){
			tx_str(tx_buf,buf_len);
			memset(tx_buf,'\0',MAX_TXBUF_SIZE);
			buf_len=0;
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
#define MAX_TXBUF_SIZE 128
unsigned char tx_buf[MAX_TXBUF_SIZE];
int buf_len=0;

#include<stdio.h>
#include<stdlib.h>

int cnt=0;

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

void tx_char(unsigned char txChar){
	while(!(UCSR1A & (1<<UDRE1)));
	UDR1 = txChar;
}

void tx_str(unsigned char *txStr, int len){
	int i;
	for(i=0;i<len;i++)tx_char(txStr[i]);
}

ISR(INT4_vect){
	unsigned char *tmp;
	tmp=malloc(30);
	memset(tmp,'\0',30);
	tmp="INT4 PUSHED\n";
	buf_len=strlen(tmp);
	strncpy(tx_buf,tmp,buf_len);
	EIFR |= 0x10;
	sei();
}

ISR(INT5_vect){
	//unsigned char *tmp;
	//tmp=malloc(30);
	//memset(tmp,'\0',30);
	//tmp="INT5 PUSHED\n";
	//buf_len=strlen(tmp);
	//strncpy(tx_buf,tmp,buf_len);
	
	cnt++;
	Printf("cnt %d",cnt);
	
	EIFR |= 0x10;
	sei();
}

ISR(USART1_RX_vect){
	unsigned char _usart_rcv=0;
	_usart_rcv = UDR1;
	tx_char(_usart_rcv);
	sei();
}

int main(void)
{
	/* Replace with your application code */
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
	sei();
	
	Printf("System Init\nUART Init\n");
	
	while (1)
	{
		if(buf_len>0){
			tx_str(tx_buf,buf_len);
			memset(tx_buf,'\0',MAX_TXBUF_SIZE);
			buf_len=0;
		}
	}
}

#endif