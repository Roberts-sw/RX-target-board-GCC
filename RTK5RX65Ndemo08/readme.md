RTK65RX65Ndemo08:

This demo is inspired by the function of the yellow communication LED
 in a Modbus-RTU slave, that blinks shortly at receiving a message and
 longer when the slave responds to a message from the Modbus-master.
 For the demo a short key-press on SW1, less than a second, will be
 used for reception, and 1 second or more for responding with a long
 blink.

**functionality:** LED1 initially off, poll SW1
- HOCO clock 16 MHz, PLL / 1 * 15 = 240 MHz
- ICLK / 2 = 120 MHz, PCLKB / 4 = 60 MHz
- main(): poll SW1 every 0,1s
- release after press blinks LED1: long or short

### create the project as a C++-project with GCC:
### make the HardwareDebug settings:
### Debug/Run:
take a look at `readme.md` of RTK5RX65Ndemo07 or
 see [GCC_e2studio_RX65N.md](../GCC_e2studio_RX65N.md)

**taking over code from RTK65RX65Ndemo04 and demo07:**
- rename `RTK65RX65Ndemo08.cpp` to `main.cpp`.
- copy contents of `main.cpp` from demo07 into this projects `main.cpp`.
- take `hardware_setup()` and the Arduino-style functions from demo04.
- make some more changes to arrive at the `main.cpp` aas inside this project.
- change `inthandler.c` by commenting out vector 149 (line 381) to
  `//void INT_Excep_PERIB_INTB149(void){ }`.

**result:**
 - prog: 3340
 - constant: 1212
 - data: 4
 - bss: 18
 - other: 28
