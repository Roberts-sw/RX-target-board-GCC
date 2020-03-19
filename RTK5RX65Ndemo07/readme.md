RTK65RX65Ndemo07:
**functionality:** LED0 initially on, LED1 initially off, IRQ4 on SW1
- HOCO clock 16 MHz, PLL / 1 * 15 = 240 MHz
- ICLK / 2 = 120 MHz, PCLKB / 4 = 60 MHz
- main(): empty loop
- IRQ4 (SW1-press) toggles LED0, (SW1-release) toggles LED1

### create the project as a C++-project with GCC:
For example, create project RTK5RX65Ndemo07
- File > New > C/C++ Project > 
- _Templates for New C/C++ Project_
  - Renesas RX > 
  - GCC for Renesas RX C/C++ Executable Project > 
  - Next >
- _New GCC for Renesas RX Executable Project_
  - Project name: RTK5RX65Ndemo07
  - Next >
- _Select toolchain, device & debug settings_
  - at Toolchain settings, for language, tick `C++`
  - Target Device: ... > RX600 > RX65N > RX65N - 100 pin > R5F565NEDxFP_DUAL
  - change dropdown selection of `E1 (RX)` to `E2 Lite (RX)`
  - select Endian setting: Big or Little (default)
  - Next >
- _Select Coding Assistant Settings_
  - Next >
- _Select Additional CPU Options_
  - Next >
- _Select library generator settings_
  - Next >
- _Summary of project "RTK5RX65Ndemo07"_
  - Finish >

### make the HardwareDebug settings:
### Debug/Run:
take a look at `readme.md` of previous demo-projects or
see [GCC_e2studio_RX65N.md](https://github.com/Roberts-sw/RX-target-board-GCC/blob/master/GCC_e2studio_RX65N.md)

**taking over code from RTK65RX65Ndemo06:**
- rename `RTK65RX65Ndemo06.cpp` to `main.cpp`, copy contents of `main.c` from last demo.
- copy files `iopin.c` and `iopin.h` to this project folder src
- change `inthandler.c` by commenting out vector 68 (line 147) to
  `//void INT_Excep_ICU_IRQ4(void){ }`.

### converting the project afterwards to C++:
In case you forgot to create the project as a C++-project, it has to be
converted to a C++-project:
- File > New > Convert to a C/C++ Project (adds C/C++ Nature) >
  - _Convert to a C/C++ Project_
  - select `RTK65RX65Ndemo07`, choose Finish >
- change file name of `main.c` to `main.cpp`

Now when using statis objects, a build will probably give you error messages,
stating something with **undefined reference to `__dso_handle'**.

In order to get a proper build you need to add an **assembler**-setting,
because the code in `start.S` needs to know there are static C++-objects to
be able to initialize them:
- Project > Properties > 
- _Properties for RTK5RX65Ndemo07_
  - C/C++ Build > Settings > Assembler > Source >
    - In `User defined options` click on the green `+` and enter the text `-DCPPAPP`.
    - Confirm with OK > Apply and Close >

### Differences with C-program demo06:
1. `#include "iopin.h"` is surrounded with with a predecessor line
   `extern "C" {` and a successor line `}`, as is
   `#include "interrupt_handlers.h"`, as both are header files for C-routines.
   The compiler will use `C` call conventions for the functions in these header
   files.
2. Preprocessor #define statements for the pins are replaced by objects as
   class instances, as can be seen in lines 32, 33 and 36.
3. The iopin_init()-functions have been replaced by object methods(),
   see lins 75-77 and 87.
4. The IRQ4-interrupt routine, although called with `C` conventions, is a
   C++-routine, as it can directly call the objects `read()`, `write()` and
   `toggle()` methods.

I left the original C statements as comments on the same lines, so it is
easier to compare them with their C++-counterpart.

**result:**
 - prog: 4860 (using commented version: 4404, demo06: 4220)
 - constant: 1380 (commented version: 1370, demo06: 1334)
 - data: 1068 (commented version: 4, demo06: 0)
 - bss: 16 (commented version: 4, demo06: 0)
 - other: 28 (unchanged)

It can be seen that this demo with C++-wrappers takes quite a bit more memory
than the C-only version, but the nice thing is that both can be used in the
same program, the run-time efficient C-functions, as well as the
typewriter-friendly C++-methods.
The program size has grown because the C++-wrappers, even if we don't use them 
take space as we still compile with `-O0` (optimization: **None**).

