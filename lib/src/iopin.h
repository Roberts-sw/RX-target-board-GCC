#ifndef IOPIN_H_
#define IOPIN_H_
/* iopin.h
   ------------------------------------------ TAB-size 4, code page UTF-8 --
references:
0: https://reference.digilentinc.com/_media/reference/pmod/pmod-interface-specification-1_2_0.pdf
1: https://www.renesas.com/en-eu/doc/products/mpumcu/doc/rx_family/r12um0038ej0100-cloud.pdf
2: https://www.renesas.com/en-eu/doc/products/mpumcu/doc/rx_family/001/r12um0039eg0100-cloud.pdf
3: https://www.macronix.com/Lists/Datasheet/Attachments/7426/MX25L3233F,%203V,%2032Mb,%20v1.6.pdf

wijzigingen:
	RvL 29-3-2020	opname in lib/src
	RvL 18-3-2020	C++-wrapper
	RvL 17-3-2020	aanmaak
------------------------------------------------------------------------- */
#include "rx65x.h"

	//HW 1.5 Pin Assignments
	//pin-interrupts 100-pin PQFP:
	//	15 (P35): NMI
	//	34 (P12): IRQ
	//	41-44 (ext. bus control): -
	//	87-93 95 98 100 (ADC0+DA): IRQ
	//	rest: input capture/output compare
#include "RX65x100_X.h"

	/* ---------------------------------------------------------
	public
	--------------------------------------------------------- */
#define IO_LO	0b00000000	//data : PODRx='0' for low init
#define IO_HI	0b00000001	//data : PODRx='1' for high init
//#define IOOUT	0b00000000	//output : PDRx='1', use IO_LO or IO_HI
#define IO_IN	0b00000010	//input : PDRx='0'
#define IO_PE	0b00000100	//peripheral: PMRx='1'
#define IO_ON	0b00001000	//open-Drain N-channel: ODRxy='01'
#define IO_OP	0b00010000	//open-Drain P-channel: ODRxy='10'
#define IO_PU	0b00100000	//Rpull-up: PCRx='1'
#define IO_CA	0b01000000	//Drive Capacity control: DSCRx='1'
#define IO_SP	0b10000000	//Drive Speed control: DSCR2x='1'

#define ENUM2(a,b,...)		P ## b,
typedef enum en_iopin		{PJ3, CHIP(ENUM2) eIOPINS	} eIOPIN;
#define _isiopin(iopin)		(0<=iopin&&eIOPINS>iopin)	//_isiopin(PJ3) => 1
extern u08 const ICPIN[eIOPINS];						//ICPIN[PJ3] => 4

#ifdef __cplusplus
#	define CCALL	extern "C"	//C-call conventions
#else
#	define CCALL
#endif

	//HW 22 + #define IO_??
CCALL void iopin_dir(eIOPIN iopin, u08 asoutput);
CCALL void iopin_init(eIOPIN iopin, u08 iomode);
CCALL void iopin_mpcfunc(eIOPIN iopin, u08 mpcfunc);//HW 23, p.830-892
CCALL int iopin_read(eIOPIN iopin);
CCALL void iopin_toggle(eIOPIN iopin);
CCALL void iopin_write(eIOPIN iopin, u08 ashigh);

#ifdef __cplusplus
class IoPin
{
public:
	 IoPin (eIOPIN iopin)	: nr(iopin) {}
	~IoPin ()	{}
	void dir (u08 asoutput )	{ iopin_dir(this->nr,asoutput);	}
	void init (u08 mode ) 		{ iopin_init(this->nr,mode);	}
	void mpcfunc (u08 func)		{ iopin_mpcfunc(this->nr,func);	}
	int read ()					{ return iopin_read(this->nr);	}
	void toggle ()				{ iopin_toggle(this->nr);		}
	void write (u08 ashigh)		{ iopin_write(this->nr,ashigh);	}
protected:
private:
	eIOPIN nr;
};

#endif

#undef CCALL

#endif /* IOPIN_H_ */
