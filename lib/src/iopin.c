/* iopin.c
   ------------------------------------------ TAB-size 4, code page UTF-8 --

wijzigingen:
	RvL 29-3-2020	opname in lib/src
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

#define ICPIN1(a,...)			a,
#define IOPIN2(a,b,...)			0x ## b,
#define IOMODE3(a,b,c,...)		c,
#define NMIPIN					P35

//static u08 iomode[eIOPINS]= {IO_LO, CHIP(IOMODE3)};	//in application

	/* ---------------------------------------------------------
	protected
	--------------------------------------------------------- */
u08 const IOPORT[eIOPINS] = {0, CHIP(IOPIN2) };			//PJ3 special
//u08 const IOINIT[eIOPINS] = {IO_LO, CHIP(IOMODE3)}; 	//use for init
//char const HEX[] = "0123456789ABCDEF";
//char const *iopin_name (eIOPIN iopin)
//{	static char str[4];
//	if(_isiopin(iopin) )
//	{	str[0]='P';
//		str[1]=!iopin?'J':HEX[portnr_(iopin)];
//		str[2]=HEX[portbit_(iopin)];
//	} else str[0]=str[1]=str[2]='?';
//	str[3]='\0';
//	return (char const *)str;
//}
	/* ---------------------------------------------------------
	public
	--------------------------------------------------------- */
u08 const ICPIN[eIOPINS]  = {4, CHIP(ICPIN1)};			//pin 4 is PJ3

void iopin_dir (eIOPIN iopin, u08 asoutput)
{	PROTECT;
	if(!asoutput||NMIPIN==iopin)	//input
		IO_._PDR[portnr_(iopin)]&=~(1<<portbit_(iopin) );
	else IO_._PDR[portnr_(iopin)]|=(1<<portbit_(iopin) );
}
void iopin_init (eIOPIN iopin, u08 iomode)
{	PROTECT;
	register int mask=1<<portbit_(iopin);
	register int port=portnr_(iopin);

	if(!(iomode&IO_IN)&&NMIPIN!=iopin)	//0b------??
	{	if(IO_._PDR[port]|= mask, !(iomode&IO_HI) )
			IO_._PODR[port]&=~mask;
		else IO_._PODR[port]|=mask;
	} else IO_._PDR[port]&=~mask;

	if(!(iomode&IO_PE) )				//0b-----?--
		IO_._PMR[port]&=~mask;
	else IO_._PMR[port]|=mask;

	if(!(iomode&IO_PU) )				//0b--?-----
		IO_._PCR[port]&=~mask;
	else IO_._PCR[port]|=mask;

	if(!(iomode&IO_SP) )				//0b??------
	{	if(IO_._DSCR2[port]&=~mask, !(iomode&IO_CA) )
			IO_._DSCR[port]&=~mask;
		else IO_._DSCR[port]|=mask;
	} else IO_._DSCR2[port]|=mask;		//DSCR-setting don't care

	mask=portbit_(iopin)%4*2;			//0 2 4 6
	iomode=(iomode>>3&3)<<mask;			//0b---??---
	mask=3<<mask;
	if(portbit_(iopin)<=3)
		IO_._ODR[port]._0=IO_._ODR[port]._0 & ~mask | iomode;
	else
		IO_._ODR[port]._1=IO_._ODR[port]._1 & ~mask | iomode;
}
	//HW 23. Multi-Function Pin Controller (MPC)
void iopin_mpcfunc (eIOPIN iopin, u08 mpcfunc)
{	PROTECT;
	if(NMIPIN==iopin)	return;
	PFS_unlock();//PWPR_&=~0x80; PWPR_|=0x40;	//PmnPFS_writes_deblock();
//#	define _PFS  ( (volatile u08 *const)0x0008c140)//array
//	_PFS[8*portnr_(iopin)+portbit_(iopin)]=mpcfunc;
//#	undef _PFS
//	MPC_.PFS[8*portnr_(iopin)+portbit_(iopin)]=mpcfunc;
	PmnPFS(portnr_(iopin), portbit_(iopin))=mpcfunc;
	PFS_relock();//PWPR_&=~0x40; PWPR_|=0x80;	//PmnPFS_writes_block();
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
void iopin_write (eIOPIN iopin, u08 ashigh)
{	PROTECT;
	if(!ashigh||NMIPIN==iopin)	//low or input-only
		IO_._PODR[portnr_(iopin)]&=~(1<<portbit_(iopin) );
	else IO_._PODR[portnr_(iopin)]|=(1<<portbit_(iopin) );
}

#undef NMIPIN
#undef IOMODE3
#undef IOPIN2
#undef ICPIN1

#undef portnr_
#undef portbit_
#undef PROTECT
