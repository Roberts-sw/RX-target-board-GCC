programming features of this demo (06)
---

**main.c**:
like before, changes:
1. preprocessor macro for direct numbering of an output pin:
   - `#define LED0 PD6`: will expand to 62, **unequal** to the chip pin number
1. using library routines translating pin numbering for having
   hardware register access:
   - `iopin_init(LED0,IO_LO);`: will access several PORTD-registers to
     initialize the pin as an output pin having a low level.

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
- various 'private' precompiler-macros that gef '#undef'-ed at the end of
  the source file so they are unknown outside the source file.
  The term **private** is being used for things that remain inside this
  source file.
- a 'protected' IOPORT-array of constant values that holds bits for
  register offset inside the 'IO_'-structure as well as lsb bit-offsets
  of the pin inside the elements of 'IO_'.
  The term **protected** in this case only means that the name isn't exported
  within the header file, an `extern ...` could be used in a source file that
  knows of the existence and needs access.
- a 'public' ICPIN-array of constant values that holds the chip's pin numbers
  for each of the IO-pins of the chip.
  The term **public** means that the header declares the array, so it is
  accessible by a source file directly or indirectly including the header file.
- multi-purpose protection macro, for 'void' and non-void functions, to
  return  a value or nothing if the parameter for the iopin passed an invalid
  number:
  - `#define PROTECT	if(_isiopin(iopin) ) ; else return`
- macro's to determine port and bit numbers using the enum defined above
  and IOPORT[]:
  - `#define portbit_(iopin)    (!iopin ? 3 : IOPORT[iopin]& 7)`
  - `#define portnr_(iopin)     (!iopin ? 18: IOPORT[iopin]>>4)`
- definition of an input-only pin to test against for IO-functions:
  - `#define NMIPIN  P35`: will expand to 3, being the only pin that
	cannot be used as an output on this ucon. ICPIN[NMIPIN] has the value
	15, being the chip pin number of pin P35.
