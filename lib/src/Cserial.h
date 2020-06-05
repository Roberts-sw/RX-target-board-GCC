#ifndef CSERIAL_H_
#define CSERIAL_H_
/* Cserial.h
   ------------------------------------------ TAB-size 4, code page UTF-8 --

test:
- board: RTK5RX65N3 Target Board for RX65N, manual R20UT4167EJ0101 (TB)
- chip: R5F565NEDDFP PLQP-100KB, hardware manual R01UH0590EJ0230 (HW)
- compiler: GCC for Renesas RX 8.3.0.201904
- IDE: Renesas e2-studio 7.7.0

changes:
	RvL  11-4-2020	concept
------------------------------------------------------------------------- */
#include "iopin.h"

	/* ---------------------------------------------------------
	public
	--------------------------------------------------------- */
//#ifndef JOIN
//#  define JOIN(a,b)   JOIN_2(a,b)
//#  define JOIN_2(a,b) a ## b
//#endif

	//fifo
#define diff_(b)		( JOIN(b,_in) - JOIN(b,_out) )
#define peek_(b)		( diff_(b) ? JOIN(b,_buf)[ JOIN(b,_out) ] : -1 )
#define used_(b,sz)		( ((unsigned)((sz)+diff_(b)) )%(sz) )
#define unused_(b,sz)	( sz - 1 - used_(b,sz) )
#define get_(b,sz, r /*resultaat*/)\
	do r=JOIN(b,_buf)[JOIN(b,_out)], JOIN(b,_out)=(JOIN(b,_out)+1)%sz; while(0)
#define put_(b,sz, a /*argument*/)\
	do{ unsigned int t_=(JOIN(b,_in)+1)%sz;\
	if(t_!=JOIN(b,_out)) JOIN(b,_buf)[JOIN(b,_in)]=a, JOIN(b,_in)=t_; }while(0)

typedef struct st_hwserial Shwserial;

extern Shwserial Serial0, Serial1, Serial2, Serial3, Serial4,
	    Serial5, Serial6, Serial7, Serial8, Serial9;//SCIg
extern Shwserial Serial12;//SCIh, PCLKB
extern Shwserial Serial10, Serial11;//SCIi => PCLKA

u08 search4(u08 const *_str, u08 ch);//search in string of 4 chars: 0..3=found

void Cserial_init_uart(Shwserial *self, u32 pclk);

int Cserial_read(Shwserial *self);
u08 Cserial_write(Shwserial *self, u08 c);

	/* ---------------------------------------------------------
	protected
	--------------------------------------------------------- */
extern void Cserial_rx_irq(Shwserial *self);//vec 2n+0
extern void Cserial_tx_irq(Shwserial *self);//vec 2n+1
extern void Cserial_te_irq(Shwserial *self);//grp 2n+0
extern void Cserial_er_irq(Shwserial *self);//grp 2n+1

#endif /* CSERIAL_H_ */
