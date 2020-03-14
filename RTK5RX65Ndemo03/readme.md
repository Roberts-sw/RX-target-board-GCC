RTK65RX65Ndemo03:
**functionality:** LED1 500 ms on, 500 ms off
- HOCO clock 16 MHz, PLL / 1 * 15 = 240 MHz
- ICLK / 2 = 120 MHz, PCLKB / 4 = 60 MHz
- TMR0 source PCLKB, CMIA0: 240 no interrupt => 0,25 MHz
- TMR1 source CMIA0, CMIA1: 250 + interrupt, vector INTB149 => 1kHz
- INTB149 Millis-count, div 500 = toggle LED1 (PORTD bit 7) => 2 Hz

**skipping smart configurator:**
- in stead of selecting smart configurator, choose Finish >

**taking functional code from RTK65RX65Ndemo02:**
- copy main.c to this project folder src
- edit some things to arrive at what is now inside the src folder
- change `inthandler.c` by commenting out vector 149 to
  `//void INT_Excep_PERIB_INTB149(void){ }`.

**make the HardwareDebug settings:**
- Project > Properties >
- _Properties for RTK5RX65Ndemo03_
  - Run/Debug Settings > RTK5RX65Ndemo03 HardwareDebug > Edit... >
- _Edit launch configuration properties_
  - Debugger > Connection Settings >
    - Clock: HOCO
    - Connection with Target Board: change dropdown selection JTag to Fine
    - Power: change dropdown selection Yes to No
  - OK > Apply and Close >
- Control-B to build the project, or: Project > Build Project >

**Debug/Run:**
- ^B (build project), F11 (Debug), _Confirm Perspective Switch_ Yes >
  F8 (Resume), F8  
- ^F2 will terminate the debug session

**result:**
 - 8 files in "generate" folder, of which 1 line was commented out,
 - 1 file in "src" folder has all code that matters for the developer
 - prog: 2772
 - constant: 1176
 - data: 0
 - bss: 4
 - other: 28

**note:**
With these settings you can get an idea of the performance of this ucon.
Restrictions apply, as the peripheral clocks except PCLKA have a maximum
setting of 60 MHz.
Furthermore, as the Flash memory is only capable of operation upto 50 MHz,
ROM wait states had to be inserted.
