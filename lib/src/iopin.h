#ifndef IOPIN_H_
#define IOPIN_H_
/* iopin.h 
   ------------------------------------------ TAB-size 4, code page UTF-8 --

chip: R5F565NEDDFP PLQP-100KB, hardware manual (HW)
	https://www.renesas.com/eu/en/doc/products/mpumcu/doc/rx_family/001/r01uh0590ej0230-rx651.pdf

target board: RTK5RX65N3 Target Board for RX65N, manual (TB)
	https://www.renesas.com/en-eu/doc/products/mpumcu/doc/rx_family/r12um0038ej0100-cloud.pdf

option board: RTK00CLDX0B Cloud Option Board (OB) 
	https://www.renesas.com/en-eu/doc/products/mpumcu/doc/rx_family/001/r12um0039eg0100-cloud.pdf 

PMOD-connector specification (PM)
	https://reference.digilentinc.com/_media/reference/pmod/pmod-interface-specification-1_2_0.pdf

Wijzigingen:
	RvL 11- 4-2020  + implicit pin states: 1=initialised 2=tied to peripheral
	RvL 29-3-2020	opname in lib/src
	RvL 18-3-2020	C++-wrapper
	RvL 17-3-2020	aanmaak
------------------------------------------------------------------------- */
#include "rx65x.h"

	//HW 11. Low Power Consumption
#define MSTP_CTRL(X)\
	/*HW 11.2.2 Module Stop Control Register A (MSTPCRA)*/\
	X(EXDMAC    ,A,29)\
	X(DMADTC    ,A,28)\
/*	X(DMAC      ,A,28)\	X(DTC       ,A,28)*/\
	X(DA        ,A,19)\
	X(S12AD0    ,A,17)\
	X(S12AD1    ,A,16)\
	X(CMT01     ,A,15)\
/*	X(CMT0      ,A,15)\	X(CMT1      ,A,15)*/\
	X(CMT23     ,A,14)\
/*	X(CMT2      ,A,14)\	X(CMT3      ,A,14)*/\
	X(TPU       ,A,13)\
/*	X(TPU0      ,A,13)\	X(TPU1      ,A,13)\	X(TPU2      ,A,13)\
	X(TPU3      ,A,13)\	X(TPU4      ,A,13)\	X(TPU5      ,A,13)*/\
	X(PPG0      ,A,11)\
	X(PPG1      ,A,10)\
	X(MTU       ,A, 9)\
/*	X(MTU0      ,A, 9)\	X(MTU1      ,A, 9)\	X(MTU2      ,A, 9)\
	X(MTU3      ,A, 9)\	X(MTU4      ,A, 9)\	X(MTU5      ,A, 9)*/\
	X(TMR01     ,A, 5)\
/*	X(TMR0      ,A, 5)\	X(TMR1      ,A, 5)*/\
	X(TMR23     ,A, 4)\
	X(CMTW0     ,A, 1)\
	X(CMTW1     ,A, 0)\
	/*HW 11.2.3 Module Stop Control Register B (MSTPCRB)*/\
	X(SCI0      ,B,31)\
	X(SCI1      ,B,30)\
	X(SCI2      ,B,29)\
	X(SCI3      ,B,28)\
	X(SCI4      ,B,27)\
	X(SCI5      ,B,26)\
	X(SCI6      ,B,25)\
	X(SCI7      ,B,24)\
	X(CRC       ,B,23)\
	X(PDC       ,B,22)\
	X(RIIC0     ,B,21)\
	X(RIIC1     ,B,20)\
	X(USB0      ,B,19)\
	X(RSPI0     ,B,17)\
	X(RSPI1     ,B,16)\
	X(EEDMAC0   ,B,15)\
	X(ELC       ,B, 9)\
	X(TEMPS     ,B, 8)\
	X(DOC       ,B, 6)\
	X(SCI12     ,B, 4)\
	X(CAN0      ,B, 1)\
	X(CAN1      ,B, 0)\
	/*HW 11.2.4 Module Stop Control Register C (MSTPCRC)*/\
	X(GLCDC     ,C,29)\
	X(DRW2D     ,C,28)\
	X(SCI8      ,C,27)\
	X(SCI9      ,C,26)\
	X(SCI10     ,C,25)\
	X(SCI11     ,C,24)\
	X(QSPI      ,C,23)\
	X(RSPI2     ,C,22)\
	X(CAC       ,C,19)\
	X(RIIC2     ,C,17)\
	X(STBYRAM   ,C, 7)\
	X(EXPRAM    ,C, 2)\
	X(RAM       ,C, 0)\
	/*HW 11.2.5 Module Stop Control Register D (MSTPCRD)*/\
	X(TSIP      ,D,27)\
	X(MMCIF     ,D,21)\
	X(SDHI      ,D,19)\
	X(SDSI      ,D,13)\
//    module   reg bit
#define ENUMa(a,b,c,...) MSTP_ ## a ## _ID,

	//HW 1.5 Pin Assignments
	//pin-interrupts 100-pin PQFP:
	//	15 (P35): NMI
	//	34 (P12): IRQ
	//	41-44 (ext. bus control): -
	//	87-93 95 98 100 (ADC0+DA): IRQ
	//	rest: input capture/output compare
#include "RX65x100_X.h"
#define ENUMb(a,b,c,...) P ## b,

	/* ---------------------------------------------------------
	public
	--------------------------------------------------------- */
//enum en_IO //0|1|2|4 + opt.
//{IO_LO=0, IO_HI=1, IO_IN=2,// IO_PE=4,
// IO_ODN=0b00001000,//Open-Drain N-channel: ODRxy='01'
// IO_ODP=0b00010000,//Open-Drain P-channel: ODRxy='10'
// IO_RPU=0b00100000,//R_Pull-Up: PCRx='1'
// IO_CAP=0b01000000,//drive CAPacity control: DSCRx='1'
// IO_SPD=0b10000000,//drive SPeeD control: DSCR2x='1'
//};

typedef enum en_mstp_id {MSTP_NO_ID, MSTP_CTRL(ENUMa) MSTP_IDS} eMSTP_ID;
#define _is_mstp(id)    (0<id && id<MSTP_IDS)

typedef enum en_iopin   {NO_IOPIN, CHIP(ENUMb) IOPINS,} eIOPIN;
#define _isiopin(pin)   (0<pin && pin<IOPINS)

extern u08 const IO_MODE[ IOPINS];	//pindir etc.
extern u08 const MPC_FUNC[IOPINS];	//module to attach pin to
extern u08 const MSTP_ID[ IOPINS];	//module to start before attaching pin

#ifdef __cplusplus
#	define CCALL	extern "C"	//C-call conventions
#else
#	define CCALL
#endif

	//HW 22 + #define IO_??
CCALL void iopin_all_deinit(void);
CCALL void iopin_all_init(void);

CCALL u08 iopin_deinit(eIOPIN pin);
CCALL u08 iopin_init(eIOPIN pin, u08 mode);

CCALL u08 iopin_start(eIOPIN pin, u08 func);	//HW 23, p.830-892
CCALL u08 iopin_stop(eIOPIN pin);

CCALL void iopin_dir(eIOPIN pin, u08 asoutput);
CCALL int iopin_read(eIOPIN pin);
CCALL void iopin_toggle(eIOPIN pin);
CCALL void iopin_write(eIOPIN pin, u08 ashigh);

CCALL u08 module_start(eMSTP_ID id);
CCALL u08 module_stop( eMSTP_ID id);

#ifdef __cplusplus
class IoPin
{
public:
	 IoPin (eIOPIN iopin) : nr(iopin) {}
	~IoPin () {}

	void deinit ()          {iopin_deinit(this->nr);       }
	void init (u08 mode)    {iopin_init( this->nr, mode);  }
	void start (u08 func)   {iopin_start(this->nr, func);  }
	void stop (u08 func)    {iopin_stop( this->nr);        }

	void dir (u08 asoutput) {iopin_dir(this->nr, asoutput);}
	int read ()             {return iopin_read(this->nr);  }
	void toggle ()          {iopin_toggle(this->nr);       }
	void write (u08 ashigh) {iopin_write(this->nr, ashigh);}
protected:
private:
	eIOPIN nr;
};

#endif

#	undef CCALL

#endif /* IOPIN_H_ */
