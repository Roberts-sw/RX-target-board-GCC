RTK65RX65Ndemo06:
**functionality:** LED0 initially on, LED1 initially off, IRQ4 on SW1
- HOCO clock 16 MHz, PLL / 1 * 15 = 240 MHz
- ICLK / 2 = 120 MHz, PCLKB / 4 = 60 MHz
- main(): empty loop
- IRQ4 (SW1-press) toggles LED0, (SW1-release) toggles LED1

### create the project with GCC:
### make the HardwareDebug settings:
### Debug/Run:
take a look at `readme.md` of previous demo-projects or
see [GCC_e2studio_RX65N.md](https://github.com/Roberts-sw/RX-target-board-GCC/blob/master/GCC_e2studio_RX65N.md)

**taking over code from RTK65RX65Ndemo05:**
- copy main.c to this project folder src
- edit some things to arrive at what is now inside the src folder
- change `inthandler.c` by commenting out vector 68 to
  `//void INT_Excep_ICU_IRQ4(void){ }`.
- add files `iopin.c` and `iopin.h`

**result:**
 - prog: 4220
 - constant: 1334
 - data: 0
 - bss: 0
 - other: 28

**questions:**
1. Why doesn't LED0 toggle anymore if the value in line 111 is changed
   from eBOTH to eRISE? What happens if we change it to eFALL?
   - Hint: set a breakpoint inside the interrupt routine and check when
     the program arrives at that point.
