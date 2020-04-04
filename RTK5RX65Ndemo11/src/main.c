/* RTK5RX65Ndemo11 > src > main.c
   ------------------------------------------ TAB-size 4, code page UTF-8 --

functies:
- ICLK 120 MHz, PCLKA 120 MHz, PCLKB 60 MHz
- LED1: TIOCA3 pwm output
-(LED2: TIOCB3 pwm output, at the PCB PB1 isn't connected to LED2)
- LED2: TIOCC3 pwm output, LED2 is connected to PB2, despite the documentation
  (OB) telling it is PB1 and warning of PB1 also being connected to TB SW1.

test:
- board: RTK5RX65N3 Target Board for RX65N, manual R20UT4167EJ0101 (TB)
  board: RTK00CLDX0B Cloud Option Board, manual R12UM0039EG0100 (OB)
- chip: R5F565NEDDFP PLQP-100KB, hardware manual R01UH0590EJ0230 (HW)
- compiler: GCC for Renesas RX 8.3.0.201904
- IDE: Renesas e2-studio 7.7.0

wijzigingen:
    RvL 4-4-2020    test
------------------------------------------------------------------------- */
#include "dimmer.h"

    //TB 5.11 User Switch, p.15
#define USER_SW    U08_BIT(IO_._PIDR[0x3]).B1
#define sample(b) !(b)

    //HW 22. I/O Ports
#define PWM_PERIOD  (100*100)//100,00 [%]
void init_peripherals (void)
{   //PB2 PB0 output
    iopin_alldirs(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                  0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x00,
                  0x00,0x00);

    //HW 26.2.1 Timer Control Register (TCR)
    MSTP_TPU_=0;                        //release TPU from stop
    TPU3_.TCR=6<<5|eTPU_BOTH<<3|2<<0;   //TGRD-clear, PCLKB/16 = 7,5MHz count
    TPU3_.TMDR=1+2;                     //PWM mode 2

    //use TGR3D for period and interrupt for time keeping: TGI3D is ...
    //... SLIB-source 31, 0 is reserved, SLIBR128 + 31 - 1 => SLIBR158
    TPU3_.TGRD=PWM_PERIOD-1;            //period=10.000 count => 750Hz
    TPU3_.TIER=1<<'D'-'A';              //enable interrupt request TGI3D
    ICU_.SLIBR158=31;                   //SLIBR158 clock source = 31 (TGI3D)
    ICU_.IPR[158]=1;                    //priority 1

    //HW 23.4.2 Notes on MPC Register Setting:
    //  PFS-settings should be made while the PMR-bit is 0
    MPC_.PFS[8 * 0xB + 0]=3;            //PB2 => TIOCA3
    MPC_.PFS[8 * 0xB + 2]=3;            //PB0 => TIOCC3
    IO_._PMR[0xB]|=1<<2|1<<0;           //peripheral function for PB2 and PB0
}

void start_peripherals (void)
{// ICU_.IR[158]=0;                     //clear  PCLKB-interrupt nr 158
    ICU_.IER[158/8]|=1<<158%8;          //enable SLIBR158-interrupt
    TPU_CST3=1;                         //88100:0 start TCNT3
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

u16 pwm;
void dimmer_do (void)
{   DIMMER_EVENT ev=dimmer_event( sample(USER_SW) );
    if(NO_DIM_EVENT==ev)
        return;

    static LAMP_STATE st=O;
    dimmer_action act=DIMMER_AUTOM[st][ev];
    if(!act)
        return;//incomplete matrix?
    u08 tmp=pct;
    st=act();
    if(tmp==pct)
        return;//no pct change

    pwm=pct*pct;//gamma 2,00
    if(pct)
    {   if(!tmp)
        {   //for a small pwm value the match will have passed already, ...
            //... so reinit the timer to prevent one cycle without match:
            TPU_CST3=0;
            TPU3_.TCNT=0;
            TPU3_.TIORH=0<<4|3<<0;//MATB_NO, MATA_LT: low-toggle
            TPU3_.TIORL=0<<4|3<<0;//MATD_NO, MATC_LT
        }
        TPU3_.TGRA=pwm-1;//off after match
        TPU3_.TGRC=pwm-1;//off after match
    } else
    {   //if the pwm value was at or above period, there was no match, ...
        //... so reinit the timer to ensure high (off) level:
        TPU_CST3=0;
        TPU3_.TCNT=0;
        TPU3_.TIORH=0<<4|6<<0;//MATB_NO, MATA_HH: high-high
        TPU3_.TIORL=0<<4|6<<0;//MATD_NO, MATC_HH
    }
    TPU_CST3=1;
}

volatile u08 flag_100ms, cnt_750Hz;
int main(void)
{   SYSTEM_.PRCR = 0xa50b;
    HOCO_PLL_120MHz();
    PFS_unlock();
    init_peripherals();
    PFS_relock();
    SYSTEM_.PRCR = 0xa500;

    for(start_peripherals(); ; )
        if( flag_100ms )
        {   flag_100ms=0;
            dimmer_do();
        }
return 0;
}

    /* ---------------------------------------------------------
    protected
    --------------------------------------------------------- */
#include "interrupt_handlers.h"

void INT_Excep_PERIB_INTB158 (void)
{   if(75<=++cnt_750Hz)
        cnt_750Hz=0, flag_100ms=1;
}
