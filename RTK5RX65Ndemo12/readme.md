RTK65RX65Ndemo12:
---
This demo uses the cloud option board to communicate via serial-to-USB with
 a PC serial port program.

For creating the program a serial port library has been created in an
 object-orientated manner in C language, such that the routines can be used
 for all 13 SCI-modules of the RX65N.
 C language is used for the library so one can explicitly see what 
 is going on "behind the scenes" from the source code.

The ucon pins being used have functions at the option board:
- LED1 at pin 61, PB0
- LED2 at pin 58, PB2
- TxD5 at pin 66, PA4
- RxD5 at pin 67, PA3

uses:
- target/option board library: ../lib/.
  Be sure to have the same data Endian-setting for the library as 
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
 - prog: 9376
 - constant: 1552
 - data: 1132
 - bss: 312
 - other: 28
