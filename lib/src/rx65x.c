/* rx65x.c
   ------------------------------------------ TAB-size 4, code page UTF-8 --

clock sources:
0. LOCO: 240 kHz (default)
1. HOCO clock: 16 MHz
2. HOCO + PLL: 120 MHz, maximum specified clock, with wait states

test:
- board: RTK5RX65N3 Target Board for RX65N, manual R20UT4167EJ0101 (TB)
- chip: R5F565NEDDFP PLQP-100KB, hardware manual R01UH0590EJ0230 (HW)
- compiler: GCC for Renesas RX 8.3.0.201904
- IDE: Renesas e2-studio 7.7.0

changes:
	RvL 29-3-2020	concept, test
------------------------------------------------------------------------- */
#include "rx65x.h"

void mcu_clock_deinit (void)				//address:default
{	SYSTEM_.LOCOCR_=0;						//0008 0034h:00h   (LOCO on)
	SYSTEM_.HOCOCR=0;						//(not default)     start HOCO
	while(!bitget_(SYSTEM_.OSCOVFSR,b03_HCOVF) )
		NOP();
	SYSTEM_.SCKCR3=CKS_HOCO<<8|0x00;		//(not default)     sysclock=HOCO
	SYSTEM_.SCKCR2=0x0011;					//0008 0024h:0011h
	SYSTEM_.SCKCR=0x00000000;				//0008 0020h:00000000h
	SYSTEM_.ROMWT=0x00;						//0008 101Ch:00h
	SYSTEM_.PLLCR2=0x01;					//0008 002Ah:01h
	SYSTEM_.PLLCR=0x1d00;					//0008 0028h:1d00h (source main osc)
	SYSTEM_.SCKCR3=CKS_LOCO<<8|0x00;		//0008 0026h:0000h (sysclock=LOCO)
	SYSTEM_.HOCOCR=0x01;					//0008 0036h:01h
}
void LOCO_240kHz (void)
{	//select LOCO as clock source
	SYSTEM_.SCKCR3=CKS_LOCO<<8|0x00;		//0008 0026h:0000 (default)
	SYSTEM_.ROMWT=0x00;
}
void HOCO_16MHz (void)
{	//start HOCO:
	SYSTEM_.HOCOCR=0;
	while(!bitget_(SYSTEM_.OSCOVFSR,b03_HCOVF) )
		NOP();

	//stop sub-clock oscillator:
	RTC_.RCR4=0;							//0008 C428h:0?
	bitclr_(RTC_.RCR3,0);					//0008 C426h:0?
	IOREG_WRITE(SYSTEM_.SOSCCR,0x1);		//0008 0033h:00

	//stop SDCLK and BCLK output:
	SYSTEM_.SCKCR=0x00c00000;				//0008 0020h:00000000 PSTOP1|PSTOP0

	//select HOCO as clock source, optionally stop LOCO:
	SYSTEM_.SCKCR3=CKS_HOCO<<8|0x00;		//0008 0026h:0000
//	SYSTEM_.LOCOCR_=1;						//0008 0034h:00	stop LOCO
}
	//PLL output must be 120..240MHz
#define PLLDIV_ (1)		//choose from 1 2 3
#define PLLMUL2 (30)	//choose from 20-60, mul = choice/2
#define PLL_OUT	(240*1000*1000)	//16MHz/1 * 30/2 => 240 MHz
void HOCO_PLL_120MHz (void)
{	//start HOCO:
	SYSTEM_.HOCOCR=0;
	while(!bitget_(SYSTEM_.OSCOVFSR,b03_HCOVF) )
		NOP();

	//stop RTC-oscillator:
	RTC_.RCR4=0;
	RTC_.RCR3&=~0;
	SYSTEM_.SOSCCR=1;

	//set and start PLL (source HOCO), wait for oscillation to stabilize:
	SYSTEM_.PLLCR=PLLMUL2-1<<8|1<<4|PLLDIV_-1;
	SYSTEM_.PLLCR2=0;
	while(!bitget_(SYSTEM_.OSCOVFSR,b02_PLOVF) )
		NOP();
	WRITE_CHK(SYSTEM_.ROMWT,2);//2 wait states (> 100 MHz):

	//set various bus clock division factors:
	SYSTEM_.SCKCR=	1<<23|1<<22			//	PSTOP1|PSTOP0
	|CK_4<<28|CK_2<<24|CK_4<<16			//	FCK ICK BCK  =60 120 60    MHz
	|CK_2<<12|CK_4<< 8|CK_4<< 4|CK_4<<0;//	PCKA -B -C -D=120 60 60 60 MHz

	//USB clock: default
//	SYSTEM.SCKCR2.WORD=0x11;

	//select PLL as clock source, optionally stop LOCO:
	SYSTEM_.SCKCR3=CKS_PLL<<8|0;
//	SYSTEM.LOCOCR.BYTE=1;
}

