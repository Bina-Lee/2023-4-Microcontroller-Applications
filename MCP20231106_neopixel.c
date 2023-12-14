/*
 * MCP20231106_neopixel.c
 *
 * Created: 2023-11-06 오후 3:37:04
 * Author : Herb
 */ 

#include <avr/io.h>

#define F_CPU 16000000UL
#define FOSC 16000000UL
#include <util/delay.h>

#define interrupt_is_disabled

#define LED_COUNT 16

#include "mc_ws2812.h"



void led_on(rgbw_color color){
	ws2812_setleds_rgbw_PB(6,&color,1);
}

void led_all_off(){
	rgbw_color tempColors[LED_COUNT];
	for(unsigned char i=0;i<LED_COUNT;i++){
		tempColors[i]=(rgbw_color){0,0,0,0};
	}
	ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
}

//void ws2812_setleds_rgbw_P

//green, red, blue, white

#if 0
int main(void)
{
	DDRB |= 0x01<<6;
	//rgbw_color redColor;
	//redColor=(rgbw_color){128,0,0,0};
		
	rgbw_color tempColors[LED_COUNT];
	
    while (1) 
    {
		//for(int i=0;i<16;i++){
			//led_on(redColor);
			for(unsigned char k=0;k<LED_COUNT;k++){
				tempColors[k]=(rgbw_color){0,128,0,0};
				//tempColors[(i+1)%LED_COUNT]=(rgbw_color){128,0,0,0};
				//tempColors[(i+2)%LED_COUNT]=(rgbw_color){0,0,128,0};
			}
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
			_delay_ms(300);
			//led_all_off();
			for(unsigned char i=0;i<LED_COUNT;i++){
				tempColors[i]=(rgbw_color){0,0,0,0};
			}
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
			//_delay_ms(330);
			
			///////////////////////////////////////////////
			for(unsigned char k=0;k<LED_COUNT;k++){
				tempColors[k]=(rgbw_color){128,0,0,0};
			}
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
			_delay_ms(300);
			for(unsigned char i=0;i<LED_COUNT;i++){
				tempColors[i]=(rgbw_color){0,0,0,0};
			}
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
			
			///////////////////////////////////////////////
			for(unsigned char k=0;k<LED_COUNT;k++){
				tempColors[k]=(rgbw_color){0,0,128,0};
			}
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
			_delay_ms(300);
			for(unsigned char i=0;i<LED_COUNT;i++){
				tempColors[i]=(rgbw_color){0,0,0,0};
			}
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
			
			///////////////////////////////////////////////
			for(unsigned char k=0;k<LED_COUNT;k++){
				tempColors[k]=(rgbw_color){0,0,0,128};
			}
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
			_delay_ms(300);
			for(unsigned char i=0;i<LED_COUNT;i++){
				tempColors[i]=(rgbw_color){0,0,0,0};
			}
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
		//}
    }
}

#endif


#if 00
int main(void)
{
	DDRB |= 0x01<<6;
	//rgbw_color redColor;
	//redColor=(rgbw_color){128,0,0,0};
	
	rgbw_color tempColors[LED_COUNT];
	
	while (1)
	{
		for(int i=0;i<16;i++){
		//led_on(redColor);
		//for(unsigned char k=0;k<LED_COUNT;k++){
			tempColors[i]=(rgbw_color){0,128,0,0};
			tempColors[(i+1)%LED_COUNT]=(rgbw_color){128,0,0,0};
			tempColors[(i+2)%LED_COUNT]=(rgbw_color){0,0,128,0};
			tempColors[(i+3)%LED_COUNT]=(rgbw_color){0,0,0,128};
		//}
		ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
		_delay_ms(100);
		
		for(unsigned char i=0;i<LED_COUNT;i++){
			tempColors[i]=(rgbw_color){0,0,0,0};
		}
		ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
		}
	}
}
#endif

#if 0
int main(void)
{
	DDRB |= 0x01<<6;
	//rgbw_color redColor;
	//redColor=(rgbw_color){128,0,0,0};
	
	rgbw_color tempColors[LED_COUNT];
	
	while (1)
	{
		for(int i=0;i<255;i++){
			//led_on(redColor);
			for(unsigned char k=0;k<LED_COUNT;k++){
			tempColors[0]=(rgbw_color){0,i,0,0};
			tempColors[(0+1)%LED_COUNT]=(rgbw_color){i,0,0,0};
			tempColors[(0+2)%LED_COUNT]=(rgbw_color){0,0,i,0};
			tempColors[(0+3)%LED_COUNT]=(rgbw_color){0,0,0,i};
			}
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
			_delay_ms(1);
			
			for(unsigned char i=0;i<LED_COUNT;i++){
				tempColors[i]=(rgbw_color){0,0,0,0};
			}
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
		}
		for(int i=255;i>=0;i--){
			//led_on(redColor);
			//for(unsigned char k=0;k<LED_COUNT;k++){
			tempColors[0]=(rgbw_color){0,i,0,0};
			tempColors[(0+1)%LED_COUNT]=(rgbw_color){i,0,0,0};
			tempColors[(0+2)%LED_COUNT]=(rgbw_color){0,0,i,0};
			tempColors[(0+3)%LED_COUNT]=(rgbw_color){0,0,0,i};
			//}
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
			_delay_ms(1);
			
			for(unsigned char i=0;i<LED_COUNT;i++){
				tempColors[i]=(rgbw_color){0,0,0,0};
			}
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
		}
	}
}
#endif

#if 0
int main(void)
{
	DDRB |= 0x01<<6;
	//rgbw_color redColor;
	//redColor=(rgbw_color){128,0,0,0};
	
	rgbw_color tempColors[LED_COUNT];
	
	while (1)
	{
		for(int i=0;i<255;i++){
			//led_on(redColor);
			//for(unsigned char k=0;k<LED_COUNT;k++){
			tempColors[i/32]=(rgbw_color){0,i,0,0};
			tempColors[(i/32+1)%LED_COUNT]=(rgbw_color){i/2,0,0,0};
			tempColors[(i/32+2)%LED_COUNT]=(rgbw_color){0,0,i/2,0};
			tempColors[(i/32+3)%LED_COUNT]=(rgbw_color){0,0,0,i/2};
			//}
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
			_delay_ms(1);
			
			for(unsigned char i=0;i<LED_COUNT;i++){
				tempColors[i]=(rgbw_color){0,0,0,0};
			}
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
		}
		for(int i=255;i>=0;i--){
			//led_on(redColor);
			//for(unsigned char k=0;k<LED_COUNT;k++){
			tempColors[(255-i)/32+8]=(rgbw_color){0,i/2,0,0};
			tempColors[((255-i)/32+8+1)%LED_COUNT]=(rgbw_color){i/2,0,0,0};
			tempColors[((255-i)/32+8+2)%LED_COUNT]=(rgbw_color){0,0,i/2,0};
			tempColors[((255-i)/32+8+3)%LED_COUNT]=(rgbw_color){0,0,0,i/2};
			//}
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
			_delay_ms(1);
			
			for(unsigned char i=0;i<LED_COUNT;i++){
				tempColors[i]=(rgbw_color){0,0,0,0};
			}
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
		}
	}
}
#endif

#if 01
int main(void)
{
	DDRB |= 0x01<<6;
	//rgbw_color redColor;
	//redColor=(rgbw_color){128,0,0,0};
	
	rgbw_color tempColors[LED_COUNT];
	
	while (1)
	{
		for(unsigned char k=0;k<LED_COUNT-1;k++){
			tempColors[k]=(rgbw_color){128,0,0,0};
			tempColors[LED_COUNT-1-k]=(rgbw_color){128,0,0,0};
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
			_delay_ms(100);
				
				
			for(unsigned char i=0;i<LED_COUNT;i++){
				tempColors[i]=(rgbw_color){0,0,0,0};
			}
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
		}
		
		for(unsigned char k=0;k<LED_COUNT-1;k++){
			tempColors[k]=(rgbw_color){0,128,0,0};
			tempColors[LED_COUNT-1-k]=(rgbw_color){0,128,0,0};
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
			_delay_ms(100);
			
			
			for(unsigned char i=0;i<LED_COUNT;i++){
				tempColors[i]=(rgbw_color){0,0,0,0};
			}
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
		}
		
		for(unsigned char k=0;k<LED_COUNT-1;k++){
			tempColors[k]=(rgbw_color){0,0,128,0};
			tempColors[LED_COUNT-1-k]=(rgbw_color){0,0,128,0};
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
			_delay_ms(100);
			
			
			for(unsigned char i=0;i<LED_COUNT;i++){
				tempColors[i]=(rgbw_color){0,0,0,0};
			}
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
		}
		
		for(unsigned char k=0;k<LED_COUNT-1;k++){
			tempColors[k]=(rgbw_color){0,0,0,128};
			tempColors[LED_COUNT-1-k]=(rgbw_color){0,0,0,128};
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
			_delay_ms(100);
			
			
			for(unsigned char i=0;i<LED_COUNT;i++){
				tempColors[i]=(rgbw_color){0,0,0,0};
			}
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
		}
	}
}
#endif