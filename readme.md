demo projects:
---

**Background**: As a developer of hard- and software, the Renesas
RX microcontroller (ucon) has some strengths that appeal to me:
- It's assembly language can be read, like in the Gnu Assembler, in the 
  natural order for Latin alphabet languages, from left to right.
- It's Endian-setting can be chosen: Big-Endian or Little-Endian (default).
  Big-Endian is useful for working with "natural" quantities, where
  comparisons have to be made.
  For example sorting of ASCII-text, if the ucon is in Big-Endian mode,
  it can do multi-Byte compares to speed up sorting.

There seems to be a complete lack of interest from Renesas towards
hobbyist developers. It's like they don't understand that when you
have experience in using a microcontroller, choosing that ucon for
a development project is a natural choice.

The aim of these projects is to help people with a low budget in using
the RX ucon, by using:
1. an RX target board, priced at about 20 Euros, with embedded debugger.
2. e2studio, usable free-of-charge
3. GCC, usable free-of-charge

General instructions for getting started with installation of the tools
and starting a new project are in GCC_e2studio_RX65N.md

The projects are setup to take small incremental steps in understanding
and using the RX ucon, and at the time of this writing, are only tested
on Windows.

The projects incremental features are:
1. RTK5RX65Ndemo00_0: using smart configurator for a simple program,
1. RTK5RX65Ndemo00_1: LOCO clock oscillator 240 kHz, TMR CMIA1-interrupt,
   IO pins, simplifying a project and at the same time shrinking program size,
1. RTK5RX65Ndemo01: HOCO clock 16 MHz, IO pin macro for initializing,
1. RTK5RX65Ndemo02: HOCO + PLL, cascading timers, emulating RX100/RX200 series,
1. RTK5RX65Ndemo03: PLL + wait states, maximum specified clock speed 120 MHz,
1. RTK5RX65Ndemo03_144: overclocking the RX65N, +20% test at 144 MHz,
1. RTK5RX65Ndemo04: using different integer number sizes, Arduino-style
   functions micros() and millis(),
1. RTK5RX65Ndemo05: IRQ-interrupt
1. RTK5RX65Ndemo06: pin name macro, "bare metal" io pin control, iopin-library,
1. RTK5RX65Ndemo07: using C++ and C++-wrappers,
1. RTK5RX65Ndemo08: communication LED library,
1. RTK5RX65Ndemo09: synchronous LED-library,
1. RTK5RX65Ndemo10: combining iopin-library and synchronous LED library

#### RTK5RX65Ndemo06+: to be done
