/* RTK5RX65Ndemo12 > src > main.c
   ------------------------------------------ TAB-size 4, code page UTF-8 --

functies:
- ICLK 120 MHz, PCLKA 120 MHz, PCLKB 60 MHz
- SCI5: output string @115k2N81, repeat per second

test:
- board: RTK5RX65N3 Target Board for RX65N, manual R20UT4167EJ0101 (TB)
  board: RTK00CLDX0B Cloud Option Board, manual R12UM0039EG0100 (OB)
- chip: R5F565NEDDFP PLQP-100KB, hardware manual R01UH0590EJ0230 (HW)
- compiler: GCC for Renesas RX 8.3.0.201904
- IDE: Renesas e2-studio 7.7.0

wijzigingen:
    RvL 2-6-2020    micros()
    RvL 12-4-2020    test
------------------------------------------------------------------------- */
//#include <iodefine.h>
#include "iopin.h"
#include "Cserial.h"

#define OPTION_BOARD//comment for target board
#define TICK_CMT0//
#ifdef TICK_CMT0
#	define TICK_init	CMT0_init
#	define TICK_start	CMT0_start
#	define ticks()		CMT0_.CMCNT
#endif
u32 clockb;
Shwserial *pc=&Serial5;
char message[] ="Hello world! \r\nThis is me with a USB-serial converter.\r\n";
u32 micros(void);
u32 millis(void);

#ifndef OPTION_BOARD
//TB 5. User Circuit, p.12
#	define SW1  U08_BIT(IO_._PIDR[0xB]).b1	//PB1
#	define LED0 U08_BIT(IO_._PODR[0xD]).b6	//PD6
#	define LED1 U08_BIT(IO_._PODR[0xD]).b7	//PD7
#else
//OB 4. Cloud Option Board Circuit Description, p.11
//TB 5.5 User LED, p.12
#	define SW1  U08_BIT(IO_._PIDR[0x3]).b1	//P31
#	define LED1 U08_BIT(IO_._PODR[0xB]).b0	//PB0
#	define LED2 PORTBIT(CH2P_('B'),PODR,2)	//PB2
#endif

void init_peripherals (void)
{   //PB2 PB0 PD7 PD6 output
    iopin_alldirs(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                  0x00,0x00,0x00,0x05,0x00,0xc0,0x00,0x00,
                  0x00,0x00);
    TICK_init();	//1875 kHz count, 1kHz interrupt
    Cserial_init_uart(pc,clockb);
}
void start_peripherals (void)
{	TICK_start();
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

int c;
int main(void)
{	u16 ms=500;
	SYSTEM_.PRCR = 0xa50b;
	HOCO_PLL_120MHz();	clockb=60*1000*1000;//init_uart(..., clockb)
	init_peripherals();
	SYSTEM_.PRCR = 0xa500;

	for(start_peripherals(); ; )
    {	if(0<=(s16)(millis()-ms))
		{	ms+=500;
			if( LED1^=1 )
			{	LED2^=1;
				for(char *s=message; *s; )
					if( Cserial_write( pc, *s ) )
						s++;
			}
		}
		//echo typed characters:
		if(c=Cserial_read(pc), 0<=c)
			Cserial_write(pc,c);
    }
return 0;
}

/* ---------------------------------------------------------
private
--------------------------------------------------------- */
static u32 volatile Micros=0, Millis=0;

/* ---------------------------------------------------------
protected
--------------------------------------------------------- */
#include "interrupt_handlers.h"
u32 micros (void) {return Micros+(ticks()*34953>>16);}//(x / 1,875)
u32 millis (void) {return Millis;}

void INT_Excep_CMT0_CMI0(void) {Micros+=1000, ++Millis;}

void INT_Excep_SCI5_RXI5(void) {Cserial_rx_irq(pc);}
void INT_Excep_SCI5_TXI5(void) {Cserial_tx_irq(pc);}

#define BL0status U32_BIT(ICU_.GRPBL0)
void INT_Excep_ICU_GROUPBL0(void)
{	//SCI0..3: TEI-, ERI-
//	if(BL0status.b0 )	Cserial_te_irq(&Serial0 );
//	if(BL0status.b1 )	Cserial_er_irq(&Serial0 );
//	if(BL0status.b2 )	Cserial_te_irq(&Serial1 );
//	if(BL0status.b3 )	Cserial_er_irq(&Serial1 );
//	if(BL0status.b4 )	Cserial_te_irq(&Serial2 );
//	if(BL0status.b5 )	Cserial_er_irq(&Serial2 );
//	if(BL0status.b6 )	Cserial_te_irq(&Serial3 );
//	if(BL0status.b7 )	Cserial_er_irq(&Serial3 );
	//SCI4..7: TEI-, ERI-
//	if(BL0status.b8 )	Cserial_te_irq(&Serial4 );
//	if(BL0status.b9 )	Cserial_er_irq(&Serial4 );
	if(BL0status.b10)	Cserial_te_irq(&Serial5 );
	if(BL0status.b11)	Cserial_er_irq(&Serial5 );
//	if(BL0status.b12)	Cserial_te_irq(&Serial6 );
//	if(BL0status.b13)	Cserial_er_irq(&Serial6 );
//	if(BL0status.b14)	Cserial_te_irq(&Serial7 );
//	if(BL0status.b15)	Cserial_er_irq(&Serial7 );
	//SCI12: TEI-, ERI-
//	if(BL0status.b16)	Cserial_te_irq(&Serial12);
//	if(BL0status.b17)	Cserial_er_irq(&Serial12);
}
#undef BL0status

#define BL1status U32_BIT(ICU_.GRPBL1)
void INT_Excep_ICU_GROUPBL1(void)
{	//SCI8..9: TEI-, ERI-
//	if(BL1status.b24)	Cserial_te_irq(&Serial8 );
//	if(BL1status.b25)	Cserial_er_irq(&Serial8 );
//	if(BL1status.b26)	Cserial_te_irq(&Serial9 );
//	if(BL1status.b27)	Cserial_er_irq(&Serial9 );
}
#undef BL1status

#define AL0status U32_BIT(ICU_.GRPAL0)
void INT_Excep_ICU_GROUPAL0(void)
{	//SCI10..11: TEI-, ERI-
//	if(AL0status.b8 )	Cserial_te_irq(&Serial10);
//	if(AL0status.b9 )	Cserial_er_irq(&Serial10);
//	if(AL0status.b10)	Cserial_te_irq(&Serial11);
//	if(AL0status.b11)	Cserial_er_irq(&Serial11);
}
#undef AL0status
