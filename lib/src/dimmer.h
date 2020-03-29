#ifndef DIMMER_H_
#define DIMMER_H_
/* dimmer.h 
   ------------------------------------------ TAB-size 4, code page UTF-8 --

Wijzigingen:
	RvL 29-3-2020	opname in lib/src
	RvL 28-3-2020	aanmaak, zie ../../doc/nl/automaat.pdf
------------------------------------------------------------------------- */
#include "iopin.h"

	/* ---------------------------------------------------------
	protected
	--------------------------------------------------------- */

	/* ---------------------------------------------------------
	public
	--------------------------------------------------------- */
typedef enum en_ev
{	RELEASE, PRESS, HOLD,
	DIM_EVENTS, NO_DIM_EVENT=DIM_EVENTS
} DIMMER_EVENT;

typedef enum en_st
{//stable press hold
    O,    O1,   UP,
    H,    H1,   DN,
    L,    L1, LAMP_STATES
} LAMP_STATE;

extern u08 pct;

DIMMER_EVENT dimmer_event(u08 button_active);

LAMP_STATE (*const DIMMER_AUTOM[LAMP_STATES][DIM_EVENTS])(void);

typedef LAMP_STATE (*const dimmer_action)(void);


/* usage:

#include "dimmer.h"
u32 volatile millis( void );//Arduino-style timing

#define SW1 PB1     //'0'=pressed, '1'=released
#define sample(b)	!iopin_read(b)

void dimmer_run (void)
{	static u08 sample_time;
	if(156<=(u08)(millis()- sample_time) )//-100..-1 [ms]
		return;
	sample_time+=100;

	DIMMER_EVENT ev=dimmer_event( sample(SW1) );
	if(NO_DIM_EVENT==ev)
		return;

	static LAMP_STATE st=O;
	dimmer_action act=DIMMER_AUTOM[st][ev];
	if(!act)
		return;
	st=act();

	drive_lamp_output_with( pct );
}

main()
{	do	dimmer_run();
	while(1);
}

*/

#endif/*DIMMER_H_*/
