/*
 * MCP20230926.c
 *
 * Created: 2023-09-26 오전 10:08:36
 * Author : Herb
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#define FOSC 16000000UL
#include <util/delay.h>

int main(void)
{
    DDRC=0x01;
	while (1) 
    {
		PORTC=0x01;
		_delay_ms(300);
		PORTC=0;
		_delay_ms(300);
    }
}

