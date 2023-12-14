#ifndef LIGHT_WS2812_H_
#define LIGHT_WS2812_H_

#include <avr/io.h>
#include <avr/interrupt.h>


#if !defined(ws2812_resettime)
#define ws2812_resettime    100
#endif

///////////////////////////////////////////////////////////////////////
// Define I/O pin
///////////////////////////////////////////////////////////////////////

typedef struct rgb_color{
	uint8_t green, red, blue;
} rgb_color;
typedef struct rgbw_color{
	uint8_t green, red, blue, white;
} rgbw_color;


// Timing in ns
#define w_zeropulse   350
#define w_onepulse    900
#define w_totalperiod 1250

// Fixed cycles used by the inner loop
#define w_fixedlow    2
#define w_fixedhigh   4
#define w_fixedtotal  8   

// Insert NOPs to match the timing, if possible
#define w_zerocycles    (((F_CPU/1000)*w_zeropulse          )/1000000)
#define w_onecycles     (((F_CPU/1000)*w_onepulse    +500000)/1000000)
#define w_totalcycles   (((F_CPU/1000)*w_totalperiod +500000)/1000000)

// w1 - nops between rising edge and falling edge - low
#define w1 (w_zerocycles-w_fixedlow)
// w2   nops between fe low and fe high
#define w2 (w_onecycles-w_fixedhigh-w1)
// w3   nops to complete loop
#define w3 (w_totalcycles-w_fixedtotal-w1-w2)

#if w1>0
  #define w1_nops w1
#else
  #define w1_nops  0
#endif
 

#if w2>0
#define w2_nops w2
#else
#define w2_nops  0
#endif

#if w3>0
#define w3_nops w3
#else
#define w3_nops  0
#endif

#define w_nop1  "nop      \n\t"
#ifdef interrupt_is_disabled
#define w_nop2  "brid .+0 \n\t"
#else
#define w_nop2  "brtc .+0 \n\t"
#endif
#define w_nop4  w_nop2 w_nop2
#define w_nop8  w_nop4 w_nop4
#define w_nop16 w_nop8 w_nop8

/* User Interface
 * 
 * Input:
 *         ledarray:           An array of GRB data describing the LED colors
 *         number_of_leds:     The number of LEDs to write
 *         pinmask (optional): Bitmask describing the output bin. e.g. _BV(PB0)
 *
 * The functions will perform the following actions:
 *         - Set the data-out pin as output
 *         - Send out the LED data 
 *         - Wait 50µs to reset the LEDs
 */
void ws2812_setleds_rgbw_PA(uint8_t pin,rgbw_color *ledarray, uint16_t leds);
void ws2812_sendarray_mask_PA(uint8_t *array,uint16_t length, uint8_t pinmask);
void ws2812_setleds_rgbw_PB(uint8_t pin,rgbw_color *ledarray, uint16_t leds);
void ws2812_sendarray_mask_PB(uint8_t *array,uint16_t length, uint8_t pinmask);
void ws2812_setleds_rgbw_PC(uint8_t pin,rgbw_color *ledarray, uint16_t leds);
void ws2812_sendarray_mask_PC(uint8_t *array,uint16_t length, uint8_t pinmask);
void ws2812_setleds_rgbw_PF(uint8_t pin,rgbw_color *ledarray, uint16_t leds);
void ws2812_sendarray_mask_PF(uint8_t *array,uint16_t length, uint8_t pinmask);

#endif /* LIGHT_WS2812_H_ */