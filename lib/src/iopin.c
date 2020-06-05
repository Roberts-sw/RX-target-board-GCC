/* iopin.c 
   ------------------------------------------ TAB-size 4, code page UTF-8 --

Wijzigingen:
	RvL 6-6-2020
	* pinmode(), iopin_dir(): PDR-bit '1' is output
	- iopin_read(): ()
	+ iopin_init(): if( !io_init[pin] || ... ) 
	RvL 11-4-2020	* pin state
	RvL 29-3-2020	opname in lib/src
	RvL 17-3-2020	aanmaak
------------------------------------------------------------------------- */
#include "iopin.h"
#define PROTECT	if(_isiopin(pin) ) ; else return
#define PROTECTNMI	if(_isiopin(pin)&&NMIPIN!=pin) ; else return

	/* ---------------------------------------------------------
	private
	--------------------------------------------------------- */
#define NMIPIN				P35

enum en_IO //0|1|2|4 + opt.
{IO_LO=0, IO_HI=1, IO_IN=2,// IO_PE=4,
 IO_ODN=0b00001000,//Open-Drain N-channel: ODRxy='01'
 IO_ODP=0b00010000,//Open-Drain P-channel: ODRxy='10'
 IO_RPU=0b00100000,//R_Pull-Up: PCRx='1'
 IO_CAP=0b01000000,//drive CAPacity control: DSCRx='1'
 IO_SPD=0b10000000,//drive SPeeD control: DSCR2x='1'
};

// #define PINNRa(a,...) a,
// u08 const IOPINNRS[IOPINS]={0,CHIP(PINNRa)};
// #undef PINNRa

#define IOPINb(a,b,...) CH2P_(#b[0])<<3|CH2P_(#b[1]),
u08 const IOPORT[IOPINS] = {0, CHIP(IOPINb)};
#undef  IOPINb
// #define _PORTb(a,b,...)     _PORT(CH2P_(#b[0])),
// volatile struct st_io *const PORTS[IOPINS]={NULL, CHIP(_PORTb)};
// #undef  _PORTb
#define portbit_(pin) (IOPORT[pin]& 7)
#define portnr_( pin) (IOPORT[pin]>>3)
	
#define INITc(a,b,c,...) c,
u08 const IO_MODE[ IOPINS]={IO_IN,CHIP(INITc)};
static u08 io_init[IOPINS];
static u08 io_mode[IOPINS];
#undef  INITc

#define FUNCe(a,b,c,d,e,...) e,
u08 const MPC_FUNC[ IOPINS]={0,CHIP(FUNCe)};
static u08 mpc_func[IOPINS];//Hi-Z
#undef  FUNCe

#define MSTPd(a,b,c,d,...) MSTP_ ## d ## _ID,
u08 const MSTP_ID[IOPINS]={0,CHIP(MSTPd)};
static u08 started[MSTP_IDS-1];
#undef  MSTPd

#define IODIRMASK (IO_LO|IO_HI|IO_IN)
static void pinmode (eIOPIN pin, u08 mode)
{	if(NMIPIN==pin)	return;
	register unsigned int bit=portbit_(pin);
	register unsigned int port=portnr_(pin);

	if(!(mode&IO_IN))						//0b------??
	{	PORTREG(port,PDR)|= (1U<<bit);
		if(!(mode&IO_HI))
			 PORTREG(port,PODR)&=~(1U<<bit);
		else PORTREG(port,PODR)|= (1U<<bit);
	} else PORTREG(port,PDR)&=~(1U<<bit);

	if(!(mode&IO_RPU))						//0b--?-----
		 PORTREG(port,PCR)&=~(1U<<bit);
	else PORTREG(port,PCR)|= (1U<<bit);

	if(!(mode&IO_SPD))						//0b??------
	{	PORTREG(port,DSCR2)&=~(1U<<bit);
		if(!(mode&IO_CAP))
			 PORTREG(port,DSCR)&=~(1U<<bit);
		else PORTREG(port,DSCR)|= (1U<<bit);
	} else PORTREG(port,DSCR2)|= (1U<<bit);	//(DSCR don't care)

	register volatile u08 *const _porto=PORTREG(port,ODR)+bit/4;
	register unsigned int bito = bit*2%8;
	if(!(mode&IO_ODN))						//0b----?---
		 *_porto&=~(1U<<bito+0);
	else *_porto|= (1U<<bito+0);
	if(!(mode&IO_ODP))						//0b---?----
		 *_porto&=~(1U<<bito+1);
	else *_porto|= (1U<<bito+1);
}

static void pin_func (eIOPIN pin, u08 func)
{	if(NMIPIN==pin)	return;
	//HW 23. Multi-Function Pin Controller (MPC)
    //HW 23.4.2 Notes on MPC Register Setting:
    //(1) Settings of PmnPFS should be made while the PMR-bit is set to 0.
	register int bit=portbit_(pin);
	register int port=portnr_(pin);
	if(func)
		PORTREG(port,PMR)&=~(1U<<bit);
	PFS_unlock();
	PmnPFS(port, bit)=func;
	PFS_relock();
	if(func)
		PORTREG(port,PMR)|= (1U<<bit);
	mpc_func[pin]=func;	//1 <-> 2
}

static u08 module (eMSTP_ID id, u08 start)
{	if _is_mstp(id)
	{	--id;//nr -> array-index
#define MSTPrb(a,r,b,...)	((#r)[0]-'A'<<5|(b)),//register: nr + bit
		u08 const ID[MSTP_IDS-1]={MSTP_CTRL(MSTPrb)};
#undef MSTPrb
		register unsigned int bit=ID[id]%32;
		register volatile u32 *mstpcr = &SYSTEM_.MSTPCRA + ID[id]/32;
	
	    SYSTEM_.PRCR = 0xa50b;
		if(start)
		{	if(!started[id]++)//n pins, 1 start
				do  *mstpcr &=~(1U<<bit); 	while( (*mstpcr&(1U<<bit)) );
		}
		else if(started[id])
		{	if(!--started[id])//n pins, 1 stop
				do  *mstpcr |= (1U<<bit);	while(!(*mstpcr&(1U<<bit)) );
		}
		SYSTEM_.PRCR = 0xa500;
		return 1;
	}	return 0;
}

	/* ---------------------------------------------------------
	protected
	--------------------------------------------------------- */
// #define str(s)   #s
// #define PINNAMEb(a,b,...) str(P##b),
// u08 const *const IOPINNAMES[IOPINS]={"",CHIP(PINNAMEb)};
// #undef  PINNAMEb

// #define TARGETf(a,b,c,d,e,f,...) str(f)
// u08 const *const TARGETPINS[IOPINS]={"",CHIP(TARGETf)};
// #undef  TARGETf

// #define OPTIONg(a,b,c,d,e,f,g,...) str(g)
// u08 const *const OPTIONPINS[IOPINS]={"",CHIP(OPTIONg)};
// #undef  OPTIONg

	/* ---------------------------------------------------------
	public
	--------------------------------------------------------- */
void iopin_all_deinit (void)
{	for(eIOPIN pin=0; ++pin<IOPINS; )
		iopin_deinit(pin);
}
void iopin_all_init (void)
{	for(eIOPIN pin=0; ++pin<IOPINS; )
		iopin_init(pin,IO_MODE[pin]);
}

u08 iopin_deinit (eIOPIN pin)
{	PROTECT 0;
	if(!mpc_func[pin])	//1 -> 0
	{	pinmode(pin,IO_IN);
		io_init[pin]=0;
	}	return 1;
}
u08 iopin_init (eIOPIN pin, u08 mode)
{	PROTECT 0;
	if( !io_init[pin] || io_mode[pin]!=mode )
	{	pinmode(pin,mode);
		io_mode[pin]=mode;
	}	return io_init[pin]=1;	//0 -> 1
}

u08 iopin_start (eIOPIN pin, u08 func)
{	PROTECT 0;
	if( io_init[pin]
	&& mpc_func[pin]!=func )
		pin_func(pin,func);
	return 1;
}
u08 iopin_stop (eIOPIN pin)
{	PROTECT 0;
	if( io_init[pin] )
		pin_func(pin,0);
	return 1;
}

void iopin_dir (eIOPIN pin, u08 asoutput)
{	PROTECTNMI;	if(mpc_func[pin]) return;//(tied to peripheral)
	register unsigned int bit=portbit_(pin);
	register unsigned int port=portnr_(pin);
	if(!asoutput)
	{	PORTREG(port,PDR)&=~(1U<<bit);
		io_mode[pin]=io_mode[pin]&~IODIRMASK|IO_IN;
	} else
	{	PORTREG(port,PDR)|= (1U<<bit);
		io_mode[pin]=io_mode[pin]&~IODIRMASK;
	}
}
int iopin_read (eIOPIN pin)
{	PROTECT -1;
	return PORTREG(portnr_(pin),PIDR) >> portbit_(pin)&1U;
}
void iopin_toggle (eIOPIN pin)
{	PROTECTNMI;	if(mpc_func[pin]) return;//(tied to peripheral)
	PORTREG(portnr_(pin),PODR)^= (1U<<portbit_(pin));
	io_mode[pin]^= IO_HI;

}
void iopin_write (eIOPIN pin, u08 ashigh)
{	PROTECTNMI;	if(mpc_func[pin]) return;//(tied to peripheral)
	if(!ashigh)
	{	PORTREG(portnr_(pin),PODR)&=~(1U<<portbit_(pin));
		io_mode[pin]&=~IO_HI;
	} else
	{	PORTREG(portnr_(pin),PODR)|= (1U<<portbit_(pin));
		io_mode[pin]|= IO_HI;
	}
}

u08 module_start (eMSTP_ID id) {return module(id,1);}
u08 module_stop ( eMSTP_ID id) {return module(id,0);}
#undef  IODIRMASK
#undef  portnr_
#undef  portbit_
#undef  NMIPIN
#undef  PROTECTNMI
#undef  PROTECT
