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
	RvL 4-4-2020	test Little-Endian data (+ lib setting!)
	RvL 3-4-2020	test Big-Endian data
------------------------------------------------------------------------- */
#include "dimmer.h"

	//TB 5.11 User Switch, p.15
#define SW1			U08_BIT(IO_._PIDR[0xB]).B1//iopin_read(PB1)
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
	MSTP_MTU_=0;				//release MTU from stop
//	MTU_CST8=0;					//C1280.3:0	stop TCNT8
	ICU_.SLIAR247=42;			//SLIAR247 source 42 (TGIB8)
	ICU_.IPR[247]=15;			//+ priority
	MTU8_.TCR=0<<5|1<<3|2<<0;	//CLR_NONE|CNT_RISE|CKA_16 => 7,5MHz count
	MTU8_.TIER=1<<1;			//enable MTU8-TGIEB
	MTU8_.TIORH=0<<4|3<<0;		//MATB_NO, MATA_LT: '0'=on, toggle to off
	MTU8_.TGRB=PWM_PERIOD-1;	//period=10.000 count => 750Hz
	MTU8_.TGRA=PWM_PERIOD/2-1;	//on-time 50,00%
	MPC_.PFS[8*0xD+6]=8;		//MPC pin func PD6=MTIOC8A
	IO_._PMR[0xD]|=1<<6;		//mode register D: peripheral on bit 6

//LED1 PD7 is MTIC5U-pin
	//MTU5 has neither output compare nor pwm possibility, see HW p.894
	//=> use TGIU5-interrupt for duty cycle and for period

	//TGIU5 is SLIA-source 27, sources 0 26 are reserved,
	//	so choose offset of 25 from SLIAR208, will be SLIAR233
//	MTU5_.TSTR=0;				//C1CB4:00
	ICU_.SLIAR233=27;			//SLIAR233 source 27 (TGIU5)
	ICU_.IPR[233]=15;			//+ priority
	MTU5_.TIER=1<<2;			//enable TGIU5-interrupt
	MTU5_.TCRU=2;				//PCLKA/16 = 7,5MHz
	MTU5_.TCNTCMPCLR=1<<2;		//MAT_U clear
	MTU5_.TGRU=PWM_PERIOD;		//period=10.000 count => 750Hz
}

void start_peripherals (void)
{   ICU_.IER[247/8]|= 1<<247%8;	//enable SLIAR247-interrupt
    MTU_CST8=1;					//start TCNT8
    ICU_.IER[233/8]|= 1<<233%8;	//enable SLIAR233-interrupt
    MTU5_.TSTR=1<<2;			//start TCNTU5
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
void dimmer_do (void)
{	DIMMER_EVENT ev=dimmer_event( sample(SW1) );
	if(NO_DIM_EVENT==ev)
		return;

	static LAMP_STATE st=O;
	dimmer_action act=DIMMER_AUTOM[st][ev];
	if(!act)
		return;
	st=act();

	pwm=pct*pct;//gamma 2,00
}

volatile u08 flag_100ms, cnt_750Hz;
int main(void)
{	SYSTEM_.PRCR = 0xa50b;
	HOCO_PLL_120MHz();
	PFS_unlock();
	init_peripherals();
	PFS_relock();
	SYSTEM_.PRCR = 0xa500;

    for(start_peripherals(); ; )
    	if( flag_100ms )
    	{	flag_100ms=0;
    		dimmer_do();
    	}
return 0;
}

	/* ---------------------------------------------------------
	protected
	--------------------------------------------------------- */
#include "interrupt_handlers.h"

void INT_Excep_PERIA_INTA233(void)
{//	extern u16 pwm;
#define LED1 U08_BIT(IO_._PODR[0xD]).B7//#define LED1 PD7
	if(!LED1)//if(!iopin_read(LED1) )//LED is on
	{	if(pwm<PWM_PERIOD)
		{	LED1=1;//iopin_toggle(LED1);//put LED off
			goto period;
		}
		if(MTU5_.TGRU!=PWM_PERIOD)
period:	{	MTU5_.TGRU=PWM_PERIOD;
			MTU5_.TCNTCMPCLR|=0x04;//counter clear on CMIU5
		}
	} else if(pwm)
		{	LED1=0;//iopin_toggle(LED1);//put LED on
			MTU5_.TGRU=pwm;
			MTU5_.TCNTCMPCLR&=~0x04;//no clear on CMIU5
		}
}

void INT_Excep_PERIA_INTA247(void)
{	MTU_CST8=0;
	MTU8_.TCNT=0x00000000;			//reset TCNT8
	if(pwm)
	{	MTU8_.TGRA=pwm-1;			//off after match
		MTU8_.TIORH=0<<4|3<<0;		//MATA_LT => start as on
	} else
		MTU8_.TIORH=0<<4|6<<0;		//MATA_HH => '1'=off
	MTU_CST8=1;

	if(75<=++cnt_750Hz)
		cnt_750Hz=0, flag_100ms=1;
}
