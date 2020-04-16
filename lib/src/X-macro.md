Steps for getting the X-macro from RX65N-QFP100.csv:
---
1. Open "RX65N-QFP100.csv" in a spreadsheet program:
   1. choose comma as column separator
   1. set formatting type of the second column: "I/O Port" to text
   1. click OK to import the file into a new spreadsheet.
   1. save the file as "RX65x100_X.csv"
   1. select the whole sheet with Ctrl+A
   1. assign an auto filter to this data
   1. set the filter of column C: "init" to "empty"
   1. delete the lines that are still visible, except for line 1
   1. remove the auto filter to have the other lines visible again
   1. save the file (again) as "RX65x100_X.csv".
2. Open "RX65x100_X.csv" in a spreadsheet or continue from step 1.: 
   1. don't forget to have column C: "I/O Port" formatted as text
   1. optional step: remove columns you don't need, columns are:
      - column N for the interrupt input pins
      - column M for the LCD controller pins
      - column L for multi-bit serial memory interfaces: QSPI SDHI SDSI MMCIF
	  - column K for other communication peripherals
	  - column J for timer peripherals
	  - column I for external bus controller
	  - column H for Power Supply Clock System Control
	  - column G for the RX65N cloud option board
	  - column F for the RX65N target board
	  - column E: (iopin.h) MPC pin function init for the peripheral pin.
	  - column D: (iopin.h) module stop for using the peripheral pin.
	  - column C: (iopin.h) input/output initialisation
   1. optional: change column D initialisation to values for your own board.
   1. save the file as a "RX65x100_X.h" with a comma as field separator.
3. Open "RX65x100_X.h" in a plain-text editor or code editor:
   1. remove all commas behind `\' as in lines 16 38 62 and 84 (4 lines)
   1. remove the backslash `\' from line 84
   1. insert a new first line with the text "#define CHIP(X)\"
   1. optionally insert tabs/spaces for visual formatting:
      - insert a space afer `(`, except for pin 100, double space for pin 
        numbers below 10.
	  - line out columns with tabs, manual changes and convert tabs
	    to spaces.
   1. save the file as "RX65x100_X.h".

Reason to format the header like in the last step of 3:
- separator comma after whitespace is more visible then at field ending,
  especially when in step 2 not all columns F..N were removed.

###Background###
For the RX65N cloud kit I used the hardware manual, looking in the tables 
 from HW 1.5 Pin Assignments, I chose 
 _Table 1.10 List of Pin and Pin Functions (100-Pin LFQFP)_,
 as this is for the chip used in the RX65N target board.

Possible steps for making a file like RX65N-QFP100.csv:
1. Copy page by page into the spreadsheet, I also used a text editor.
1. Switch columns for Power Supply Clock System Control and I/O Port, as
   the I/O Port name will be used as an identifier. Now column C is for 
   "Power Supply Clock System Control", and new columns will be inserted
   before this one.
1. Insert columns for the hardware you will be using with this chip. 
   For the github examples using the cloud kit, I inserted "target board" 
   and "option board". 
   
   Use an autoufilter on column B to get non-empty lines and put a '-' in
   each cell that belongs to an I/O Port pin. This will keep the 
   preprocessor macro handler happy when working with the file. 
   
   Fill in functions for the pins to be used on the boards.
   
1. Remove the autofilter to be able to insert more columns. 
   For me it seemed reasonable to have data for:
   - init: I/O-pin initialisation including open-drain and drive strength.
   - MSTP: the module to be taken out of stop before using the pin for a
     peripheral function.
   - MPC: the function to assign to the pin after peripheral initialisation.
   
   Use the autofilter again to fill in the cells. I started with IN_LO for
   init of all (unused) pins, NO for not using periperal functions and 0 for
   the MPC-functions, and then change cells depending on the board used. 
   
1. For clarity, I added lines after each corner of the QFP-chip, in this case 
   after pins 25, 50, 75, and 100.
   
1. Save the file as a .csv file, use an editor to enclose each pin line in
   `X( )\` and each non-pin line in `/* */\`, and replace all `#` by `n`.
