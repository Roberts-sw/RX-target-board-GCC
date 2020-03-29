/* syncled.c 
   ------------------------------------------ TAB-size 4, code page UTF-8 --

Wijzigingen:
	RvL 29-3-2020	opname in lib/src
	RvL 20-3-2020	aanmaak
------------------------------------------------------------------------- */
#include "syncled.h"

	/* ---------------------------------------------------------
	private
	--------------------------------------------------------- */

	/* ---------------------------------------------------------
	public
	--------------------------------------------------------- */
	//single object:
void led_blink (Sled *self, u08 nr)
{	self->next=nr;
}
void led_drive (Sled *self, u08 on)
{	iopin_write(self->pin,self->off^on);
}
void led_flash (Sled *self, u08 nr)
{	if(!self->patt&&nr<8)
		self->patt=(16<<2*nr)-16&0x55555555;//ex: 3 => 0...0101010000
}
void led_init (Sled *self)
{	iopin_init(self->pin,self->off);
}
void led_run (Sled *self, u08 period, u08 cnt)
{	if(!cnt)
		self->now=self->next;
	if(self->patt)//if(0)//
	{	led_drive(self,1&self->patt);
		if(!(self->patt>>=1) )
			self->now=0;
	}
	else if(self->now >period)	led_drive(self,!!cnt );		//ON
	else if(self->now==period)	led_drive(self,!(1&cnt) );	//ALARM
	else if(self->now*4>cnt)	led_drive(self,!(3&cnt) );	//BLINK
	else 						led_drive(self,0);			//OFF
}

	//object vector:
void syncled_init (Ssyncled *self)
{	for( int i=self->nr_leds; i--; )
		led_init(self->leds + i);
}
void syncled_run (Ssyncled *self)
{	u08 dt=millis()-self->ms;
	if(dt>=256-94)//-94..-1 returns		//if(0>(int8_t)dt)//-128..-1 returns
		return;
	int cnt=self->cnt;
	self->ms+=cnt&3?94:93;				//(93+94+94+94=375 == 3000/8)
	for( int i=self->nr_leds; i--; )
		led_run(self->leds+i, self->period, self->cnt);
	if(4*self->period<=++self->cnt)
		self->cnt=0;
}
