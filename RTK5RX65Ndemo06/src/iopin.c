/* iopin.c
   ------------------------------------------ TAB-size 4, code page UTF-8 --

wijzigingen:
	RvL 17-3-2020	aanmaak
------------------------------------------------------------------------- */
#include "iopin.h"

	/* ---------------------------------------------------------
	private
	--------------------------------------------------------- */
#define PROTECT	if(_isiopin(iopin) ) ; else return//if(1) ; else return//
	//I in port names was skipped, so PORTJ is the 9th after PORT9:
#define portbit_(iopin)		(!iopin ? 3 : IOPORT[iopin]& 7)
#define portnr_(iopin)		(!iopin ? 18: IOPORT[iopin]>>4)

#define IOPIN2(a,b,...)			0x ## b,
#define IOMODE3(a,b,c,...)		c,
#define IOFUN4(a,b,c,d,...)		#d,
#define ICPIN1(a,...)			a,
#define NMIPIN					P35

//static uint8_t iomode[eIOPINS]= {IO_LO, CHIP(IOMODE3)};	//in application

	/* ---------------------------------------------------------
	protected
	--------------------------------------------------------- */
uint8_t const IOPORT[eIOPINS] = {0, CHIP(IOPIN2) };			//PJ3 special
//uint8_t const IOINIT[eIOPINS] = {IO_LO, CHIP(IOMODE3)}; 	//use for init
//char const *const IOFUNC[eIOPINS]={"-", CHIP(IOFUN4)};
//char const HEX[] = "0123456789ABCDEF";

	/* ---------------------------------------------------------
	public
	--------------------------------------------------------- */
uint8_t const ICPIN[eIOPINS]  = {4, CHIP(ICPIN1)};			//pin 4 is PJ3

void iopin_dir (eIOPIN iopin, uint8_t asoutput)
{	PROTECT;
	if(!asoutput||NMIPIN==iopin)	//input
		IO_._PDR[portnr_(iopin)]&=~(1<<portbit_(iopin) );
	else IO_._PDR[portnr_(iopin)]|=(1<<portbit_(iopin) );
}
void iopin_init (eIOPIN iopin, uint8_t iomode)
{	PROTECT;
	register int mask=1<<portbit_(iopin);
	register int port=portnr_(iopin);

	if(!(iomode&IO_IN)&&NMIPIN!=iopin)	//0bxxxxxx??
	{	if(IO_._PDR[port]|= mask, !(iomode&IO_HI) )
			IO_._PODR[port]&=~mask;
		else IO_._PODR[port]|=mask;
	} else IO_._PDR[port]&=~mask;

	if(!(iomode&IO_PE) )			//0bxxxxx?xx
		IO_._PMR[port]&=~mask;
	else IO_._PMR[port]|=mask;

	if(!(iomode&IO_PU) )			//0bxx?xxxxx
		IO_._PCR[port]&=~mask;
	else IO_._PCR[port]|=mask;

	if(!(iomode&IO_SP) )			//0b??xxxxxx
	{	if(IO_._DSCR2[port]&=~mask, !(iomode&IO_CA) )
			IO_._DSCR[port]&=~mask;
		else IO_._DSCR[port]|=mask;
	} else IO_._DSCR2[port]|=mask;	//DSCR-setting don't care

	mask=portbit_(iopin)%4*2;		//0 2 4 6
	iomode=(iomode>>3&3)<<mask;		//0bxxx??xxx
	mask=3<<mask;
	if(portbit_(iopin)<=3)
		IO_._ODR[port]._0=IO_._ODR[port]._0 & ~mask | iomode;
	else
		IO_._ODR[port]._1=IO_._ODR[port]._1 & ~mask | iomode;
}
	//HW 23. Multi-Function Pin Controller (MPC)
void iopin_mpcfunc (eIOPIN iopin, uint8_t mpcfunc)
{	PROTECT;
//	if(NMIPIN==iopin)	return;
#	define PWPR_ (*(volatile uint8_t * const)0x0008c11f)//register
	PWPR_&=~0x80; PWPR_|=0x40;	//PmnPFS_writes_deblock();
#	define _PFS  ( (volatile uint8_t * const)0x0008c140)//array
	_PFS[8*portnr_(iopin)+portbit_(iopin)]=mpcfunc;
#	undef _PFS
	PWPR_&=~0x40; PWPR_|=0x80;	//PmnPFS_writes_block();
#	undef PWPR_
}
int iopin_read (eIOPIN iopin)
{	PROTECT -1;
	return IO_._PIDR[portnr_(iopin)] >> portbit_(iopin) & 1;
}
void iopin_toggle (eIOPIN iopin)
{	PROTECT;
	if(NMIPIN!=iopin)
		IO_._PODR[portnr_(iopin)]^= (1<<portbit_(iopin) );
}
void iopin_write (eIOPIN iopin, uint8_t ashigh)
{	PROTECT;
	if(!ashigh||NMIPIN==iopin)	//low or input-only
		IO_._PODR[portnr_(iopin)]&=~(1<<portbit_(iopin) );
	else IO_._PODR[portnr_(iopin)]|=(1<<portbit_(iopin) );
}

#undef NMIPIN
#undef ICPIN1
#undef IOFUN4
#undef IOMODE3
#undef IOPIN2

#undef portnr_
#undef portbit_
#undef PROTECT
