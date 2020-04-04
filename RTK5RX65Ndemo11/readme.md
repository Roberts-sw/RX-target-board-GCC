RTK65RX65Ndemo11:
---
This demo is like demo10, only it now uses the cloud option board, where the
 LED pins have pwm-possibility.

For the target board the ucon-pins were:
- SW1 at pin 59, PB1
- LED0 at pin 80, PD6 (using MTU8A output compare and TGIB8-interrupt)
- LED1 at pin 79, PD7 (using software pin drive and TGIU5-interrupt)

For the option board, the ucon pins are:
- USER_SW at pin 19, P31
- LED1 at pin 61, PB0 (TIOCA3 pin function with pwm)
- (LED2 at pin 59, PB1 (TIOCB3 pin function with pwm) ) incorrect documentation
- LED2 at pin 58, PB2 (TIOCC3 pin function with pwm)

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
   - _Properties for RTK5RX65Ndemo11_
     - C/C++ Build > Settings > Compiler > Includes >
     - click at the 'plus' sign next to `Include file directories'
   - _Add directory path_
     - Workspace... > 
     - Folder selection: select `lib/src', OK > OK > Apply and Close > Yes >
1. library file:
   - Project > Properties
   - _Properties for RTK5RX65Ndemo11_
     - C/C++ Build > Settings > Linker > Source >
     - click at the 'plus' sign next to `Additional input files'
   - _Add directory path_
     - Workspace... > 
     - File selection: select `lib/Debug/src/liblib.a', OK > OK > Apply and Close >

**result:**
 - prog: 3840
 - constant: 1276
 - data: 1
 - bss: 11
 - other: 28
