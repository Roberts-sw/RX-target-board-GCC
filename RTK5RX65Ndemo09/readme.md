RTK65RX65Ndemo09:
---
This demo is inspired by the user interface of an industrial coffee
 machine, where several LED's are blinking whenever a choice can be
 made between the (touch) buttons they are located at.
 As the target board only has one button, it will be used differently:
1. after a long press, >= 1 second, changes the target LED for short press.
1. after a short press, < 1 second, changes the blink pattern of the LED,
   cycling through the numbers 0 to 9. The blink pattern repeats every 3 seconds:
   - 0 means LED off
   - 1-7 means 1-7 blinks per period
   - 8 means 16 fast blinks per period
   - 9 means LED on, with one off-moment to indicate the period-start

uses:
- iopin-library: iopin.c/.h
- syncled-library: syncled.c/.h

**create the project as a C++-project with GCC**:

**make the HardwareDebug settings**:

**Debug/Run**:

see [GCC_e2studio_RX65N.md](../GCC_e2studio_RX65N.md)

**result:**
 - prog: 5356
 - constant: 1370
 - data: 36
 - bss: 17
 - other: 28
