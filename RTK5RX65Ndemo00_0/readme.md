RTK65RX65Ndemo00_0
---
**functionality:** LED1 500 ms on, 500 ms off
- LOCO clock 240 kHz
- TMR1 source PCLKB, CMIA1: 240 + interrupt, vector INTB149 => 1kHz
- INTB149 Millis-count, div 500 = toggle LED1 (PORTD bit 7) => 2 Hz

**using smart configurator:**
- clocks: default settings according to hardware manual as much as reasonable
  - untick main clock, tick LOCO clock
  - select LOCO input for clock multiplexer, set all divides to x1 => 240 kHz
- components:
  - add 8-Bit Timer > Next > choose Resource: TMR1 > Finish, settings:
    - Counter clear: Cleared by compare match A
    - Compare match A value (TCORA): 240 Count
    - Compare match B value (TCORB): 256 Count (default!)
    - Interrupt setting: tick enable TCORA compare match interrupt (CMIA1)
  - add Ports > Finish, settings:
    - tick PORTD, select tab PORTD, at PD7 choose Out
- save settings with Control-S, left-click symbol to generate C-code

**make the HardwareDebug settings:**
- Project > Properties >
- _Properties for RTK5RX65Ndemo00_0_
  - Run/Debug Settings > RTK5RX65Ndemo00_0 HardwareDebug > Edit... >
- _Edit launch configuration properties_
  - Debugger > Connection Settings >
    - Clock: HOCO
    - Connection with Target Board: change dropdown selection JTag to Fine
    - Power: change dropdown selection Yes to No
  - OK > Apply and Close >
- Control-B to build the project, or: Project > Build Project >

**add code:**
- in src > smc_gen > Config_TMR1 > Config_TMR1_user.c,
  in routine r_Config_TMR1_cmia1_interrupt(), between the comment lines, add:
  ```.c
  static uint32_t volatile Millis;
  if( 500<= ++Millis )
      PORTD.PODR.BIT.B7^=1,Millis=0;
  ```
- in src > RKT5RX65Ndemo00_0.c, in routine main() add:
  ```.c
  R_Config_TMR1_Start();
  ``` 
- rename the file RKT5RX65Ndemo00_0.c to main.c

**Debug/Run:**
- ^B (build project), F11 (Debug), _Confirm Perspective Switch_ Yes >
- In the menu Run, take a look at the function key shortcuts to step and run
- ^F2 will terminate the debug session

**result:**
This project shows that with very little effort, only 4 lines of source code,
you can produce a lot of program, more than 15 kB in size with this specific
ucon:
- 128 files in 31 folders, excluding documentation
- prog: 15648
- constant: 1504
- data: 12
- bss: 2116
- other: 5404

**note:**
When debugging and trying to understand what was going on, I gave up after
having 16 open source files by repeatedly choosing "Step Into", and then I
just did "Run".
Using the "configurators" with an IDE will **not** give you insight in what
is happening!

A program with this functionality programmed for an 8-bit ucon would
typically require less than 200 Bytes of program code altogether, and as
additional benefit apart from the much smaller memory footprint you as a
programmer would have insight in what happens "under the hood", meaning
that you would also know how to change settings in a running program.

The IDE provides a benefit of quick start-up, but you loose the knowledge
for changing settings as could be needed in a certain program and as an
added disadvantage you need a more expensive ucon, with more memory.

This controller speed suffices for a lot of projects where IO has to be
controlled. I remember the days when we were building fan controllers
powered by PIC-microcontrollers, and using a 3,68MHz crystal to arrive
at a, divided-by-four, maximum instruction speed of less than 1 MHz.
The frequency was chosen to make using a hardware-UART possible with
communication at for example 9600 baud.
All calculations had to be done by software, as multiply and divide
were not implemented, and using more than 8-bit values required extra
software to handle it. Even then, multiple analog output were made by
software-PWM routines and it all functioned well enough for
livestock-farming.