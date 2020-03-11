RTK65RX65Ndemo01:
- functionality:
  - HOCO clock 16 MHz
  - TMR1 source PCLKB/64, CMIA1: 250 + interrupt, vector INTB149
  - INTB149 Millis-count, div 500 = toggle LED1 (PORTD bit 7)
- skipping smart configurator
  - in stead of selecting smart configurator, choose Finish >
- taking functional code from RTK65RX65Ndemo00_1:
  - copy RTK5RX65Ndemo00_1.c to this project folder src and rename to main.c
  - edit some things to arrive at what is now inside the src folder
  - change `inthandler.c` by commenting out vector 149 to
    `//void INT_Excep_PERIB_INTB149(void){ }`.
- **make the HardwareDebug settings:**
  - Project > Properties >
  - _Properties for RTK5RX65Ndemo01_
    - Run/Debug Settings > RTK5RX65Ndemo01 HardwareDebug > Edit... >
  - _Edit launch configuration properties_
    - Debugger > Connection Settings >
      - Clock: HOCO
      - Connection with Target Board: change dropdown selection JTag to Fine
      - Power: change dropdown selection Yes to No
    - OK > Apply and Close >
  - Control-B to build the project, or: Project > Build Project >
- **Debug/Run:**
  - ^B (build project), F11 (Debug), _Confirm Perspective Switch_ Yes >
    F8 (Resume), F8  

result:
 - 8 files in "generate" folder, of which 1 line was commented out,
 - 1 file in "src" folder has all code that matters for the developer
 - prog: 2676
 - constant: 1176
 - data: 0
 - bss: 4
 - other: 28

This project has seen a giant jump in clock frequency, from 240 kHz to
16 MHz, a more than 66 times speedup, without even initializing the on-chip
PLL, 16 MHz is the default frequency of the HOCO-oscillator.
