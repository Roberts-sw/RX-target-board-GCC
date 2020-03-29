/* dimmer.c 
   ------------------------------------------ TAB-size 4, code page UTF-8 --

Wijzigingen:
	RvL 29-3-2020	opname in lib/src
	RvL 28-3-2020	aanmaak, zie ../../doc/nl/automaat.pdf
------------------------------------------------------------------------- */
#include "dimmer.h"

	/* ---------------------------------------------------------
	private
	--------------------------------------------------------- */
#define Hbit	128
#define REP_WT_	10
static u08 btn_cnt;
	
static u08 const M=100, m=4, dp=4;//max min delta-pct [%]
static u08 pct_s=20;//saved lamp value [%]

#define _(x)	static LAMP_STATE x (void)
_(f1) {return pct=pct_s, O1;}//restore, switch on
_(f2) {return pct=pct>M-dp?M:pct+dp, UP;}//dim up
_(f3) {return pct=pct>m+dp?pct-dp:m, DN;}//dim down
_(f4) {return pct_s=pct,pct=0, O;}//save, switch off
_(h ) {return H;} //high intensity
_(h1) {return H1;}//press after ''
_(l ) {return L;} //low intensity
_(l1) {return L1;}//press after ''
#undef _

	/* ---------------------------------------------------------
	public
	--------------------------------------------------------- */
DIMMER_EVENT dimmer_event (u08 button_active)
{	if(!button_active)
		if(btn_cnt&Hbit)
			return btn_cnt&=~Hbit, RELEASE;
		else return btn_cnt=0, NO_DIM_EVENT;
	if(!(btn_cnt&Hbit) )
		return btn_cnt|=Hbit, PRESS;
	if(btn_cnt<(Hbit|REP_WT_) )
		return ++btn_cnt, NO_DIM_EVENT;
	return HOLD;
}

u08 pct;//lamp value [%]
LAMP_STATE (*const DIMMER_AUTOM[LAMP_STATES][DIM_EVENTS])(void)=
{//	state  rel. press hold
	[O ]={ 0,   f1,    0 },
	[O1]={ h,    0,   f2 },
	[UP]={ h,    0,   f2 },
	[H ]={ 0,   h1,    0 },
	[H1]={ f4,   0,   f3 },
	[DN]={ l,    0,   f3 },
	[L ]={ 0,   l1,    0 },
	[L1]={ f4,   0,   f2 },
};

#undef REP_WT_
#undef Hbit
