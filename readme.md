demo projects:
---

The aim of these projects is to help people with a low budget in using
 the RX ucon, by using: 
1. an RX target board, priced at about 25 Euros, with embedded debugger.
2. e2studio, usable free-of-charge
3. GCC, usable free-of-charge

General instructions for getting started with installation of the tools
 and starting a new project are in [GCC_e2studio_RX65N.md](./GCC_e2studio_RX65N.md).

The projects are setup to take small incremental steps in understanding
 and using the RX ucon, and at the time of this writing, are only tested
 on Windows. Proejct names all start with `RTK5RX65N`, I will just use
 what's behind that in the followin:

The projects incremental features are:
1. [demo00_0](./RTK5RX65Ndemo00_0): using smart configurator for a simple program,
1. [demo00_1](./RTK5RX65Ndemo00_1): LOCO clock oscillator 240 kHz, TMR CMIA1-interrupt,
   IO pins, simplifying a project and at the same time shrinking program size,
1. [demo01](./RTK5RX65Ndemo01): HOCO clock 16 MHz, IO pin macro for initializing,
1. [demo02](./RTK5RX65Ndemo02): HOCO + PLL, cascading timers, emulating RX100/RX200 series,
1. [demo03](./RTK5RX65Ndemo03): PLL + wait states, maximum specified clock speed 120 MHz,
1. [demo03_144](./RTK5RX65Ndemo03_144): overclocking the RX65N, +20% test at 144 MHz,
1. [demo04](./RTK5RX65Ndemo04): using different integer number sizes, Arduino-style
   functions micros() and millis(),
1. [demo05](./RTK5RX65Ndemo05): IRQ-interrupt
1. [demo06](./RTK5RX65Ndemo06): pin name macro, "bare metal" io pin control, iopin library,
1. [demo07](./RTK5RX65Ndemo07): using C++ and C++-wrappers,
1. [demo08](./RTK5RX65Ndemo08): communication LED,
1. [demo09](./RTK5RX65Ndemo09): synchronous LED library,
1. [demo10](./RTK5RX65Ndemo10): combining iopin library and synchronous LED library
1. [demo11](./RTK5RX65Ndemo11): communication LED, program/control button and LED light

#### RTK5RX65Ndemo09+: to be done
 
**Background**: As a developer of hard- and software, the Renesas
 RX microcontroller (ucon) has some strengths that appeal to me:
- It's assembly language can be read, like in the Gnu Assembler,
  like the English language, from left to right.
- It's Endian-setting can be chosen: Big-Endian or Little-Endian.
  Big-Endian feels more natural because it starts with the most
  important things, refining details further on.

There seems to be little interest from Renesas in hobbyist developers. 
It's like they don't understand that when you have experience in using 
a microcontroller, choosing that ucon for a development project is a 
natural choice.
