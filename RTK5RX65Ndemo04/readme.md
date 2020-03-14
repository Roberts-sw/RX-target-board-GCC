RTK65RX65Ndemo04:
**functionality:** sw-toggle: LED0 250.000 us on-off, LED1 250 ms off-on
- HOCO clock 16 MHz, PLL / 1 * 15 = 240 MHz
- ICLK / 2 = 120 MHz, PCLKB / 4 = 60 MHz
- TMR0 source PCLKB, CMIA0: 60 no interrupt => 1 MHz
- TMR1 source CMIA0, CMIA1: 250 + interrupt, vector INTB149 => 2kHz
- INTB149 acc_4kHz count + increment Micros, div 4 = Millis 
- main(): time polling with micros() and millis()

**skipping smart configurator:**
- in stead of selecting smart configurator, choose Finish >

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

**taking over code from RTK65RX65Ndemo03:**
- copy main.c to this project folder src
- edit some things to arrive at what is now inside the src folder
- change `inthandler.c` by commenting out vector 149 to
  `//void INT_Excep_PERIB_INTB149(void){ }`.

**Debug/Run:**
- ^B (build project), F11 (Debug), _Confirm Perspective Switch_ Yes >
  F8 (Resume), F8  
- ^F2 will terminate the debug session

**result:**
 - 8 files in "generate" folder, of which 1 line was commented out,
 - 1 file in "src" folder has all code that matters for the developer
 - prog: 3004 or 3008, depending on TEST value and code in TEST 0.
 - constant: 1176
 - data: 0
 - bss: 20
 - other: 28

**note:**
This project serves to gain some feeling about which integer sizes to use for
RAM-restricted programs (I know, this one has 640 kB of it), and why you would
sometimes use unsigned and signed integers of differing sizes. With a `#define`
statements to be set for variations in functionality, you can debug and try to
explain in which way and why the behaviour differs with the chosen setting.

**questions:**
1. Why does TEST with calue 0 not work, and why does it work with the other
   values 1, 2, and 3?
   - Hints:
     1. Why is an unsigned 8-bit difference greater than 6 used for toggling
        the LED output and incrementing the 8-bit time value?
        - NB: a value of 256 will be understood as 0 when stored in 8 bits.
     2. Why does a signed 16-bit difference suffice to toggle the LED output
        and increment the 16-bit time value? The millis()-value is unsigned
        32-bit, so what will happen after half a minute, or a minute, when the
        16-bit time value can't store the millis()-value anymore?
        - NB: type promotion will extend the stored 16-bit time value to 32-bit
          before subtracting it from the 32-bit millis()-value, and then a
          type-cast (or bit cutt-off) to 16-bit will be done before regarding
          the resulting value as a signed value.
     3. Why is an unsigned 16-bit difference greater than 65286 used for
        toggling the LED output and incrementing the 16-bit time value?
        - NB: a value of 65536 will be understood as 0 when stored in 16 bits.
   - The above hints can be understood better when thinking of the numbers
     within a given size written on a disc that turns around 1 number every
     milli-second. Compare it to a decimal number with 1 or 2 digits, counting
     up and arriving back at zero after 1 (or 2 in the latter case) 9's have
     displayed. For a signed number at the other side of the disc the numbers
     must be interpreted as negative, so 5 in the decimal case would be read
     as -5, 6 as -4 and so on.
2. Why does TEST with value 0 not work?
   - Try to do the calculation `millis() - t_LED1` by hand, starting with
     t_LED1=0 and Millis=0. The first test will increase t_LED1 by 250, what
     is happening then when Millis is still 0, or when it is 1? Go on until
     Millis=7, and try to discover what the ucon `sees` as a result of the
     calculation after casting it to int8_t.
3. Why does TEST with value 0 work when the test is expanded to
   `if(0>dt1||6<=dt1)`?
   - Hints: see the test with TEST-value 1 as an example, read the text above
     about signed numbers and determine when the difference will be seen as
     negative or non-negative.
