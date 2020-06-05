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
    RvL 2-6-2020
    * CMT0_start(), TMR01_start(): 1 interrupt flag per IRn-register
    RvL 1-6-2020
    * comment line HOCO_PLL_120MHz(): //  10               (source HOCO)
    RvL 29-3-2020   concept, test
------------------------------------------------------------------------- */
#include "rx65x.h"                      //address:default

    /* ---------------------------------------------------------
    mcu_clock_deinit(): back to default
    LOCO_240kHz()
    HOCO_16MHz()
    HOCO_PLL_120MHz()
    --------------------------------------------------------- */
#define start_HOCO()\
do{ SYSTEM_.HOCOCR=0;                   /*0008 0036:0?*/\
    while(!BITGET_(SYSTEM_.OSCOVFSR,b03_HCOVF) )    NOP();\
} while(0)
#define stop_subclk()\
do{ RTC_.RCR4=0;                        /*0008 C428:0?*/\
    RTC_.RCR3&=~1;                      /*0008 C426:0?*/\
    IOREG_WRITE(SYSTEM_.SOSCCR,0x1);    /*0008 0033:00*/\
} while(0)

void mcu_clock_deinit (void)
{   SYSTEM_.LOCOCR_=0;                  //0008 0034:00       (LOCO on)
    SYSTEM_.HOCOCR=0;                   //0008 0036:0?       (HOCO on)
    while(!BITGET_(SYSTEM_.OSCOVFSR,b03_HCOVF) )    NOP();
    SYSTEM_.SCKCR3=CKS_HOCO<<8|0x00;    //0008 0026:0000     (sysclk=HOCO)
    SYSTEM_.SCKCR2=0x0011;              //0008 0024:0011
    SYSTEM_.SCKCR=0x00000000;           //0008 0020:00000000
    SYSTEM_.ROMWT=0x00;                 //0008 101C:00
    SYSTEM_.PLLCR2=0x01;                //0008 002A:01
    SYSTEM_.PLLCR=0x1d00;               //0008 0028:1d00     (source main)
    SYSTEM_.SCKCR3=CKS_LOCO<<8|0x00;    //0008 0026:0000     (sysclk=LOCO)
    SYSTEM_.HOCOCR=0x01;                //0008 0036:01       (HOCO off)
}
void LOCO_240kHz (void)
{   SYSTEM_.SCKCR3=CKS_LOCO<<8|0x00;    //0008 0026h:0000 (default)
    SYSTEM_.ROMWT=0x00;
}
void HOCO_16MHz (void)
{   start_HOCO();
    stop_subclk();

    //stop SDCLK and BCLK output:
    SYSTEM_.SCKCR=0x00c00000;           //0008 0020h:00000000 (PSTOP1|PSTOP0)

    SYSTEM_.SCKCR3=CKS_HOCO<<8|0x00;    //0008 0026:0000     (sysclk=HOCO)
//  SYSTEM_.LOCOCR_=1;                  //0008 0034:00       (LOCO off)
}
    //PLL output must be 120..240MHz
#define PLLDIV_ (1)     //choose from 1 2 3
#define PLLMUL2 (30)    //choose from 20-60, mul = choice/2
#define PLL_OUT (240*1000*1000) //16MHz/1 * 30/2 => 240 MHz
void HOCO_PLL_120MHz (void)
{   start_HOCO();
    stop_subclk();

    //set and start PLL (source HOCO), wait for oscillation to stabilize:
    SYSTEM_.PLLCR = PLLMUL2-1<<8        //1d00 => mul = (29+1)/2
        | 1<<4                          //  10               (source HOCO)
        | PLLDIV_-1;                    //   0 => div = (0+1)
    SYSTEM_.PLLCR2=0;
    while(!BITGET_(SYSTEM_.OSCOVFSR,b02_PLOVF) )    NOP();
    WRITE_CHK(SYSTEM_.ROMWT,2);         //2 wait states (> 100 MHz)

    //set various bus clock division factors:
    SYSTEM_.SCKCR=  1<<23|1<<22         //00c00000  PSTOP1|PSTOP0
    |CK_4<<28|CK_2<<24|CK_4<<16         //21020000  FCK ICK BCK   =60 120 60 MHz
    |CK_2<<12|CK_4<< 8|CK_4<< 4|CK_4<<0;//00001222  PCLKA -B -C -D=120 60 60 60

    //USB clock: default
//  SYSTEM.SCKCR2.WORD=0x11;

    //select PLL as clock source, optionally stop LOCO:
    SYSTEM_.SCKCR3=CKS_PLL<<8|0x00;     //0008 0026:0000     (sysclk=PLL-OUT)
//  SYSTEM_.LOCOCR_=1;                  //0008 0034:00       (LOCO off)
}

    /* ---------------------------------------------------------
    ms-timer:   PCLKB = PLL_OUT/4 = 60 MHz
        16-bit, src = PCLKB/32 = 1,875 MHz => div 1,875 = *34953>>16
    
    CMT0: period-1  
    TMR01: 16-bit mode, period-1
    --------------------------------------------------------- */
void CMT0_init (void)
{   MSTP_CMT01_=0;          //release CMT01 from stop
    //29.2.3 CMTCR: b7 write value should be '1'
    CMT0_.CMCR=128|BIT_(6)|1;//b7|CMIE|PCLKB/32 => 1875 kHz
    CMT0_.CMCOR=1875-1;     //f_match = f_clock / 1875
    ICU_.IPR[IPR_NR(28)]=1; //vector 28 priority -> IPR[4] !
}
void CMT0_start (void)
{   ICU_.IR[28]=0;          //clear flag
    ICU_.IER[28/8]|=1<<28%8;//enable interrupt 28
    CMT_CST0=1;             //CMT-count start ch.0
}
//void INT_Excep_CMT0_CMI0(void) {Micros+=1000, ++Millis;}

void TMR01_init (void)
{   MSTP_TMR01_=0;          //release TMR01 from stop
    TMR0_.TCR=0x48;         //clear on match A, with interrupt
    TMR01_.TCORA=1875-1;    //f_match = f_clock / 1875
    ICU_.SLIBR146=3;        //CMIA0=SLIB-clock 3+144-1=146
    ICU_.IPR[146]=1;        //vector 146 priority
}
void TMR01_start (void)
{   ICU_.IR[146]=0;         //clear flag
    ICU_.IER[146/8]|=1<<146%8;//enable interrupt 146
    TMR01_.TCCR=0x180b;     //16-bit count PCLK/32 => 1875kHz
}
//void INT_Excep_PERIB_INTB146(void) {Micros+=1000, ++Millis;}
