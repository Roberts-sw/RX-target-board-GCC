/* RTK5RX65Ndemo10 > src > main.c
   ------------------------------------------ TAB-size 4, code page UTF-8 --

functies:
- ICLK 120 MHz, PCLKA 120 MHz, PCLKB 60 MHz
- LED0: TGIB8 output compare, restart at period interrupt (no pwm!)
- LED1: TGIU5 compare match, drive pin, change match (no output function!)

test:
- board: RTK5RX65N3 Target Board for RX65N, manual R20UT4167EJ0101 (TB)
- chip: R5F565NEDDFP PLQP-100KB, hardware manual R01UH0590EJ0230 (HW)
- compiler: GCC for Renesas RX 8.3.0.201904
- IDE: Renesas e2-studio 7.7.0

wijzigingen:
	RvL 28-3-2020	aanmaak
------------------------------------------------------------------------- */
#include <iodefine.h>
#include "dimmer.h"
//#include "rx65x.h"

	//TB 5.11 User Switch, p.15
#define SW1			iopin_read(PB1)
#define sample(b)	!(b)

	//HW 22. I/O Ports
#define PWM_PERIOD	(100*100)	//100,00 [%]
void init_peripherals (void)
{	//PD7 output
	iopin_alldirs(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				  0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,
				  0x00,0x00);

//LED0 PD6 is MTU8A-pin
	//MTU8 has input capture and output compare possibility, see HW p.894
	//output compare without pwm is one-shot, see HW p.980 !
	//=> use TGIB8-interrupt to restart period with one-shot output

	//TGIB8 is SLIA-source 42, sources 0 26 40 are reserved,
	//	so choose offset of 39 from SLIAR208, will be SLIAR247
	MSTP(MTU8)=0;					//release MTU from stop
	MTU.TSTRA.BIT.CST8=0;			//stop TCNT8
	ICU.SLIAR247.BYTE=42;			//SLIAR247 source 42 (TGIB8)
	IPR(PERIA, INTA247)=15;			//+ interrupt priority
	MTU8.TCR.BYTE=0<<5|1<<3|2<<0;	//CLR_NONE|CNT_RISE|CKA_16 => 7,5MHz count
//	MTU8.TCR2.BYTE=0;				//default => TCR determines CKA div factor
	MTU8.TIER.BYTE=1<<1;			//enable MTU8-TGIEB
	MTU8.TIORH.BYTE=0<<4|3<<0;		//MATB_NO, MATA_LT: '0'=on, toggle to off
//	MTU8.TIORL.BYTE=0<<4|0<<0;		//MATD_NO, MATC_NO
	MTU8.TGRB=PWM_PERIOD-1;			//period=10.000 count => 750Hz
	MTU8.TGRA=PWM_PERIOD/2-1;//~0L;	//on-time 50,00%
//	MTU8.TGRC=~0L;					//not used
//	MTU8.TGRD=~0L;					//not used
	MPC.PD6PFS.BYTE=8;				//PD6=MTIOC8A
	PORTD.PMR.BYTE|=0x40;			//... peripheral function

//LED1 PD7 is MTIC5U-pin
	//MTU5 has neither output compare nor pwm possibility, see HW p.894
	//=> use TGIU5-interrupt for duty cycle and for period

	//TGIU5 is SLIA-source 27, sources 0 26 are reserved,
	//	so choose offset of 25 from SLIAR208, will be SLIAR233
//	MSTP(MTU5)=0;					//release MTU from stop, done above
	MTU5.TSTR.BYTE=0;				//stop MTU5 counters
	ICU.SLIAR233.BYTE=27;			//SLIAR233 source 27 (TGIU5)
	IPR(PERIA, INTA233)=15;			//+ interrupt priority
	MTU5.TIER.BYTE=4;//1<<2;			//enable TGIU5-interrupt
	MTU5.TCRU.BYTE=2;				//PCLKA/16 = 7,5MHz
//	MTU5.TCRV.BYTE=0;
//	MTU5.TCRW.BYTE=0;
//	MTU5.TCR2U.BYTE=0;
//	MTU5.TCR2V.BYTE=0;
//	MTU5.TCR2W.BYTE=0;
	MTU5.TCNTCMPCLR.BYTE=4;//1<<2;		//MAT_U clear
//	MTU5.TIORU.BYTE=0;
//	MTU5.TIORV.BYTE=0;
//	MTU5.TIORW.BYTE=0;
	MTU5.TGRU=PWM_PERIOD;			//period=10.000 count => 750Hz
//	MTU5.TGRV=~0;
//	MTU5.TGRW=~0;
}

void start_peripherals (void)
{	IEN(PERIA, INTA247)=1;			//enable SLIAR247-interrupt
	MTU.TSTRA.BIT.CST8=1;			//start TCNT8

	IEN(PERIA, INTA233) = 1U;		//enable SLIAR233-interrupt
    MTU5.TSTR.BYTE=1<<2;//7;//		//start TCNTU5(+TCNTV5+TCNTW5)
}

#ifdef CPPAPP
//Initialize global constructors
extern void __main()
{
  static int initialized;
  if (! initialized)
    {
      typedef void (*pfunc) ();
      extern pfunc __ctors[];
      extern pfunc __ctors_end[];
      pfunc *p;

      initialized = 1;
      for (p = __ctors_end; p > __ctors; )
    (*--p) ();

    }
}
#endif 

u16 pwm;
volatile u08 flag_100ms, cnt_750Hz;
void dimmer_run (void)
{	//sample timing:
	if( flag_100ms )
		flag_100ms=0;
	else return;

	DIMMER_EVENT ev=dimmer_event( sample(SW1) );
	if(NO_DIM_EVENT==ev)
		return;

	static LAMP_STATE st=O;
	dimmer_action act=DIMMER_AUTOM[st][ev];
	if(!act)
		return;
	st=act();

	pwm=pct*pct;//gamma 2,00
}

int main(void)
{	SYSTEM_.PRCR = 0xa50b;
	HOCO_PLL_120MHz();
	PFS_unlock();
	init_peripherals();
	PFS_relock();
	SYSTEM_.PRCR = 0xa500;

    start_peripherals();

    do	dimmer_run();
    while(1);
return 0;
}
