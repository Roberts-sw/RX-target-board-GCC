programming features of this demo (07)
---

**main.cpp**:
like main.c before, changes:
1. object creation for remembering pin numbers:
   - `IoPin led0(PD6);`: will create the object led0 and store 62 in
	 its private storage. In the C-coded variant, commented out, no
	 storage was used for remembering the pin as it was declared as a
	 precompiler `#define` statement.
1. using library routines translating pin numbering for having
   hardware register access:
   - `led0.init(IO_LO);`: will access several PORTD-registers to
     initialize the pin of object led0 as an output pin having a low level.
     The commented tou C-coded variant iopin_init() had no access to
     a stored pin number, so it needed the defined number as a parameter.
1. `#include "interrupt_handlers.h"` is surrounded with with a predecessor
   line `extern "C" {` and a successor line `}`, as it is a header file for
   C-routines.
   The compiler will use `C` call conventions for its C-functions.
1. The IRQ4-interrupt routine, although called with `C` conventions, is a
   C++-routine, as it can directly call the objects `read()`, `write()` and
   `toggle()` methods.
   - Because C++ in the iopin-library is added for each function as an
     inline C++-wrapper, one can still call the C-functions, even in
     C++-source files.

**iopin.h**:
The concept of 'X'-macros is being used to create a table of 'interesting'
 things around the IO-pins of the ucon.
An 'X'-macro resembles a table and needs another macro to create the wanted
 functionality.

Here a variadic macro is used only because it it typewriter-friendly, not
 all parameters have to be typed in as arguments, and extending the table
 at the right-hand side can be done without changing the existing macro's.
- `#define RX65x100(X)\ ...`: does nothing by itself
- `#define ENUM2(a,b,...)    P ## b,`: will translate one element of the
  'X'-macro, the second column of each X()-line, prepended with a P into
  an initializer list element. The comma is used for element separation.
- `typedef enum en_iopin     {PJ3, CHIP(ENUM2) eIOPINS } eIOPIN;`: will
  couple the names PJ3, ..., P05 to numbers 0..78 and eIOPINS to 79, the
  number of IO-pins in the 100-pin package for the RX651 or RX65N.
  An RX650, would it exist like the RX230 next to the RX231, could probably
  have 3 extra IO-pins in stead of the USB Vcc, DM and DP pins, like I
  hinted in this header file.

In order to use the same header for C as well as C++ code, I decided not to
 have an embracing `extern "C" { ... }` for the C part, but rather prepend
 each C function with `extern "C"`, defined in the macro CCALL.
 For the C case it will expand to nothing to prepend.

As the RX-microcontroller series can be used in Big-Endian as well as
 Little-Endian programs, I was looking for a way to get rid of depending
 on the huge `iodefine.h` file.

I decided to skip the bit definitions, because if you have to program
 hardware interfaces you still need to check with the hardware manual
 that the header files or at least the generated code addresses the 
 correct registers and bits.

When I saw a question at [http://renesasrulz.com/rx](http://renesasrulz.com/rx)
 from someone asking for a way to
 [assign an array to an absolute address in RAM](http://renesasrulz.com/rx/f/rx---forum/15912/how-to-assign-an-array-to-an-absolute-address-in-ram-based-on-rxv3)
 I ended up composing the PORT-registers as a struct of arrays located
 at their respective addresses. Then I only needed something for the
 'interwoven' ODR0 and ODR1 registers, straight-forward would be as a
 2-dimensional array, but I liked the idea of just one index combined
 with element acces, so I settled for a 'struct inside a struct':
```.c
#define IO_ (*(struct\
{   uint8_t _PDR[32],_PODR[32],_PIDR[32],_PMR[32];\
    struct {uint8_t _0,_1;} _ODR[32];\
    uint8_t _PCR[32],_DSCR[32],_fill[0x28],_DSCR2[32];\
} volatile *const)0x0008C000)
```

**iopin.c**:
As this file hasn't changed from demo06, the programming features remain:
- multi-purpose protection macro, for 'void' and 'int' functions, to
  return if the parameter for the iopin passed an invalid number:
  - `#define PROTECT	if(_isiopin(iopin) ) ; else return`:
	can be used with ';' or value and ';' to reaturn from the routine.
- macro's to determine port and bit numbers using the enum defined above
  and an array of constant values:
  - `#define portbit_(iopin)    (!iopin ? 3 : IOPORT[iopin]& 7)`
  - `#define portnr_(iopin)     (!iopin ? 18: IOPORT[iopin]>>4)`
- definition of an input-only pin to test against for IO-functions:
  - `#define NMIPIN  P35`: will expand to 3, being the only pin that
	cannot be used as an output on this ucon. ICPIN[NMIPIN] has the value
	15, being the chip pin number of pin P35.
