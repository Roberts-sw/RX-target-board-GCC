RTK65RX65Ndemo05:
**functionality:** LED0 initially on, LED1 initially off, IRQ4 on SW1
- HOCO clock 16 MHz, PLL / 1 * 15 = 240 MHz
- ICLK / 2 = 120 MHz, PCLKB / 4 = 60 MHz
- main(): empty loop
- IRQ4 (SW1-press) toggles LED0, (SW1-release) toggles LED1

**skipping smart configurator:**
- in stead of selecting smart configurator, choose Finish >

**make the HardwareDebug settings:**
- Project > Properties >
- _Properties for RTK5RX65Ndemo05_
  - Run/Debug Settings > RTK5RX65Ndemo05 HardwareDebug > Edit... >
- _Edit launch configuration properties_
  - Debugger > Connection Settings >
    - Clock: HOCO
    - Connection with Target Board: change dropdown selection JTag to Fine
    - Power: change dropdown selection Yes to No
  - OK > Apply and Close >
- in folder src rename the file RKT5RX65Ndemo05.c to main.c
- Control-B to build the project, or: Project > Build Project >

**taking over code from RTK65RX65Ndemo04:**
- copy main.c to this project folder src
- edit some things to arrive at what is now inside the src folder
- change `inthandler.c` by commenting out vector 68 to
  `//void INT_Excep_ICU_IRQ4(void){ }`.

**Debug/Run:**
- ^B (build project), F11 (Debug), _Confirm Perspective Switch_ Yes >
  F8 (Resume), F8  
- ^F2 will terminate the debug session

**result:**
 - prog: 2828
 - constant: 1176
 - data: 0
 - bss: 8
 - other: 28

**questions:**
1. Why doesn't LED0 toggle anymore if the value in line 111 is changed
   from eBOTH to eRISE? What happens if we change it to eFALL?
   - Hint: set a breakpoint inside the interrupt routine and check when
     the program arrives at that point.
