demo projects:
---

#### RTK5RX65Ndemo00_0:
**functionality:** LED1 500 ms on, 500 ms off, using smart configurator
- LOCO clock 240 kHz
- TMR1 source PCLKB, CMIA1: 240 + interrupt, vector INTB149 => 1kHz
- INTB149 Millis-count, div 500 = toggle LED1 (PORTD bit 7)

This project shows that with very little effort, only 4 lines of source code,
you can produce a lot of program, more than 15 kB in size with this specific
microcontroller (ucon):
- main routine:
  ```.c
  R_Config_TMR1_Start();
  ``` 
- interrupt callback:
  ```.c
  static uint32_t volatile Millis;
  if( 500<= ++Millis )
      PORTD.PODR.BIT.B7^=1, Millis=0;
  ```

A program with this functionality programmed for an 8-bit ucon would
typically require less than 200 Bytes of program code altogether, and as
additional benefit apart from the much smaller memory footprint you as a
programmer would have insight in what happens "under the hood", meaning
that you would also know how to change settings in a running program.

The IDE provides a benefit of quick start-up, but you loose the knowledge
for changing settings as could be needed in a certain program and as an
added disadvantage you need a more expensive ucon, with more memory.

#### RTK5RX65Ndemo00_1:
**functionality:** same as version `_0`, with only functional code extracted,
so that debugging gives much less source files opened and much more insight
in what is needed to start working with the ucon, compiler and IDE.

This project shows that even on a 32-bit ucon the amount of memory used for
having some functionality can be restricted, in this case from 15 kB to a
little more than 3 kB.

#### RTK5RX65Ndemo01:
**functionality:** same as before, with other settings:
- HOCO clock 16 MHz
- TMR0 source PCLKB, CMIA0: 64 without interrupt => 0,25 MHz
- TMR1 source CMIA0, CMIA1: 250 + interrupt, vector INTB149 => 1 kHz
- INTB149 Millis-count, div 500 = toggle LED1 (PORTD bit 7)
- using a precompiler-macro to initialize all IO-pins with ease

The instruction clock frequency has gone up more than 66-fold, but by
hardware-interlocking the second timer the interrupt-routine doesn't
change at all to get the same functionality of the LED.
As an added bonus, despite having to implement clock switching to get
at the higher frequency, the program now only takes 2,5 kB in stead of
the original 15 kB, so in total the size has been divided by 6.

#### RTK5RX65Ndemo02: to be done
**functionality:** same as version `01`, with other settings:
- HOCO clock 16 MHz, PLL used to get ICLK-frequency of 32 MHz

With these settings you can use the RX65N-board to get an idea of the
RX100-series performance (32MHz max, no FPU), or the RX231/RX23W
(with built-in FPU) when operating at 32 MHz.

#### RTK5RX65Ndemo03: to be done
**functionality:** same as version `02`, with other settings:
- HOCO clock 16 MHz, PLL used to get ICLK-frequency of 120 MHz
- ROM wait states inserted, because the RX65x doesn't use MONOS-Flash!

With these settings you can get an idea of the performance of this ucon.

#### RTK5RX65Ndemo04: to be done
**functionality:** differing from version `03`:
- made Arduino-style functions micros() and millis(), derived from TMR0 and TMR1
- LED0 (PORTD bit 6) starts with '0', toggles every 250.000 us, using micros()
  - at least 32-bit local time-value needed to compare with micros()
- LED1 (PORTD bit 7) starts with '1', toggles every 250 ms, using millis()
  - at least 8-bit local time-value needed to compare with millis()

This project serves to gain some feeling about which integer sizes to use for
RAM-restricted programs (I know, this one has 640 kB of it), and why you would
sometimes use unsigned and signed integers of differing sizes. With a few
`#define` statements to be (un)commented for variations in functionality, you
can debug and try to explain in which way and why the behaviour differs with
the chosen setting.

#### RTK5RX65Ndemo05: to be done
