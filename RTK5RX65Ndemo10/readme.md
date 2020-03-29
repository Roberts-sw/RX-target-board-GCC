RTK65RX65Ndemo10:
---
This demo is inspired by home automation, Latin name: domotica, where simple
 push buttons can be used to control lamp dimming/switching, curtain sliders,
 window blinds etcetera.
 In the example also the concept of a state automaton is used, as it eases
 the explanation of what the software does.
 The tactile push button of the target board will be used for 3 input events:
1. release, used to get into a stable output state 
1. press, used to leave a stable output state 
1. hold, after >= 1 second, repeats every 0,1 second until release, used 
   for changing lamp intensity.

For a `natural' response, the lamp will switch on at press, and off at
 release after press when it was on. When the lamp is on, hold will change
 the lamp brightness, and release after hold will change the direction of
 lamp brightness dimming.

uses:
- target board library: ../lib/

**create the project as a C++-project with GCC**:

**make the HardwareDebug settings**:

**Debug/Run**:

**creating a library project**:

see [GCC_e2studio_RX65N.md](../GCC_e2studio_RX65N.md)

**adding a library path**:

- Project > Properties
- _Properties for RTK5RX65Ndemo10_
  - C/C++ Build > Settings > Compiler > Includes >
  - click at the `plus' sign next to `Include file directoties'
- _Add directory path_
  - Workspace... > 
  - Folder selection: select `lib/src', OK > OK > Apply and Close > Yes >

**result:**
 - prog: 5660
 - constant: 1434
 - data: 1
 - bss: 11
 - other: 28
