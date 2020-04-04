programming features of this demo (10)
---

**main.c**:
like main.c before, changes:
1. Removed '#include <iodefine.h>' as this is a very large file and I wanted 
   to improve compilation time.
   Instead in the accomodating library 'lib' the files 'rx65x.c' and '.h'
   are created for a bare-bone approach to the hardware.
   The header, though incomplete, at the moment takes less than 500 lines,
   compared with the more than 32k lines of iodefine.h, and I already tested 
   the project with Big-Endian as well as Little-Endian data setting.
1. Added '#include "dimmer.h"' as an all-in-one include, that takes more
   includes from the library.
1. Moved clock initialisation to 'lib/src/rx65.c' where some possible clock
   speeds have their own routine, in this demo HOCO_PLL_120MHz() is used.
1. "Bare-metal" initialisation of peripherals with init_peripherals().
   This function utilises 'lib/src/rx65x.h > iopin_alldirs()', specifically 
   made for the 100-pin QFP of the RX65N target board.
1. Function dimmer_do() to handle button-events and state machine of the 
   dimmer.
1. SLIA233-interrupt for simulating pwm by period and duty cycle time-interrupt
   to drive output pin.
1. SLIA247-interrupt for simulating pwm by restarting output compare pulse.

**lib/src/dimmer.h**:
The enums DIMMER_EVENT and LAMP_STATE are declared to index the array of
 function pointers DIMMER_AUTOM[LAMP_STATES][DIM_EVENTS].

The declared control variable pct is the percentage output resulting from
 the state handling.

**lib/src/dimmer.c**:
The 'acting' functions of the dimmer can be private, as handling the dimmer
 state returns a pointer to the function to be called and the function itself
 has no input parameters.

**lib/src/rx65x.h**:
As the RX-microcontroller series can be used in Big-Endian as well as
 Little-Endian programs, I was looking for a way to get rid of depending
 on the huge `iodefine.h` file.

I decided to skip the bit definitions, because if you have to program
 hardware interfaces you still need to check with the hardware manual
 that the header files or at least the generated code addresses the 
 correct registers and bits.

Instead I made macros Uxx_BIT(), that transform a variable into a
 bit-struct of a given size depending on Endian-setting.
 Using this kind of macro to define IO-pins "bare-metal", for example
 '#define LED1 U08_BIT(IO_._PODR[0xD]).B7' will let you use the output
 pin like 'LED1=1;' and it will work in Big-Endian as well as Little-Endian.
