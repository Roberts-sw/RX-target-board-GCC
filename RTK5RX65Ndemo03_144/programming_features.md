programming features of this demo
---

1. using preprocessor macro's for constant values:
   - `#define ICK_HZ    (144*1000*1000)  //120 MHz`
1. incorporating assembly language with a preprocessor macro:
   - `#define NOP() __asm("nop")`
1. preprocessor macro for direct naming of an output pin:
   - `#define LED1 PORTD.PODR.BIT.B7`
1. reading back/checking a written value with a preprocessor macro:
   - `#define IOREG_WRITE(r,v)  do{(r)=(v); if((r)!=(v)) NOP();} while(0)`
   - `#define WRITE_CHK(r,v)    do (r)=(v); while( (r)!=(v) )`
1. using enums for constant values:
   - `enum en_ck_div {CK_1, CK_2, CK_4, CK_8, CK_16, CK_32, CK_64}; //0..6`
   - `enum en_sck {SCK_LOCO,SCK_HOCO,SCK_MAIN,SCK_SUB,SCK_PLL};     //0..4`
1. preprocessor macro for initializing IO-pins as well as unused pins:
   - `define _(r,na,in,out) (r)=(na)|(~(in)&(out))`
1. using interrupt routines and private C-variables directly:
```.c
int main (void)
{	...
}

#include "interrupt_handlers.h"

void INT_Excep_PERIB_INTB149 (void)
{	static ...
	...
}
```
