programming features of this demo (08)
---

**main.cpp**:
1. precompiler macro for direct addressing of SW1 and LED1 pins.
1. clock initialisation as in demo06
1. hardware_setup() as in demo04.
1. C-like C++, not using classes.
1. using very little code and RAM-memory for keyscan-state and sample-timing.
1. `#include "interrupt_handlers.h"` is surrounded with with a predecessor
   line `extern "C" {` and a successor line `}`, as it is a header file for
   C-routines.
   The compiler will use `C` call conventions for its C-functions.
1. The INTB149-interrupt routine, does the work of translating a released
   keypress into a blink time and driving the LED, as well as keeping time
   for micros() and millis(), all in 17 documented lines of code.
