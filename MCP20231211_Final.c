/*
 * MCP20231211_Final.c
 *
 * Created: 2023-12-11 오후 8:48:18
 * Author : Herb
 */ 

#include <avr/io.h>

#include "mc_ws2812.h"

#define F_CPU 16000000UL
#include<util/delay.h>
#include<avr/interrupt.h>


#define BAUD_9600 103
#define BAUD_2400 416
#define BAUD_4800 207
#define BAUD_14400 68
#define BAUD_19200 51
#define BAUD_28800 34
#define BAUD_38400 25
#define BAUD_57600 16
#define BAUD_76800 12
#define BAUD_115200 8
#define BAUD_230400 3
#define BAUD_250000 3


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

#define ConnectionCheck 0x01
#define Betting 0x02
#define GameStart 0x03
#define Report 0x04
#define Result 0x05

#define LED_COUNT 16

#define FREQ(x) (unsigned int)(16000000/(2*8*(1+x)))

#define ON 1
#define OFF 0

int mode=1;

int on_off=0;

int current_buzzer=0;

void buzzer_play(unsigned int y){
	DDRB |= 0x02;
	TCCR1A |= 0x04;
	TCCR1B |= 0x1A;
	ICR1=y;
}
void buzzer_stop(){
	TCCR1A=0x00;
	TCCR1B=0x00;
	TCNT1=0;
	ICR1=0;
	DDRB=~(0x02);
}

unsigned int note_freq[7]={
	2093,
	2349,
	2637,
	2794,
	3136,
	3520,
	3951
};

rgbw_color tempColors[LED_COUNT];

char tempBool[16][4]={0,};

unsigned char brightness=16;


uint8_t _len=0;
uint8_t _command=0;
uint8_t _command_data1=0;
uint8_t _command_data2=0;





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
unsigned int adc_value=0;



int current_led_rotate=0;
void led_rotate_sample(){
	current_led_rotate++;
	if(current_led_rotate>15)current_led_rotate=0;
	for(int i=0;i<16;i++)tempColors[i]=(rgbw_color){0,0,0,0};
	tempColors[current_led_rotate]=(rgbw_color){0,0,0,brightness};
	ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
}

unsigned int main_led_routine_arr[45][16]={
	{1,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
	{0,1,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1},
	{0,0,1,0, 0,0,0,0, 0,0,0,0, 0,0,1,0},
	{0,0,0,1, 0,0,0,0, 0,0,0,0, 0,1,0,0},
	{0,0,0,0, 1,0,0,0, 0,0,0,0, 1,0,0,0},
	{0,0,0,0, 0,1,0,0, 0,0,0,1, 0,0,0,0},
	{0,0,0,0, 0,0,1,0, 0,0,1,0, 0,0,0,0},
	{0,0,0,0, 0,0,0,1, 0,1,0,0, 0,0,0,0},
	{0,0,0,0, 0,0,0,0, 1,0,0,0, 0,0,0,0},
		
	{1,0,0,0, 0,0,0,0, 1,0,0,0, 0,0,0,0},
	{0,1,0,0, 0,0,0,0, 1,0,0,0, 0,0,0,1},
	{0,0,1,0, 0,0,0,0, 1,0,0,0, 0,0,1,0},
	{0,0,0,1, 0,0,0,0, 1,0,0,0, 0,1,0,0},
	{0,0,0,0, 1,0,0,0, 1,0,0,0, 1,0,0,0},
	{0,0,0,0, 0,1,0,0, 1,0,0,1, 0,0,0,0},
	{0,0,0,0, 0,0,1,0, 1,0,1,0, 0,0,0,0},
	{0,0,0,0, 0,0,0,1, 1,1,0,0, 0,0,0,0},
		
	{1,0,0,0, 0,0,0,1, 1,1,0,0, 0,0,0,0},
	{0,1,0,0, 0,0,0,1, 1,1,0,0, 0,0,0,1},
	{0,0,1,0, 0,0,0,1, 1,1,0,0, 0,0,1,0},
	{0,0,0,1, 0,0,0,1, 1,1,0,0, 0,1,0,0},
	{0,0,0,0, 1,0,0,1, 1,1,0,0, 1,0,0,0},
	{0,0,0,0, 0,1,0,1, 1,1,0,1, 0,0,0,0},
	{0,0,0,0, 0,0,1,1, 1,1,1,0, 0,0,0,0},
		
	{1,0,0,0, 0,0,1,1, 1,1,1,0, 0,0,0,0},
	{0,1,0,0, 0,0,1,1, 1,1,1,0, 0,0,0,1},
	{0,0,1,0, 0,0,1,1, 1,1,1,0, 0,0,1,0},
	{0,0,0,1, 0,0,1,1, 1,1,1,0, 0,1,0,0},
	{0,0,0,0, 1,0,1,1, 1,1,1,0, 1,0,0,0},
	{0,0,0,0, 0,1,1,1, 1,1,1,1, 0,0,0,0},
		
	{1,0,0,0, 0,1,1,1, 1,1,1,1, 0,0,0,0},
	{0,1,0,0, 0,1,1,1, 1,1,1,1, 0,0,0,1},
	{0,0,1,0, 0,1,1,1, 1,1,1,1, 0,0,1,0},
	{0,0,0,1, 0,1,1,1, 1,1,1,1, 0,1,0,0},
	{0,0,0,0, 1,1,1,1, 1,1,1,1, 1,0,0,0},
		
	{1,0,0,0, 1,1,1,1, 1,1,1,1, 1,0,0,0},
	{0,1,0,0, 1,1,1,1, 1,1,1,1, 1,0,0,1},
	{0,0,1,0, 1,1,1,1, 1,1,1,1, 1,0,1,0},
	{0,0,0,1, 1,1,1,1, 1,1,1,1, 1,1,0,0},
		
	{1,0,0,1, 1,1,1,1, 1,1,1,1, 1,1,0,0},
	{0,1,0,1, 1,1,1,1, 1,1,1,1, 1,1,0,1},
	{0,0,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,0},
		
	{1,0,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,0},
	{0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1},
		
	{1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1},	
};

int game_led_rotate_RGB=0;
int game_led_rotate_BGR=0;

void main_led_routine(){
	game_led_rotate_BGR=0;
	game_led_rotate_RGB=0;
	current_led_rotate++;
	if(current_led_rotate>45)current_led_rotate=0;
	for(int i=0;i<16;i++)tempColors[i]=(rgbw_color){0,0,0,0};
	for(int i=0;i<16;i++){
		if(main_led_routine_arr[current_led_rotate][i]){
			tempColors[i]=(rgbw_color){0,brightness,0,0};
		}
	}
	ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
}

void game_mode_RGB(){
	current_led_rotate=0;
	game_led_rotate_BGR=0;
	
	game_led_rotate_RGB++;
	if(game_led_rotate_RGB>47)game_led_rotate_RGB=0;
	
	for(int i=0;i<16;i++)tempColors[i]=(rgbw_color){0,0,0,0};
	if(game_led_rotate_RGB%3 == 0){
		tempColors[game_led_rotate_RGB/3]=(rgbw_color){0,brightness,0,0};
	}else if(game_led_rotate_RGB%3 == 1){
		tempColors[game_led_rotate_RGB/3]=(rgbw_color){brightness,0,0,0};
	}else if(game_led_rotate_RGB%3 == 2){
		tempColors[game_led_rotate_RGB/3]=(rgbw_color){0,0,brightness,0};
	}
	ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
}

void game_mode_BGR(){
	current_led_rotate=0;
	game_led_rotate_RGB=0;
	
	game_led_rotate_BGR++;
	if(game_led_rotate_BGR>47)game_led_rotate_BGR=0;
	
	for(int i=0;i<16;i++)tempColors[i]=(rgbw_color){0,0,0,0};
	if(game_led_rotate_BGR%3 == 0){
		tempColors[game_led_rotate_BGR/3]=(rgbw_color){0,0,brightness,0};
	}else if(game_led_rotate_BGR%3 == 1){
		tempColors[game_led_rotate_BGR/3]=(rgbw_color){brightness,0,0,0};
	}else if(game_led_rotate_BGR%3 == 2){
		tempColors[game_led_rotate_BGR/3]=(rgbw_color){0,brightness,0,0};
	}
	ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
}

int count=0;

ISR(TIMER3_OVF_vect){
	//TCNT3=0xFF-0x3D08;
	TCNT3=0xFF8F-1024;//(adc_value>>1);//0xFF - adc_value<<4;// | 0x0400;//0xFFFF - (adc_value<<2);//0xFFFF-0x3D08-adc_value;
	//to make 5ms
	
	//TovVal++;
	//
	//if(TovVal==2){
	//led_rotate_sample();
	
	//game_mode_RGB();
	
	if(mode==1){
		main_led_routine();	
	}else if(mode==2){
		game_mode_RGB();
	}else if(mode==3){
		game_mode_BGR();
	}
	
	//TovVal=0;
	//}
	sei();
}








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

int quit_time=0;



ISR(TIMER5_OVF_vect){
	TCNT5=0xFF-0x3E08;
	//0x3D08
	count++;
	//Printf("count : %d\n",count);
	
	
	if(quit_time==count){
		int now_mode=mode;
		TIMSK5 &= ~(1<<TOIE1);
		mode=0;
		
		//Printf("%d\n",game_led_rotate_RGB);
		//Printf("%d\n",game_led_rotate_BGR);
		
		while(!(UCSR1A & (1<<UDRE1)));
		UDR1 = 0x02;
		while(!(UCSR1A & (1<<UDRE1)));
		UDR1 = 0x02;
		while(!(UCSR1A & (1<<UDRE1)));
		UDR1 = 0x04;
		
		uint8_t packet=0x00;
		if(now_mode==2){
			if(game_led_rotate_RGB%3==0)packet |= 0x80;
			else if(game_led_rotate_RGB%3==1)packet |= 0x40;
			else if(game_led_rotate_RGB%3==2)packet |= 0x20;
			packet=packet | (game_led_rotate_RGB/3 + 1);
		}
		else if(now_mode==3){
			if(game_led_rotate_BGR%3==0)packet |= 0x20;
			else if(game_led_rotate_BGR%3==1)packet |= 0x40;
			else if(game_led_rotate_BGR%3==2)packet |= 0x80;
			packet=packet | (game_led_rotate_BGR/3 + 1);
		}
		while(!(UCSR1A & (1<<UDRE1)));
		UDR1 = packet;
		while(!(UCSR1A & (1<<UDRE1)));
		UDR1 = 0x03;
		Printf("\n");
		
		
		
	}
	sei();
}

int decode =0;
int len_copy=2;

ISR(USART1_RX_vect){
	cli();
	
	unsigned char _usart_rcv=UDR1;
	
	//Printf("%d : %d\n",decode,_usart_rcv);
	//Printf("%c",_usart_rcv);
	if(decode==0 && _usart_rcv==STX){
		decode=1;
	}else if(decode==1){
		_len=_usart_rcv;
		len_copy=2;
		decode=2;
	}else if(decode==2){
		_command=_usart_rcv;
		decode=3;
		if(_len==0x01)decode=5;
	}else if(decode==3){
		_command_data1=_usart_rcv;
		
		if(_len==0x02){
			decode=5;
		}
		else if(_len>0x02){
			decode=4;
		}
	}else if(decode==4){
		_command_data2=_usart_rcv;
		decode=5;
		if(_len>0x03)decode=6;
	}else if(decode==5){
		if(_usart_rcv==ETX){
			sint_flag=1;
		}else{
			Printf("ETX ERROR\n");
		}
		decode=0;
	}
	else if(decode==6){
		_buff[len_copy-2]=_usart_rcv;
		_buff[len_copy-1]=0;
		if(len_copy+2==_len)decode=5;
		len_copy++;
	}
	sei();
}

int red=0;
int green=0;
int blue=0;
int index=0;

void betting(uint8_t data){
	red=0;
	green=0;
	blue=0;
	index=0;
	if(data>>7 & 0x01)red=1;
	if(data>>6 & 0x01)green=1;
	if(data>>5 & 0x01)blue=1;
	index=(data & 0x1F)-1;
	
	for(int i=0;i<16;i++)tempColors[i]=(rgbw_color){0,0,0,0};
		
	if(green)tempColors[index]=(rgbw_color){brightness,0,0,0};
	else if(red)tempColors[index]=(rgbw_color){0,brightness,0,0};
	else if(blue)tempColors[index]=(rgbw_color){0,0,brightness,0};
		
	ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
	
	mode=0;
}
void start_game(uint8_t data1, uint8_t data2){
	
	
	if(data1 & 0x80){
		if(green){
			game_led_rotate_RGB=1;
		}
		else if(blue){
			game_led_rotate_RGB=2;
		}
	}else if(data1 & 0x40){
		if(green){
			game_led_rotate_BGR=1;;
		}
		else if(red){
			game_led_rotate_BGR=2;
		}
	}
	game_led_rotate_RGB=game_led_rotate_RGB+ 3*((data1 & 0x1F)-1);
	game_led_rotate_BGR=game_led_rotate_BGR+ 3*((data1 & 0x1F)-1);
	
	
	
	quit_time=data2;
	
	count=-1;
	TIMSK5 |= 1<<TOIE1;
	
	if(data1 & 0x80){
		mode=2;
	}
	else if(data1 & 0x40){
		mode=3;
	}
	//Printf("mode : %d\n",mode);
}

void hex_command(uint8_t len,uint8_t cd,uint8_t cd_t1,uint8_t cd_t2){
	if(cd==ConnectionCheck){
		//Printf("commandConnectionCheck\n");
		if(len==0x01){
			Printf("[RX:01] Text :\n");
		}else if(len==0x02){
			Printf("[RX:01] Text : %c\n",cd_t1);
		}else if(len==0x03){
			Printf("[RX:01] Text : %c%c\n",cd_t1,cd_t2);
		}else{
			Printf("[RX:01] Text : %c%c%s\n",cd_t1,cd_t2,_buff);
			//Printf("%c",_buff[0]);
			Printf("\n");
		}
	}else if(cd==Betting){
		//Printf("command Betting\n");
		betting(cd_t1);
		char rgb;
		if(red==1)rgb='r';
		else if(green==1)rgb='g';
		else if(blue==1)rgb='b';
		Printf("[RX:02] Color : %c, Value : %d\n",rgb,index+1);
	}else if(cd==GameStart){
		start_game(cd_t1,cd_t2);
		if(cd_t1 & 0x80){
			Printf("[RX:03] ColorCycle : RGB, Value : %d, Timer : %d\n",cd_t1&0x1F,cd_t2);
		}
		else if(cd_t1 & 0x40){
			Printf("[RX:03] ColorCycle : BGR, Value : %d, Timer : %d\n",cd_t1&0x1F,cd_t2);
		}
	}else if(cd==Result){
		if(cd_t1==0xF0){
			Printf("[RX:05] Result : Win\n");
			win_sound();
		}
		else if(cd_t1==0x0F){
			Printf("[RX:05] Result : Lose\n");
			lose_sound();
		}
		mode=1;
	}else{
		Printf("Invalid Command\n");
	}
}


void disp_led(unsigned int adc){
	PORTC=0xFF;
	for(int i=0;i<=(adc/128);i++) PORTC &= ~(0x01<<i);
}

void win_sound(){
	DDRB |= 0x02;
	TCCR1A |= 0x04;
	TCCR1B |= 0x1A;
	ICR1=FREQ(note_freq[0]);
	_delay_ms(200);
	ICR1=FREQ(note_freq[1]);
	_delay_ms(200);
	ICR1=FREQ(note_freq[2]);
	_delay_ms(200);
	TCCR1A=0x00;
	TCCR1B=0x00;
	TCNT1=0;
	ICR1=0;
	DDRB=~(0x02);
}
void lose_sound(){
	DDRB |= 0x02;
	TCCR1A |= 0x04;
	TCCR1B |= 0x1A;
	ICR1=FREQ(note_freq[2]);
	_delay_ms(200);
	ICR1=FREQ(note_freq[1]);
	_delay_ms(200);
	ICR1=FREQ(note_freq[0]);
	_delay_ms(200);
	TCCR1A=0x00;
	TCCR1B=0x00;
	TCNT1=0;
	ICR1=0;
	DDRB=~(0x02);
}

int main(void)
{
	unsigned char rx_char=0;
	cli();
	
	//LED
	DDRC=0xFF;
	
	//button and intterupt
	DDRE &= ~(0x30);
	EICRB = 0xAA;
	EIMSK = 0x30;
	EIFR |= 0x30;
	
	//BAUD Rate
	UBRR1H = (unsigned char)(BAUD_38400>>8);
	UBRR1L = (unsigned char)(BAUD_38400);
	
	UCSR1A=0x00;
	
	//RX, TX Enable
	UCSR1B = (1<<TXEN1) | (1<<RXEN1);
	UCSR1B |=(1<<RXCIE1);
	//character size : 8bit
	// 5~8bit : 00~11
	//if use 9bit : 11 and { UCSR1B |= 1<<UCSZ12; }
	UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);
	
	//if use parity
	UCSR1C |= 1<<UPM11; // Even Parity
	UCSR1C |=	1<<UPM10; // Odd Parity (use with UPM11;)
	
	//if use 2 stop bit
	UCSR1C |= 1<<USBS1;
	
	
	//Timer3
	
	TCCR3A=0x00;
	//NORMAL MODE
	TCCR3B |= (1<<CS12);
	//1024 prescale
	TCCR3B |=(1<<CS10);
	TCNT3=(1023<<4);
	//to generate OV at every 5ms
	TIMSK3 |= 1<<TOIE1;
	//timer0 overflow interrupt enable
	TIFR3 |=1<<TOV1;
	
	
	//Timer5
	
	TCCR5A=0x00;
	//NORMAL MODE
	TCCR5B |= (1<<CS12);
	//1024 prescale
	TCCR5B |=(1<<CS10);
	TCNT5=(1023<<4);
	//to generate OV at every 5ms
	//TIMSK5 |= 1<<TOIE1;
	//timer0 overflow interrupt enable
	TIFR5 |=1<<TOV1;
	
	sei();
	
	PORTC=0xFF;
	
	//NEOPIXEL
	DDRB |= 0x01<<6;
	//Buzzer
	DDRB |= 0x80;
	
	led_all_on();
	for(int i=0;i<16;i++){
		led_red_n(i);
		_delay_ms(25);
	}
	for(int i=0;i<16;i++){
		led_green_n(i);
		_delay_ms(25);
	}
	for(int i=0;i<16;i++){
		led_blue_n(i);
		_delay_ms(25);
	}
	//led_all_on();
	
	for(int i=0;i<2;i++){
		brightness=0;
		set_led();
		ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
		_delay_ms(250);
		brightness=16;
		set_led();
		ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
		_delay_ms(250);
	}
	brightness=16;
	led_all_on();
	
	win_sound();
	_delay_ms(500);
	lose_sound();
	_delay_ms(500);
	
	//Printf("System Init\nUART Init\n");
	
	while (1)
	{	
		adc_value=GetADCData(0);
		disp_led(adc_value);
		brightness=((adc_value/128)+1)*2;
		if(mode==0){
			for(int i=0;i<16;i++){
				if(tempColors[i].blue !=0)tempColors[i].blue =brightness;
				if(tempColors[i].red  !=0)tempColors[i].red  =brightness;
				if(tempColors[i].green!=0)tempColors[i].green=brightness;
				if(tempColors[i].white!=0)tempColors[i].white=brightness;
			}
			ws2812_setleds_rgbw_PB(6,tempColors,LED_COUNT);
		}
		
		//PORTC=~(adc_value>>2);
		
		if(sint_flag){
			hex_command(_len,_command,_command_data1,_command_data2);
			sint_flag=0;
			_len=0;
			_command=0;
			_command_data1=0;
			_command_data2=0;
		}
	}
}