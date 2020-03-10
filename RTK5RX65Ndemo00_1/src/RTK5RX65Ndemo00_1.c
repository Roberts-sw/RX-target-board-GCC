/* RTK5RX65Ndemo00_1.c
   ------------------------------------------ TAB-size 4, code page UTF-8 --

functies:
- LOCO clock 240 kHz
- TMR1 source PCLKB, CMIA1: 240 + interrupt, vector INTB149
- INTB149 Millis-count, div 500 = toggle LED1 (PORTD bit 7)

test:
- board: RTK5RX65N3 Target Board for RX65N
- chip: R5F565NEDDFP PLQP-100KB, hardware manual R01UH0590EJ0230 (HW)
- compiler: GCC for Renesas RX 8.3.0.201904
- IDE: Renesas e2-studio 7.7.0

wijzigingen:
	RvL 10-3-2020	aanmaak
------------------------------------------------------------------------- */
#define BSP_ICLK_HZ		(240*1000)	//240 kHz
#define R_BSP_NOP()    __asm("nop")

#include <iodefine.h>
#include <stdint.h>

void clock_source_select (void)
{	volatile uint8_t i, dummy, tmp;
	SYSTEM.MOFCR.BYTE=0;			// 0008 C293h:00

	if(0==SYSTEM.HOCOCR.BIT.HCSTP)	//HOCO is operating
		while(0 == SYSTEM.OSCOVFSR.BIT.HCOVF)
			R_BSP_NOP();
	else
		SYSTEM.HOCOPCR.BYTE=1;		//turn off HOCO-power

	if (0 == SYSTEM.RSTSR1.BIT.CWSF)
	{	RTC.RCR4.BIT.RCKSEL = 0;
		for (i = 0; i < 4; i++)
			dummy = RTC.RCR4.BYTE;	/* dummy read four times */
		if (0 != RTC.RCR4.BIT.RCKSEL)
			R_BSP_NOP();

		RTC.RCR3.BIT.RTCEN = 0;
		for (i = 0; i < 4; i++)
			dummy = RTC.RCR3.BYTE;	/* dummy read four times */
		if (0 != RTC.RCR3.BIT.RTCEN)
			R_BSP_NOP();
	}
	SYSTEM.SOSCCR.BYTE = 0x01;
	if (0x01 != SYSTEM.SOSCCR.BYTE)
		R_BSP_NOP();
	while (0 != SYSTEM.OSCOVFSR.BIT.SOOVF)
		R_BSP_NOP();

	if (BSP_ICLK_HZ > 100*1000*1000)
	{	SYSTEM.ROMWT.BYTE = 0x02;
		if(0x02 == SYSTEM.ROMWT.BYTE)
			R_BSP_NOP();
	}
	else
	if (BSP_ICLK_HZ > 50*1000*1000)
	{	SYSTEM.ROMWT.BYTE = 0x01;
		if(0x01 == SYSTEM.ROMWT.BYTE)
			R_BSP_NOP();
	}
}
void operating_frequency_set (void)
{	SYSTEM.PRCR.WORD = 0xA50B;
	clock_source_select();

	uint32_t tmp_clock;
	tmp_clock = 0x00c00000;			//PSTOP1|PSTOP0
	SYSTEM.SCKCR.LONG = tmp_clock;
    if(tmp_clock ==  SYSTEM.SCKCR.LONG)
        R_BSP_NOP();

    tmp_clock = 0x00000011;
    SYSTEM.SCKCR2.WORD = (uint16_t)tmp_clock;
    if((uint16_t)tmp_clock ==  SYSTEM.SCKCR2.WORD)
        R_BSP_NOP();

    tmp_clock = ((uint16_t)0) << 8;	//LOCO
    SYSTEM.SCKCR3.WORD = (uint16_t)tmp_clock;
    if((uint16_t)tmp_clock ==  SYSTEM.SCKCR3.WORD)
        R_BSP_NOP();

	SYSTEM.PRCR.WORD = 0xA500;
}
void mcu_clock_setup (void)
{	operating_frequency_set();
}

void bsp_non_existent_port_init (void)
{	PORT0.PDR.BYTE |= 0x5f;
	PORT1.PDR.BYTE |= 0x03;
	PORT2.PDR.BYTE |= 0x00;
	PORT3.PDR.BYTE |= 0x00;
	PORT4.PDR.BYTE |= 0x00;
	PORT5.PDR.BYTE |= 0xc0;
	PORT6.PDR.BYTE |= 0xff;
	PORT7.PDR.BYTE |= 0xff;
	PORT8.PDR.BYTE |= 0xff;
	PORT9.PDR.BYTE |= 0xff;
	PORTA.PDR.BYTE |= 0x00;
	PORTB.PDR.BYTE |= 0x00;
	PORTC.PDR.BYTE |= 0x00;
	PORTD.PDR.BYTE |= 0x00;
	PORTE.PDR.BYTE |= 0x00;
	PORTF.PDR.BYTE |= 0xff;
	PORTG.PDR.BYTE |= 0xff;
	PORTJ.PDR.BYTE |= 0xf7;
}
void bsp_adc_initial_configure (void)
{	SYSTEM.PRCR.WORD = 0xA502;
	uint32_t tmp_mstp = MSTP(S12AD1);

	MSTP(S12AD1) = 0;
	S12AD1.ADSAMPR.BYTE = 0x03;
	S12AD1.ADSAM.WORD = 0x0020;
	S12AD1.ADSAMPR.BYTE = 0x02;

	MSTP(S12AD1) = tmp_mstp;
	SYSTEM.PRCR.WORD = 0xA500;
}
void bsp_bsc_initial_configure (void)//EXDMAC (LCD-buscontroller) prioriteit
{    BSC.EBMAPCR.LONG = 0x42130;//000C 5800h:00043210
}
void hardware_setup (void)
{//	output_ports_configure();
//	interrupts_configure();
//	peripheral_modules_enable();	=> void R_Systeminit();
	SYSTEM.PRCR.WORD = 0xA50BU;
	MPC.PWPR.BIT.B0WI = 0U;
	MPC.PWPR.BIT.PFSWE = 1U;
	//	R_CGC_Create();
	//	R_Config_PORT_Create();
	//		PORTD.PODR.BYTE = 0;
	//		PORTD.ODR1.BYTE = 0;
	//		PORTD.DSCR.BYTE = 0;
	//		PORTD.DSCR2.BYTE = 0;
	//		PORTD.PMR.BYTE = 0;
			PORTD.PDR.BYTE = 0x80;
	//	R_Config_TMR1_Create();
			IEN(PERIB, INTB149) = 0U;
			MSTP(TMR01) = 0U;
			TMR1.TCR.BYTE = 0x48;
			TMR1.TCORA = 240-1;
	//		TMR1.TCORB = 255;
			ICU.SLIBR149.BYTE = 0x06U;
			IPR(PERIB, INTB149) = 15;
	//	R_BSP_InterruptWrite(BSP_INT_SRC_UNDEFINED_INTERRUPT,(bsp_int_cb_t)r_undefined_exception);

	MPC.PWPR.BIT.PFSWE = 0U;
	MPC.PWPR.BIT.B0WI = 1U;
	SYSTEM.PRCR.WORD = 0xA500U;

	bsp_non_existent_port_init();
	bsp_adc_initial_configure();
	bsp_bsc_initial_configure();
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

void R_Config_TMR1_Start(void)
{	IR(PERIB, INTB149) = 0U;
	IEN(PERIB, INTB149) = 1U;
	TMR1.TCCR.BYTE = 0x08;
}

int main(void)
{   //PowerON_Reset (in start.s) heeft al het volgende gedaan:
	//- mbv. MVTC-instructie:
	//  - ISP en USP (stack pointers) gezet
    //  - EXTB en INTB (exception- en interrupt-tabelstartadres) gezet
    //  - FPSW (floating-point status word) ingevuld: denormalized of +/-0
    //- DATA-sectie (geïnitialiseerde variabelen) ingevuld met SMOVF-instructie
    //- BSS-sectie (ongeïnitialiseerde variabelen) op 0 met SSTR.b-instructie
	//- interrupts in PSW aanzetten mbv. MVTC-instructie voordat user-mode start
	//- omschakelen naar user-mode, zie HW 2.2.2.5 Processor Status Word (PSW)
	//- globale/static C++-objecten initialiseren
	//- main() aanroepen

	mcu_clock_setup();	//2-delig:
	//1e: clock_source_select()		kies oscillator, indien niet LOCO
	//2e: operating_frequency_set()	klokdeling en schakel naar nieuwe bron
	//NB: RXCC stelt frequentie in voorafgaand aan init van DATA en BSS!

	hardware_setup();	//IO-pinnen (ook niet-aanwezige!), timers, ADC, etc.


	R_Config_TMR1_Start();
	while(1)
		;
return 0;
}

	/* ---------------------------------------------------------
	protected
	--------------------------------------------------------- */
#include "interrupt_handlers.h"

void INT_Excep_PERIB_INTB149 (void)
{	static uint32_t volatile Millis;
	if( 500<= ++Millis )
		PORTD.PODR.BIT.B7^=1,Millis=0;
}
