/* RTK5RX65Ndemo05 > src > main.c
   ------------------------------------------ TAB-size 4, code page UTF-8 --

functies:
- HOCO clock 16 MHz, PLL / 1 * 15 = 240 MHz
- ICLK / 2 = 120 MHz, PCLKB / 4 = 60 MHz
- ROMWT=2
- LED0 on, LED1 initially off
- IRQ4 (SW1-press) toggles LED0, (SW1-release) toggles LED1

test:
- board: RTK5RX65N3 Target Board for RX65N, manual R20UT4167EJ0101 (TB)
- chip: R5F565NEDDFP PLQP-100KB, hardware manual R01UH0590EJ0230 (HW)
- compiler: GCC for Renesas RX 8.3.0.201904
- IDE: Renesas e2-studio 7.7.0

wijzigingen:
	RvL 16-3-2020	aanmaak
------------------------------------------------------------------------- */
#define ICK_HZ	(120*1000*1000)	//120 MHz
#define PLIDIV_	(1)		//choose from 1 2 3
#define PLLMUL2	(30)	//choose from 20-60, mul = choice/2
#define NOP()	__asm("nop")

	//TB 5.5 User LED, p.12
#define LED0 PORTD.PODR.BIT.B6
#define LED1 PORTD.PODR.BIT.B7

	//TB 5.11 User Switch, p.15
#define SW1 PORTB.PIDR.BIT.B1

#include <iodefine.h>
#include <stdint.h>

	//HW 5. I/O-Registers, p. 187
	//  (2) Notes on writing to I/O registers:
#define IOREG_WRITE(r,v)	do{(r)=(v); if((r)!=(v)) NOP();} while(0)
#define WRITE_CHK(r,v)		do (r)=(v); while( (r)!=(v) )

	//HW 9. Clock Generation Circuit
enum en_ck_div {CK_1, CK_2, CK_4, CK_8, CK_16, CK_32, CK_64};	//0..6
enum en_sck {SCK_LOCO,SCK_HOCO,SCK_MAIN,SCK_SUB,SCK_PLL};		//0..4
void HOCO_PLL_120MHz (void)
{	//start HOCO, wait for oscillation to stabilize:
	SYSTEM.HOCOCR.BYTE=0;
	while(!SYSTEM.OSCOVFSR.BIT.HCOVF)
		NOP();
	//stop RTC-oscillator:
	RTC.RCR4.BYTE=0;
	RTC.RCR3.BIT.RTCEN=0;
	SYSTEM.SOSCCR.BIT.SOSTP=1;
	//set and start PLL (HOCO clock), wait for oscillation to stabilize:
	SYSTEM.PLLCR.WORD=PLLMUL2-1<<8 | 1<<4 | PLIDIV_-1;
	SYSTEM.PLLCR2.BYTE=0;
	while(!SYSTEM.OSCOVFSR.BIT.PLOVF)
		NOP();
	//2 wait states (> 100 MHz):
	WRITE_CHK( SYSTEM.ROMWT.BYTE, 2 );
	//set various bus clock division factors:
	SYSTEM.SCKCR.LONG= 1<<23 | 1<<22			//PSTOP1|PSTOP0
	| CK_4<<28 | CK_2<<24 | CK_4<<16			//FCK ICK BCK =60 120 60 MHz
	| CK_2<<12 | CK_4<< 8 | CK_4<< 4 | CK_4<<0;//PCKA -B -C -D=120 60 60 60 MHz
	//USB clock: default
	//select PLL as clock source, then stop LOCO:
	IOREG_WRITE(SYSTEM.SCKCR3.BIT.CKSEL,SCK_PLL);
	SYSTEM.LOCOCR.BYTE=1;
}

	//HW 22. I/O Ports
enum en_edge {eLOW, eFALL, eRISE, eBOTH};
void hardware_setup (void)
{//	LED0=0; //initial value, ON at target board
	LED1=1; //initial value, OFF at target board
#	define _(r,na,in,out) (r)=(na)|(~(in)&(out))
	//NB: 100-pin  na.:76543210 in:76543210 out76543210
	_(PORT0.PDR.BYTE,0b01011111, 0b00000000, 0b00000000);//P0:7,5
	_(PORT1.PDR.BYTE,0b00000011, 0b00000000, 0b00000000);//P1:7-2
	_(PORT2.PDR.BYTE,0b00000000, 0b00000000, 0b00000000);//P2:7-0
	_(PORT3.PDR.BYTE,0b00000000, 0b00100000, 0b00000000);//P3:7-0, P35=NMI
	_(PORT4.PDR.BYTE,0b00000000, 0b00000000, 0b00000000);//P4:7-0
	_(PORT5.PDR.BYTE,0b11000000, 0b00000000, 0b00000000);//P5:5-0
	_(PORT6.PDR.BYTE,0b11111111, 0b00000000, 0b00000000);//P6 na. 100-pin
	_(PORT7.PDR.BYTE,0b11111111, 0b00000000, 0b00000000);//P7 na. 100-pin
	_(PORT8.PDR.BYTE,0b11111111, 0b00000000, 0b00000000);//P8 na. 100-pin
	_(PORT9.PDR.BYTE,0b11111111, 0b00000000, 0b00000000);//P9 na. 100-pin
	_(PORTA.PDR.BYTE,0b00000000, 0b00000000, 0b00000000);//PA:7-0
	_(PORTB.PDR.BYTE,0b00000000, 0b00000000, 0b00000000);//PB:7-0 PB1==SW1
	_(PORTC.PDR.BYTE,0b00000000, 0b00000000, 0b00000000);//PC:7-0
	_(PORTD.PDR.BYTE,0b00000000, 0b00000000, 0b11000000);//PD:7-0 LED1, LED0
	_(PORTE.PDR.BYTE,0b00000000, 0b00000000, 0b00000000);//PE:7-0
	_(PORTF.PDR.BYTE,0b11111111, 0b00000000, 0b00000000);//PF na. 100-pin
	_(PORTG.PDR.BYTE,0b11111111, 0b00000000, 0b00000000);//PG na. 100-pin
	_(PORTJ.PDR.BYTE,0b11110111, 0b00000000, 0b00000000);//PJ:3
	//            I/O:144-65=79, in-only:1   out='1'-bits
#	undef _

	//HW 23. Multi-Function Pin Controller (MPC)
	//HW 23.2.13 PBn Pin Function Control Register (PBnPFS) (n = 0 to 7)
	//bit 6: '1' = ISEL: Interrupt input function SELect
	MPC.PWPR.BIT.B0WI=0;
	MPC.PWPR.BIT.PFSWE=1;

	MPC.PB1PFS.BYTE=1<<6;		//used as IRQn input pin (IRQ4-DS)

	MPC.PWPR.BIT.PFSWE=0;
	MPC.PWPR.BIT.B0WI=1;

	//HW 15. Interrupt Controller (ICUB)
	IEN(ICU, IRQ4)=0;			//IRQ4 disable
	ICU.IRQFLTC0.BIT.FCLKSEL4=3;//PCLKB / 64
	ICU.IRQCR[4].BIT.IRQMD=eBOTH;//eRISE;//eFALL;//
	IR(ICU, IRQ4)=0;			//IRQ4 clear
	ICU.IRQFLTE0.BIT.FLTEN4=1;	//IRQ4 digital filter enable
	IPR(ICU, IRQ4)=15;			//IRQ4 priority
	IEN(ICU, IRQ4)=1;			//IRQ4 enable
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

uint32_t t_LED0;	//2log(250.000) = 17,93...
#if(2<=TEST)
	uint16_t t_LED1;		//2log(250) = 7,96...
#else
	uint8_t t_LED1;		//2log(250) = 7,96...
#endif
int main (void)
{	//PowerON_Reset (in start.s) has arranged some things:
	//- with MVTC-instructie:
	//  - set ISP and USP stack pointers
	//  - set start address of vector tables EXTB and INTB
	//  - set floating-point status word FPSW
	// - initialized the DATA section with SMOVF-instruction
	// - cleared the BSS section with SSTR.b-instruction
	// - called (empty) HardwareSetup(), leave that empty unless using the ADC
	// - enabled interrupts in PSW with MVTC-instruction
	// - started user mode, see HW 2.2.2.5 Processor Status Word (PSW)
	// - initialized static C++-objects
	// - called main(), so we arrived here

	//HW 13. Register Write Protection Function: disable protection
	SYSTEM.PRCR.WORD = 0xa50b;

	HOCO_PLL_120MHz();//mcu_clock_setup();
	hardware_setup();

	//put protection back on
	SYSTEM.PRCR.WORD = 0xa500;

	while(1)
		;
return 0;
}

	/* ---------------------------------------------------------
	private
	--------------------------------------------------------- */

	/* ---------------------------------------------------------
	protected
	--------------------------------------------------------- */
#include "interrupt_handlers.h"

void INT_Excep_ICU_IRQ4(void)
{	if(!SW1)		//LED0 (press)
		LED0^=1;
	else LED1^=1;	//LED1 (release)
}
