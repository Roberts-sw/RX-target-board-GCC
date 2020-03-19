/* RTK5RX65Ndemo07 > src > main.cpp
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
	RvL 18-3-2020	aanmaak
------------------------------------------------------------------------- */
#define ICK_HZ	(120*1000*1000)	//120 MHz
#define PLIDIV_	(1)		//choose from 1 2 3
#define PLLMUL2	(30)	//choice from 20-60, mul = choice/2
#define NOP()	__asm("nop")

#include <iodefine.h>
#include <stdint.h>
#include "iopin.h"

	//TB 5.5 User LED, p.12
IoPin led0(PD6);//#define LED0 PD6
IoPin led1(PD7);//#define LED1 PD7

	//TB 5.11 User Switch, p.15
IoPin sw1(PB1);//#define SW1 PB1

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
{/*	iopin_alldirs(	0b00000000,0b00000000,0b00000000,0b00000000,//PORT0..3
					0b00000000,0b00000000,0b00000000,0b00000000,//PORT4..7
					0b00000000,0b00000000,0b00000000,0b00000000,//PORT8..B
					0b00000000,0b11000000,0b00000000,0b00000000,//PORTC..F
					0b00000000,0b00000000	);//PORTG, PORTJ
*/	led0.init(IO_LO);//iopin_init(LED0,IO_LO);
	led1.init(IO_HI);//iopin_init(LED1,IO_HI);
	sw1.init(IO_IN);//iopin_init(SW1, IO_IN);//

	//HW 23. Multi-Function Pin Controller (MPC)
	//HW 23.2.13 PBn Pin Function Control Register (PBnPFS) (n = 0 to 7)
	//bit 6: '1' = ISEL: Interrupt input function SELect
	sw1.mpcfunc(1<<6);//iopin_mpcfunc(SW1,1<<6);//	//(IRQ4-DS)

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

int main (void)
{	//HW 13. Register Write Protection Function: disable protection
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
	protected
	--------------------------------------------------------- */
extern "C" {
	#include "interrupt_handlers.h"
}

void INT_Excep_ICU_IRQ4(void)
{	if(!sw1.read() )//if(!iopin_read(SW1) )//	//LED0 (press)
		led0.write(!led0.read() );//iopin_write(LED0,!iopin_read(LED0) );
	else led1.toggle();//else iopin_toggle(LED1);//LED1 (release)
}
