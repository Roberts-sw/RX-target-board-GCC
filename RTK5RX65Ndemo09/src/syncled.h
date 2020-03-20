#ifndef SYNCLED_H_
#define SYNCLED_H_
/* syncled.h 
   ------------------------------------------ TAB-size 4, code page UTF-8 --

Wijzigingen:
	RvL 20-3-2020	aanmaak
------------------------------------------------------------------------- */
#include "iopin.h"	//iopin_init(), iopin_write()

	/* ---------------------------------------------------------
	protected
	--------------------------------------------------------- */
typedef struct st_led
{	eIOPIN pin;			//ex. PD7
	uint8_t off;		//'0' '1'
	uint8_t next;		//0..7|8|9 = #blinks/3s|alarm|on: 
	uint8_t now;		//#blinks*4/cnt
	uint32_t patt;		//lsb-first: '1'=on
} Sled;

struct st_syncled
{	uint8_t period;		//#blinks for alarm
	uint8_t cnt;		//0..31
	uint8_t ms;			//94+94+94+93=375 [ms] == 3[s] / 8
	uint8_t nr_leds;	//#leds in *leds
	Sled *leds;
};

	/* ---------------------------------------------------------
	public
	--------------------------------------------------------- */
typedef struct st_syncled Ssyncled; 

#ifdef __cplusplus
#	define CCALL	extern "C"	//C-call conventions
#else
#	define CCALL
#endif

	//object:
CCALL void led_blink(Sled *self, uint8_t nr);
CCALL void led_drive(Sled *self, uint8_t on);
CCALL void led_flash(Sled *self, uint8_t nr);
CCALL void led_init(Sled *self);
CCALL void led_run(Sled *self, uint8_t period, uint8_t cnt);
	//vector:
CCALL void syncled_init(Ssyncled *self);
CCALL void syncled_run(Ssyncled *self);

#	undef CCALL

/* usage:
#include "syncled.h"

Sled feedled[]=
{	{PD6,1,0},	//target board LED0: '1'=off, #blinks=0 == OFF
	{PD7,1,1},	//target board LED1: '1'=off, #blinks=1
};
Sled alarmled=
{PB0,1,7};		//option board LED2: '1'=off, #blinks=8
Ssyncled leds=
{	8,0,0,3,{feedled+0, feedled+1, &alarmled}
};

int main ()
{	...		//setup

	syncled_init(&leds);

	while(1)
	{	syncled_run(&leds);
		...	//other loop calls
	}
}
*/

#endif//SYNCLED_H_
