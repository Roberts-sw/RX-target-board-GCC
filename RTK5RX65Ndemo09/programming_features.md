programming features of this demo (09)
---

**main.cpp**:
1. iopin-library for driving led[0] and led[1]
1. syncled-library for synchronously driving led[]-patterns
1. Arduino-style micros() and millis()
1. precompiler macro for direct addressing of SW1 pin.
1. clock initialisation as in demo06
1. hardware_setup() for TMR-interrupt.
1. syncled_init() for initializing led[]-array
1. using very little code and RAM-memory for keyscan-state and sample-timing.
1. syncled_run() does timing and pattern drive of led[]-array.
1. The INTB149-interrupt routine keeps time for micros() and millis().
