demo projects:
---

The aim of these projects is to help people with a low budget in using
 the RX ucon, by using: 
1. an RX target board, priced around 25 Euros, with embedded debugger.
2. e2studio, usable free-of-charge.
3. GCC, usable free-of-charge.

General instructions for getting started with installation of the tools
 and starting a new project are in [GCC_e2studio_RX65N.md](./GCC_e2studio_RX65N.md).

The projects are setup to take small incremental steps in understanding
 and using the RX ucon, and at the time of this writing, are only tested
 on Windows. Project names all start with `RTK5RX65N`, I will just use
 what's behind that in the following summary:

The projects incremental features are:
1. [demo00_0](./RTK5RX65Ndemo00_0): 
   using smart configurator for a simple program,
1. [demo00_1](./RTK5RX65Ndemo00_1): LOCO clock oscillator 240 kHz, 
   TMR CMIA1-interrupt, IO pins, simplifying a project and at the same time 
   shrinking program size,
1. [demo01](./RTK5RX65Ndemo01): HOCO clock 16 MHz, IO pin macro for
   initializing,
1. [demo02](./RTK5RX65Ndemo02): HOCO + PLL, cascading timers, 
   emulating RX100/RX200 series, 
1. [demo03](./RTK5RX65Ndemo03): PLL + wait states, maximum specified
   clock speed 120 MHz,
1. [demo03_144](./RTK5RX65Ndemo03_144): overclocking the RX65N, +20%
   test at 144 MHz,
1. [demo04](./RTK5RX65Ndemo04): using different integer number sizes,
   Arduino-style functions micros() and millis(),
1. [demo05](./RTK5RX65Ndemo05): IRQ-interrupt
1. [demo06](./RTK5RX65Ndemo06): pin name macro, "bare metal" io pin control,
   iopin library,
1. [demo07](./RTK5RX65Ndemo07): using C++ and C++-wrappers,
1. [demo08](./RTK5RX65Ndemo08): communication LED of Modbus-RTU simulated,
1. [demo09](./RTK5RX65Ndemo09): synchronous LED library, uses iopin library
1. [demo10](./RTK5RX65Ndemo10): push button LED dimmer, automaton concept,
   simple dimmer library, pwm simulation in two different ways:
   1. repeated one-shot pulse
   2. timer-interrupts to drive a pin on and off
1. [demo11](./RTK5RX65Ndemo11): like demo10 with real pwm using 
   the cloud option board LEDs.

Starting with demo10, two new folders have been added to ease usage of
code snippets from the demos above in real projects:
1. [lib][./lib/src]: gathering of "library" files and Rx65x.h register header
2. [doc][./doc]: some .pdf-documentation about programming concepts used in the demos,
   as well as the .tex-sources for these documents.
   For generating the pdf's you can copy portable versions of TeXmaker of TeXstudio
   onto a windows computer or use an installer version like MikTeX.

#### RTK5RX65Ndemo12: UART-communication over SCI5, "bare metal" with library.
 
**Background**: Developing both hard- and software, the Renesas
 RX microcontroller (ucon) has some appealing things to me:
- It's assembly language can be read, like in the Gnu Assembler,
  as if it were in English language, from left to right.
- It's Endian-setting can be chosen to be Big-Endian, that feels
  more natural because it starts with the most important things,
  refining details further on.

These demos show that despite having a board with hardly any embedded
IO, you can still develop/test code snippets to be used in real projects.

The projects also show that even with a lot of interrupt vectors and IO-pins
 to initialize, application programs for the RX don't need much memory.
