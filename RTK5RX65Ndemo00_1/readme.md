RTK65RX65Ndemo00_1
---
**functionality:** LED1 500 ms on, 500 ms off
- LOCO clock 240 kHz
- TMR1 source PCLKB, CMIA1: 240 + interrupt, vector INTB149 => 1kHz
- INTB149 Millis-count, div 500 = toggle LED1 (PORTD bit 7) => 2 Hz

**skipping smart configurator**
- in stead of selecting smart configurator, choose Finish >

**taking functional code from RTK65RX65Ndemo00_0:** 
- look at what start.S (assembly language) is doing.
  It can be read form left to reight, so `mov #_data, r1` means
  `move the number represented by _data to register r1`.
- Discover that, apart from assembly, it only calls 2 external routines
  by putting the address in register r7 and then performing `jsr r7`:
  1. HardwareSetup(), located in hwinit.c and without actual code.
  1. main(), located in the project names .c file, and also still empty.
- Now by going back to project RTK65RX65Ndemo00_0 and stepping through the 
  code with the debugger, in stead of pressing Run, one can discover what
  has to be done to initialize the controller.
- I already have put the code in the .c file and changed the comment on top
  of the file to define the test setup and state the test date.
- In order to ease searching for code I also put the interrupt vector code
  as `protected` code behind the main() function.
  On compilation with ^B the project will fail, as the interrupt vector is
  also defined in `inthandler.c`, so I had to change line 381 of that file
  to `//void INT_Excep_PERIB_INTB149(void){ }` to comment the vector there.
- rename the file RKT5RX65Ndemo00_1.c to main.c

**make the HardwareDebug settings:**
- Project > Properties >
- _Properties for RTK5RX65Ndemo00_1_
  - Run/Debug Settings > RTK5RX65Ndemo00_1 HardwareDebug > Edit... >
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
- prog: 3272 (was 15648 with smart configurator)
- constant: 1176 (was 1504)
- data: 0 (was 12)
- bss: 4 (was 2116)
- other: 28 (was 5404)

**note:**
If debugging and repeatedly choosing "Step Into" only 3 source files will open:
1. `start.S`: startup code after reset in assembly language
2. `hwinit.c`: containing the empty `HardwareSetup()` function
3. `main.c`: containing the functional code

One can see that by looking into the hardware manual it is possible,
with some more effort, to compact the code a lot **and** gain some insight
in what is happening.
I would advise you as a reader to take this project and single-step through
its code while having the hardware manual alongside, just to understand why
some code is there.
  
The next project will do clock switching to a default speed of 16 MHz,
but use even less program memory for the same LED functionality, all
without touching compiler optimization settings.
