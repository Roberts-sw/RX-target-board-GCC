RTK65RX65Ndemo00_0:
- functionality:
  - LOCO clock 240 kHz
  - TMR1 source PCLKB, CMIA1: 240 + interrupt, vector INTB149
  - INTB149 Millis-count, div 500 = toggle LED1 (PORTD bit 7)
- using smart configurator:
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
- add code:
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
- result:
  - 128 files in 31 folders, excluding documentation
  - prog: 15648
  - constant: 1504
  - data: 12
  - bss: 2116
  - other: 5404
