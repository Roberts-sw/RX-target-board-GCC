RTK65RX65Ndemo10:
---
This demo is inspired by home automation (Latin: domotica), where simple
 push buttons can be used to control lamp dimming/switching, curtain sliders,
 window blinds etcetera.
 In the example also the concept of a state automaton is used, as it eases
 the explanation of what the software does.
 The tactile push button of the target board will be used for 3 input events:
1. release, used to get into a stable output state 
1. press, used to leave a stable output state 
1. hold, after >= 1 second, repeating every 0,1 second until release, used 
   for changing lamp intensity.

For a 'natural' response, the lamp will switch on at press, and off at
 release after press when it was on. When the lamp is on, hold will change
 the lamp brightness, and release after hold will change the direction of
 lamp brightness dimming.

**Note:**

as both LED-pins don't have pwm-possibility, 
some creative solutions had to be found. 
They are:
1. LED0: PD6 can produce a one-shot pulse with an output compare function.
   By having an interrupt at the end of the pulse, a new pulse can be started, 
   so the repetition of pulses resembles a pwm-output.
1. LED1: PD7 can use it's pin only with input capture, but the timer has a
   compare match interrupt, with which a pwm-output can be simulated.
   The timer value for the match-interrupt can be changed to have a value for
   the duty cycle and then for the period, so a sequence of two interrupts can
   be used to change the output pin. For continuous on or off only 1 interrupt 
   per period is being used.

uses:
- target board library: ../lib/.
  **Be sure to have the same data Endian-setting for the library as 
  for the project depending on it.**

**create the project as a C++-project with GCC**:

**make the HardwareDebug settings**:

**Debug/Run**:

**creating a library project**:

see [GCC_e2studio_RX65N.md](../GCC_e2studio_RX65N.md)

**adding a library path**:

1. include files:
   - Project > Properties
   - _Properties for RTK5RX65Ndemo10_
     - C/C++ Build > Settings > Compiler > Includes >
     - click at the 'plus' sign next to `Include file directories'
   - _Add directory path_
     - Workspace... > 
     - Folder selection: select `lib/src', OK > OK > Apply and Close > Yes >
1. library file:
   - Project > Properties
   - _Properties for RTK5RX65Ndemo10_
     - C/C++ Build > Settings > Linker > Source >
     - click at the 'plus' sign next to `Additional input files'
   - _Add directory path_
     - Workspace... > 
     - File selection: select `lib/Debug/src/liblib.a', OK > OK > Apply and Close >

**result:**
 - prog: 3984
 - constant: 1276
 - data: 1
 - bss: 11
 - other: 28

Even when simulating pwm in two different ways and including a library without 
any compiler optimization (optimization -O0), the code for a fairly complete 
program can be very compact, as is shown by this dimmer-demo.
