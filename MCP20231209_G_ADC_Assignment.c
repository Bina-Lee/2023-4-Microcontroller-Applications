/*
 * MCP20231209_G_ADC_Assignment.c
 *
 * Created: 2023-12-09 오후 10:38:53
 * Author : Herb
 */ 

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

#include "mc_ws2812.h"


#define LED_COUNT 16

rgbw_color tempColors[LED_COUNT];


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

unsigned int GetADCData(unsigned char aIn){
	volatile unsigned int result;
	ADMUX=aIn;
	//channel selection(0~7), 8~15sms 0x100000를 더해야함
	ADMUX |= (1<<REFS0);
	//avcc옵션(외부공급 5V);
	ADCSRA = (1<<ADEN)|(1<<ADPS2) | (1<<ADPS1)|(1<<ADPS0);
	ADCSRA |= (1<<ADSC);
	while(!(ADCSRA & (1<<ADIF)));
	_delay_us(300);
	result=ADCL +(ADCH<<8);
	ADCSRA=0x00;
	return result;
}

void disp_led(unsigned int adc){
	PORTC=0xFF;
	for(int i=0;i<=(adc/128);i++) PORTC &= ~(0x01<<i);
}
void disp_NEO(unsigned int adc){
	for(int i=0;i<16;i++){
		if(((int)adc/64 - i)>=0)tempColors[i]=(rgbw_color){0,0,16,0};
		else					tempColors[i]=(rgbw_color){0,0,0,0};
		
		ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
	}
}

int main(void)
{
	/* Replace with your application code */
	
	UBRR1H=(unsigned char)(BAUD_9600>>8);
	UBRR1L=(unsigned char)BAUD_9600;
	UCSR1B=(1<<TXEN1) | (1<<RXEN1);
	UCSR1C=(1<<UCSZ11) | (1<<UCSZ10);
	
	DDRC=0xFF;
	PORTC=0xFF;
	
	DDRB |= 0x01<<6;
	
	
	while (1)
	{
		unsigned int adc_value=GetADCData(0);
		_delay_ms(10);
		disp_led(adc_value);
		disp_NEO(adc_value);
		Printf("ADC: %d\n",adc_value);
	}
}