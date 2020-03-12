RTK65RX65Ndemo01:
**functionality:** LED1 500 ms on, 500 ms off
- HOCO clock 16 MHz
- TMR1 source PCLKB/64, CMIA1: 250 + interrupt, vector INTB149 => 1kHz
- INTB149 Millis-count,    div 500 = toggle LED1 (PORTD bit 7) => 2 Hz

**skipping smart configurator:**
- in stead of selecting smart configurator, choose Finish >

**taking functional code from RTK65RX65Ndemo00_1:**
- copy main.c to this project folder src
- edit some things to arrive at what is now inside the src folder
- change `inthandler.c` by commenting out vector 149 to
  `//void INT_Excep_PERIB_INTB149(void){ }`.

**make the HardwareDebug settings:**
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

**Debug/Run:**
- ^B (build project), F11 (Debug), _Confirm Perspective Switch_ Yes >
  F8 (Resume), F8  
- ^F2 will terminate the debug session

**result:**
 - 8 files in "generate" folder, of which 1 line was commented out,
 - 1 file in "src" folder has all code that matters for the developer
 - prog: 2676
 - constant: 1176
 - data: 0
 - bss: 4
 - other: 28

The next project will use the PLL to get an instruction clock cycle of 32 MHz.
If you make a project with it you can get a feeling of how an RX100-series
ucon would perform, as that would have a maximum clock frequency of 32 MHZ.
When using the FPU, it can also be used to get a feeling of the performance
of an RX231 ucon with a clock speed setting of 32 MHz.
Of course both of the above ucons can have a supply voltage of 5V, useful
for direct driving of MOSFET's, but the RX65N has other useful features.
 