/* RTK5RX65Ndemo04 > src > main.c
   ------------------------------------------ TAB-size 4, code page UTF-8 --

functies:
- HOCO clock 16 MHz, PLL / 1 * 15 = 240 MHz
- ICLK / 2 = 120 MHz, PCLKB / 4 = 60 MHz
- TMR0 source PCLKB, CMIA0: 60 no interrupt => 1 MHz
- TMR1 source CMIA0, CMIA1: 250 + interrupt, vector INTB149 => 4kHz
- ROMWT=2
- INTB149 acc_4kHz, derived Micros, Millis

test:
- board: RTK5RX65N3 Target Board for RX65N, manual R20UT4167EJ0101 (TB)
- chip: R5F565NEDDFP PLQP-100KB, hardware manual R01UH0590EJ0230 (HW)
- compiler: GCC for Renesas RX 8.3.0.201904
- IDE: Renesas e2-studio 7.7.0
- dt1 type: (u)int8_t, (u)int16_t, (u)int32_t

wijzigingen:
	RvL 13-3-2020	aanmaak
------------------------------------------------------------------------- */
#define ICK_HZ	(120*1000*1000)	//120 MHz
#define PLIDIV_	(1)		//choose from 1 2 3
#define PLLMUL2	(30)	//choose from 20-60, mul = choice/2
#define NOP()	__asm("nop")

	//choose TEST 0..3
#define TEST 0//1//2//3//

	//TB 5.5 User LED, p.12
#define LED0 PORTD.PODR.BIT.B6
#define LED1 PORTD.PODR.BIT.B7

#include <iodefine.h>
#include <stdint.h>

	//HW 5. I/O-Registers, p. 187
	//  (2) Notes on writing to I/O registers:
#define IOREG_WRITE(r,v)	do{(r)=(v); if((r)!=(v)) NOP();} while(0)
#define WRITE_CHK(r,v)		do (r)=(v); while( (r)!=(v) )

	//Arduino-style functions, declare-only, as they use private variables:
uint32_t micros(void);
uint32_t millis(void);

	//HW 9. Clock Generation Circuit
enum en_ck_div {CK_1, CK_2, CK_4, CK_8, CK_16, CK_32, CK_64};	//0..6
enum en_sck {SCK_LOCO,SCK_HOCO,SCK_MAIN,SCK_SUB,SCK_PLL};		//0..4
void mcu_clock_setup (void)
{	//HW 7. Option-Setting Memory (OFSM):
	//OFS1 (in vects.c) has HOCO disabled at reset.
	//It has to be started, default at 16 MHz, used here.
	SYSTEM.HOCOCR.BYTE=0;		//start the HOCO, ...
	while(!SYSTEM.OSCOVFSR.BIT.HCOVF)
		NOP();					//... wait for oscillation to stabilize

	//HW 9.4.2 Handling of Pins when Sub-Clock is Not Used
	//HW 31. Real-time Clock (RTCd): select sub-clock as RTC-source ...
	RTC.RCR4.BYTE=0;
	//...before stopping sub-clock oscillator: RCR3.RTCEN=0 and SOSCCR.SOSTP=1
	RTC.RCR3.BIT.RTCEN=0;
	SYSTEM.SOSCCR.BIT.SOSTP=1;//IOREG_WRITE(SYSTEM.SOSCCR.BIT.SOSTP,1);

	//PLL: output 120..240MHz, not used
	SYSTEM.PLLCR.WORD=PLLMUL2-1<<8 | 1<<4 | PLIDIV_-1;	//(source HOCO)
	SYSTEM.PLLCR2.BYTE=0;		//start the PLL, ...
	while(!SYSTEM.OSCOVFSR.BIT.PLOVF)
		NOP();					//... wait for oscillation to stabilize

	//wait states: precompiler-macro decides if code is needed
#	if(ICK_HZ > 100*1000*1000)
	WRITE_CHK( SYSTEM.ROMWT.BYTE, 2 );
# elif(ICK_HZ >  50*1000*1000)
	WRITE_CHK( SYSTEM.ROMWT.BYTE, 1 );
#endif

	//bus clock division: necessary: f_PLL=240M, ICLK=120M
	//frequency restrictions: ICLK>=BCLK, PCLKA>=PCLKB>=PCLKC>=PCLKD
	SYSTEM.SCKCR.LONG= 1<<23 | 1<<22			//PSTOP1|PSTOP0
	| CK_4<<28 | CK_2<<24 | CK_4<<16			//FCK ICK BCK =60 120 60 MHz
	| CK_2<<12 | CK_4<< 8 | CK_4<< 4 | CK_4<<0;//PCKA -B -C -D=120 60 60 60 MHz

	//USB clock: not used
//	SYSTEM.SCKCR2.WORD=0x0011;	//0008 0024h:0011

	//select PLL as clock source, then stop LOCO
	IOREG_WRITE(SYSTEM.SCKCR3.BIT.CKSEL,SCK_PLL);
	SYSTEM.LOCOCR.BYTE=1;
}
	//HW 22. I/O Ports
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
	_(PORTB.PDR.BYTE,0b00000000, 0b00000000, 0b00000000);//PB:7-0
	_(PORTC.PDR.BYTE,0b00000000, 0b00000000, 0b00000000);//PC:7-0
	_(PORTD.PDR.BYTE,0b00000000, 0b00000000, 0b11000000);//PD:7-0 LED1, LED0
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

	mcu_clock_setup();
	hardware_setup();

	//put protection back on
	SYSTEM.PRCR.WORD = 0xa500;

	while(1)
	{	int32_t dt0 =micros() - t_LED0;
		if(0<=dt0)
			LED0^=1, t_LED0+=250*1000;

#if(3==TEST)
		uint16_t dt1 =millis()- t_LED1;
		if(65286<=dt1)
			continue;
#elif(2==TEST)
		int16_t dt1 =millis()- t_LED1;
		if(0>dt1)
			continue;
#elif(1==TEST)
		uint8_t dt1 =millis()- t_LED1;
		if(6<=dt1)
			continue;
#else
		int8_t dt1 = millis() - t_LED1;
		if(0>dt1)//if(0>dt1||6<=dt1)//
			continue;
#endif
		else LED1^=1, t_LED1+=250;
	}
return 0;
}

	/* ---------------------------------------------------------
	private
	--------------------------------------------------------- */
static uint32_t volatile Micros=0, Millis=0;

	/* ---------------------------------------------------------
	protected
	--------------------------------------------------------- */
#include "interrupt_handlers.h"
uint32_t micros (void) {return Micros+TMR1.TCNT;}   //1 MHz
uint32_t millis (void) {return Millis;}

void INT_Excep_PERIB_INTB149 (void)
{	static uint8_t acc_250us;   //4kHz
	if(Micros+=250, !(++acc_250us&3) )
		++Millis;
}
