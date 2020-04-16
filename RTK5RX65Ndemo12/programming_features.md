programming features of this demo (12)
---

**main.c**:
1. iopin-library for driving LED1 and LED2
1. serial-library for using SCI-interfaces as UART using TxD and/or RxD pin
1. Arduino-style micros() and millis(), derived from timer CMT0

Setup to get started quickly:
1. If you are unsure about settings, it can be easier to first derive the 
   millis()-function from the right peripheral clock and only toggle the LEDs:
   - PCLKA for SCI10 and SCI11: use MTU for timing, like in demo10.
   - PCLKB for all the other SCI-interfaces: use TPU, like in demo11.
1. Start with only the TxD-pin and the write function to check if the settings 
   are right to get something received in the terminal program at your PC.
1. Finally add the RxD-pin and type something in the terminal program to be
   echoed.

**Cserial.h:**
1. fifo-macro's to shorten typework in the C-file
1. typedef to have `private` struct contents of Shwserial
1. declaration of the possible instances Serial0 upto Serial12.
   **Note:** Only Serial5 is instantiated in the library.
1. public as well as `protected` interrupt-functions to use.

**Cserial.c:**
1. #defines OBJx and CALLx to emphasize how to use the `objects`.
1. #defines for bits coupled to SCI-registers
1. Helper table and functions to set interrupt priorities for all four possible
   UART-mode interrupts.
1. Helper functions and data to ease with making SCI-settings
1. Union to contain pins used in different modes
1. Shwserial-struct with the first 4 members needing initalization to accomodate
   differemt hardware SCI-interfaces and settings, with SCI5 instantiated.
1. OBJ(init_uart, pclk) follows the hardware manuals' steps for initialization:
   1. (Added by me) Check that the SCI is meant to be used as a UART.
   2. Set the IO-pin directions and functions. I added a check that prevents
      the module ID from being returned if both TxD and RxD are used but  belonging to a different SCI-module.
   3. Release the module from stop-mode: apply clock signal to it.
   4. Make register settings for initialization, depending on input clock.
   5. Set MPC pin functions and make interrupt settings. I added a check that 
      prevents interrupts being used on unused TxD- or RxD- pins.
