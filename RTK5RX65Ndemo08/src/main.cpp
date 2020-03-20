/* RTK5RX65Ndemo08 > src > main.cpp
   ------------------------------------------ TAB-size 4, code page UTF-8 --

functies:
- HOCO clock 16 MHz, PLL / 1 * 15 = 240 MHz
- ICLK / 2 = 120 MHz, PCLKB / 4 = 60 MHz
- ROMWT=2
- LED1 communication LED, initially off, blink once at release of SW1 press
- SW1 long press=answer: 200ms blink, short press=message: 2ms blink

test:
- board: RTK5RX65N3 Target Board for RX65N, manual R20UT4167EJ0101 (TB)
- chip: R5F565NEDDFP PLQP-100KB, hardware manual R01UH0590EJ0230 (HW)
- compiler: GCC for Renesas RX 8.3.0.201904
- IDE: Renesas e2-studio 7.7.0

wijzigingen:
	RvL 19-3-2020	aanmaak
------------------------------------------------------------------------- */
#define ICK_HZ	(120*1000*1000)	//120 MHz
#define PLIDIV_	(1)		//choose from 1 2 3
#define PLLMUL2	(30)	//choice from 20-60, mul = choice/2
#define NOP()	__asm("nop")

#include <iodefine.h>
#include <stdint.h>

	//Arduino-style functions, declare-only, as they use private variables:
uint32_t micros(void);
uint32_t millis(void);

	//TB 5.5 User LED, p.12 + 5.11 User Switch, p.15
#define LED1 PORTD.PODR.BIT.B7
#define SW1 PORTB.PIDR.BIT.B1

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
void hardware_setup (void)
{	LED1=1; //initial value, OFF at target board
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
	_(PORTB.PDR.BYTE,0b00000000, 0b00000000, 0b00000000);//PB:7-0 (SW1)
	_(PORTC.PDR.BYTE,0b00000000, 0b00000000, 0b00000000);//PC:7-0
	_(PORTD.PDR.BYTE,0b00000000, 0b00000000, 0b10000000);//PD:7-0 LED1
	_(PORTE.PDR.BYTE,0b00000000, 0b00000000, 0b00000000);//PE:7-0
	_(PORTF.PDR.BYTE,0b11111111, 0b00000000, 0b00000000);//PF na. 100-pin
	_(PORTG.PDR.BYTE,0b11111111, 0b00000000, 0b00000000);//PG na. 100-pin
	_(PORTJ.PDR.BYTE,0b11110111, 0b00000000, 0b00000000);//PJ:3
	//            I/O:144-65=79, in-only:1   out='1'-bits
#	undef _

	//HW 23. Multi-Function Pin Controller (MPC)
	//no alternate pin functions used

	//HW 28. 8-Bit Timer (TMR)
	//timing interrupt vector choice:
	//HW Table 15.3, p.456 Sources for Software Configurable Interrupt B
	//  TMR1.CMIA1 is 6th, for convenience use SLIBR144 - 1 + 6 = SLIBR149.
	IEN(PERIB, INTB149)=0;		//disable PCLKB-interrupt 149
	MSTP(TMR01)=0U;				//release TMR0 and 1 from module-stop

	TMR0.TCR.BYTE=0x08;			//clear on match A, no interrupt
	TMR0.TCORA=60-1;			//match-frequency = f_clock / 60

	TMR1.TCR.BYTE=0x48;			//clear on match A, with interrupt
	TMR1.TCORA=250-1;			//match-frequency = f_clock / 250
	ICU.SLIBR149.BYTE=0x06;		//source for TMR1.CMIA1-interrupt
	IPR(PERIB, INTB149)=0x0F;	//priority 15
	IR(PERIB, INTB149)=0;		//clear  PCLKB-interrupt nr 149
	IEN(PERIB, INTB149)=1;		//enable PCLKB-interrupt nr 149

	//start timers
	TMR1.TCCR.BYTE=0x18;		//clock source = CMIA0 (1MHz)
	TMR0.TCCR.BYTE=0x08;		//clock source = PCLKB (60MHz)
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

uint8_t presscnt, t_SW1;
#define bPRESS 128
#define LONG_RELEASE 10
#define LONG_PRESS bPRESS|10
int main (void)
{	//HW 13. Register Write Protection Function: disable protection
	SYSTEM.PRCR.WORD = 0xa50b;

	HOCO_PLL_120MHz();//mcu_clock_setup();
	hardware_setup();

	//put protection back on
	SYSTEM.PRCR.WORD = 0xa500;

	while(1)
		if(256-100>(uint8_t)(millis()- t_SW1) )	//0,1s
			if(t_SW1+=100, !SW1)				//? press
			{	presscnt|=bPRESS;
				if(!++presscnt)					//  ? overflow
					presscnt--;					//    ? rollback
			} else presscnt&=~bPRESS;			//: remember time

return 0;
}

	/* ---------------------------------------------------------
	private
	--------------------------------------------------------- */
static uint32_t volatile Micros=0, Millis=0;

	/* ---------------------------------------------------------
	protected
	--------------------------------------------------------- */
uint32_t micros (void) {return Micros+TMR1.TCNT;}   //1 MHz
uint32_t millis (void) {return Millis;}
extern "C" {
	#include "interrupt_handlers.h"
}

void INT_Excep_PERIB_INTB149 (void)
{	static uint8_t acc_250us, on_ms;   //4kHz
	if(Micros+=250, !(++acc_250us&3) )
	{	++Millis;
		if(on_ms)						//LED drive
		{	if(255!=on_ms)				//(255 holds)
				on_ms--;				//on-count
			LED1=0;						//? on
		} else LED1=1;					//: off
		if(!(presscnt&bPRESS)			//not pressed
		&& presscnt )					//... after press
		{	on_ms=LONG_RELEASE>presscnt	//? short=> short
			? 2 : 200;					//  long => long
			presscnt=0;					//, forget press
		}
	}
}
