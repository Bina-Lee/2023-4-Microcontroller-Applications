#include "mc_ws2812.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>


void inline ws2812_setleds_rgbw_PA(uint8_t pin,rgbw_color *ledarray, uint16_t leds)
{
	ws2812_sendarray_mask_PA((uint8_t*)ledarray,leds<<2,0x01<<pin);
	_delay_us(ws2812_resettime);
}
void inline ws2812_setleds_rgbw_PB(uint8_t pin,rgbw_color *ledarray, uint16_t leds)
{
	ws2812_sendarray_mask_PB((uint8_t*)ledarray,leds<<2,0x01<<pin);
	_delay_us(ws2812_resettime);
}
void inline ws2812_setleds_rgbw_PC(uint8_t pin,rgbw_color *ledarray, uint16_t leds)
{
	ws2812_sendarray_mask_PC((uint8_t*)ledarray,leds<<2,0x01<<pin);
	_delay_us(ws2812_resettime);
}
void inline ws2812_setleds_rgbw_PF(uint8_t pin,rgbw_color *ledarray, uint16_t leds)
{
	ws2812_sendarray_mask_PF((uint8_t*)ledarray,leds<<2,0x01<<pin);
	_delay_us(ws2812_resettime);
}
void inline ws2812_sendarray_mask_PA(uint8_t *data,uint16_t datlen,uint8_t maskhi)
{
	uint8_t curbyte,ctr,masklo;
	uint8_t sreg_prev;

	DDRA |= maskhi; // Enable output
	masklo	=~maskhi&PORTA;
	maskhi |=        PORTA;
	
	sreg_prev=SREG;
	#ifdef interrupt_is_disabled
	cli();
	#endif

	while (datlen--) {
		curbyte=*data++;
		
		asm volatile(
		"       ldi   %0,8  \n\t"
		#ifndef interrupt_is_disabled
		"       clt         \n\t"
		#endif
		"loop%=:            \n\t"
		"       out   %2,%3 \n\t"    //  '1' [01] '0' [01] - re
		#if (w1_nops&1)
		w_nop1
		#endif
		#if (w1_nops&2)
		w_nop2
		#endif
		#if (w1_nops&4)
		w_nop4
		#endif
		#if (w1_nops&8)
		w_nop8
		#endif
		#if (w1_nops&16)
		w_nop16
		#endif
		"       sbrs  %1,7  \n\t"    //  '1' [03] '0' [02]
		"       out   %2,%4 \n\t"    //  '1' [--] '0' [03] - fe-low
		"       lsl   %1    \n\t"    //  '1' [04] '0' [04]
		#if (w2_nops&1)
		w_nop1
		#endif
		#if (w2_nops&2)
		w_nop2
		#endif
		#if (w2_nops&4)
		w_nop4
		#endif
		#if (w2_nops&8)
		w_nop8
		#endif
		#if (w2_nops&16)
		w_nop16
		#endif
		"       out   %2,%4 \n\t"    //  '1' [+1] '0' [+1] - fe-high
		#if (w3_nops&1)
		w_nop1
		#endif
		#if (w3_nops&2)
		w_nop2
		#endif
		#if (w3_nops&4)
		w_nop4
		#endif
		#if (w3_nops&8)
		w_nop8
		#endif
		#if (w3_nops&16)
		w_nop16
		#endif

		"       dec   %0    \n\t"    //  '1' [+2] '0' [+2]
		"       brne  loop%=\n\t"    //  '1' [+3] '0' [+4]
		:	"=&d" (ctr)
		:	"r" (curbyte), "I" (_SFR_IO_ADDR(PORTA)), "r" (maskhi), "r" (masklo)
		);
	}
	
	SREG=sreg_prev;
}
void inline ws2812_sendarray_mask_PB(uint8_t *data,uint16_t datlen,uint8_t maskhi)
{
	uint8_t curbyte,ctr,masklo;
	uint8_t sreg_prev;

	DDRB |= maskhi; // Enable output
	masklo	=~maskhi&PORTB;
	maskhi |=        PORTB;
	
	sreg_prev=SREG;
	#ifdef interrupt_is_disabled
	cli();
	#endif

	while (datlen--) {
		curbyte=*data++;
		
		asm volatile(
		"       ldi   %0,8  \n\t"
		#ifndef interrupt_is_disabled
		"       clt         \n\t"
		#endif
		"loop%=:            \n\t"
		"       out   %2,%3 \n\t"    //  '1' [01] '0' [01] - re
		#if (w1_nops&1)
		w_nop1
		#endif
		#if (w1_nops&2)
		w_nop2
		#endif
		#if (w1_nops&4)
		w_nop4
		#endif
		#if (w1_nops&8)
		w_nop8
		#endif
		#if (w1_nops&16)
		w_nop16
		#endif
		"       sbrs  %1,7  \n\t"    //  '1' [03] '0' [02]
		"       out   %2,%4 \n\t"    //  '1' [--] '0' [03] - fe-low
		"       lsl   %1    \n\t"    //  '1' [04] '0' [04]
		#if (w2_nops&1)
		w_nop1
		#endif
		#if (w2_nops&2)
		w_nop2
		#endif
		#if (w2_nops&4)
		w_nop4
		#endif
		#if (w2_nops&8)
		w_nop8
		#endif
		#if (w2_nops&16)
		w_nop16
		#endif
		"       out   %2,%4 \n\t"    //  '1' [+1] '0' [+1] - fe-high
		#if (w3_nops&1)
		w_nop1
		#endif
		#if (w3_nops&2)
		w_nop2
		#endif
		#if (w3_nops&4)
		w_nop4
		#endif
		#if (w3_nops&8)
		w_nop8
		#endif
		#if (w3_nops&16)
		w_nop16
		#endif

		"       dec   %0    \n\t"    //  '1' [+2] '0' [+2]
		"       brne  loop%=\n\t"    //  '1' [+3] '0' [+4]
		:	"=&d" (ctr)
		:	"r" (curbyte), "I" (_SFR_IO_ADDR(PORTB)), "r" (maskhi), "r" (masklo)
		);
	}
	
	SREG=sreg_prev;
}
void inline ws2812_sendarray_mask_PC(uint8_t *data,uint16_t datlen,uint8_t maskhi)
{
	uint8_t curbyte,ctr,masklo;
	uint8_t sreg_prev;

	DDRC |= maskhi; // Enable output
	masklo	=~maskhi&PORTC;
	maskhi |=        PORTC;
	
	sreg_prev=SREG;
	#ifdef interrupt_is_disabled
	cli();
	#endif

	while (datlen--) {
		curbyte=*data++;
		
		asm volatile(
		"       ldi   %0,8  \n\t"
		#ifndef interrupt_is_disabled
		"       clt         \n\t"
		#endif
		"loop%=:            \n\t"
		"       out   %2,%3 \n\t"    //  '1' [01] '0' [01] - re
		#if (w1_nops&1)
		w_nop1
		#endif
		#if (w1_nops&2)
		w_nop2
		#endif
		#if (w1_nops&4)
		w_nop4
		#endif
		#if (w1_nops&8)
		w_nop8
		#endif
		#if (w1_nops&16)
		w_nop16
		#endif
		"       sbrs  %1,7  \n\t"    //  '1' [03] '0' [02]
		"       out   %2,%4 \n\t"    //  '1' [--] '0' [03] - fe-low
		"       lsl   %1    \n\t"    //  '1' [04] '0' [04]
		#if (w2_nops&1)
		w_nop1
		#endif
		#if (w2_nops&2)
		w_nop2
		#endif
		#if (w2_nops&4)
		w_nop4
		#endif
		#if (w2_nops&8)
		w_nop8
		#endif
		#if (w2_nops&16)
		w_nop16
		#endif
		"       out   %2,%4 \n\t"    //  '1' [+1] '0' [+1] - fe-high
		#if (w3_nops&1)
		w_nop1
		#endif
		#if (w3_nops&2)
		w_nop2
		#endif
		#if (w3_nops&4)
		w_nop4
		#endif
		#if (w3_nops&8)
		w_nop8
		#endif
		#if (w3_nops&16)
		w_nop16
		#endif

		"       dec   %0    \n\t"    //  '1' [+2] '0' [+2]
		"       brne  loop%=\n\t"    //  '1' [+3] '0' [+4]
		:	"=&d" (ctr)
		:	"r" (curbyte), "I" (_SFR_IO_ADDR(PORTC)), "r" (maskhi), "r" (masklo)
		);
	}
	
	SREG=sreg_prev;
}
void inline ws2812_sendarray_mask_PF(uint8_t *data,uint16_t datlen,uint8_t maskhi)
{
  uint8_t curbyte,ctr,masklo;
  uint8_t sreg_prev;

  DDRF |= maskhi; // Enable output
  masklo	=~maskhi&PORTF;
  maskhi |=        PORTF;
  
  sreg_prev=SREG;
#ifdef interrupt_is_disabled
  cli();  
#endif  

  while (datlen--) {
    curbyte=*data++;
    
    asm volatile(
    "       ldi   %0,8  \n\t"
#ifndef interrupt_is_disabled
    "       clt         \n\t"
#endif
    "loop%=:            \n\t"
    "       out   %2,%3 \n\t"    //  '1' [01] '0' [01] - re
#if (w1_nops&1)
w_nop1
#endif
#if (w1_nops&2)
w_nop2
#endif
#if (w1_nops&4)
w_nop4
#endif
#if (w1_nops&8)
w_nop8
#endif
#if (w1_nops&16)
w_nop16
#endif
    "       sbrs  %1,7  \n\t"    //  '1' [03] '0' [02]
    "       out   %2,%4 \n\t"    //  '1' [--] '0' [03] - fe-low
    "       lsl   %1    \n\t"    //  '1' [04] '0' [04]
#if (w2_nops&1)
  w_nop1
#endif
#if (w2_nops&2)
  w_nop2
#endif
#if (w2_nops&4)
  w_nop4
#endif
#if (w2_nops&8)
  w_nop8
#endif
#if (w2_nops&16)
  w_nop16 
#endif
    "       out   %2,%4 \n\t"    //  '1' [+1] '0' [+1] - fe-high
#if (w3_nops&1)
w_nop1
#endif
#if (w3_nops&2)
w_nop2
#endif
#if (w3_nops&4)
w_nop4
#endif
#if (w3_nops&8)
w_nop8
#endif
#if (w3_nops&16)
w_nop16
#endif

    "       dec   %0    \n\t"    //  '1' [+2] '0' [+2]
    "       brne  loop%=\n\t"    //  '1' [+3] '0' [+4]
    :	"=&d" (ctr)
    :	"r" (curbyte), "I" (_SFR_IO_ADDR(PORTF)), "r" (maskhi), "r" (masklo)
    );
  }
  
  SREG=sreg_prev;
}