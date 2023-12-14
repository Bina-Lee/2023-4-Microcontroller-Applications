/*
 * MCP20231121_UART.c
 *
 * Created: 2023-11-21 오전 9:06:49
 * Author : Herb
 */ 

#include <avr/io.h>

#define F_CPU 16000000UL
#include<util/delay.h>
#include<avr/interrupt.h>
#define BAUD_9600 103

unsigned char send_char =0;

ISR(INT4_vect){
	send_char='A';
	EIFR |= 0x10;
	sei();
}

ISR(INT5_vect){
	send_char='B';
	EIFR |= 0x20;
	sei();
}

int main(void)
{
    /* Replace with your application code */
	cli();
	DDRE &= ~(0x30);
	EICRB=0xAA;
	EIMSK=0x30;
	EIFR |=0x30;
	
	UBRR1H=(unsigned char)(BAUD_9600 >> 8);
	UBRR1L=(unsigned char)BAUD_9600;
	UCSR1B=(1<<TXEN1) | (1<<RXEN1);
	UCSR1C=(1<<UCSZ11) | (1<<UCSZ10);
	
	sei();
	
	send_char='B';
	
    while (1) 
    {
		if(send_char){
			while(!(UCSR1A & 1<<UDRE1));
			UDR1=send_char;
			send_char=0;
		}
    }
}

