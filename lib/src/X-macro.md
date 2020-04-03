Steps for getting the X-macro from RX65N-QFP100.csv:
---
1. Open "RX65N-QFP100.csv" in a spreadsheet program:
   1. choose comma as column separator
   1. set formatting type of column C: "I/O Port" to text
   1. click OK to import the file into a new spreadsheet.
   1. save the file as "RX65x100_X.csv"
   1. select the whole sheet with Ctrl+A
   1. assign an auto filter to this data
   1. set the filter of column D: "init" to "empty"
   1. delete the lines that are still visible, except for line 1
   1. remove the auto filter to have the other lines visible again
   1. save the file again (as "RX65x100_X.csv").
2. Open "RX65x100_X.csv" in a spreadsheet or continue from step 1.: 
   1. don't forget to have column C: "I/O Port" formatted as text
   1. optional step: remove columns you don't need
      - column M for the interrupt input pins
      - column L for the LCD controller pins
      - column K for multi-bit serial interfaces: QSPI SDHI SDSI MMCIF
	  - column J for other communication peripherals
	  - column I for timer peripherals
	  - column H for external bus controller
	  - column G for the RX65N cloud option board
	  - column F for alternate MPC pin function init
	  - column E for the RX65N target board
   1. optional: change column D initialisation to values for your own board.
   1. remove column B as this column annoys the precompiler.
   1. save the file as a "RX65x100_X.h" with a comma as field separator.
3. Open "RX65x100_X.h" in a plain-text editor or code editor:
   1. remove all commas behind `\' as in lines 16 38 62 and 84 (4 lines)
   1. remove the backslash `\' from line 84
   1. replace line ends commas with backslash for line continuation (79 lines)
   1. surround line 2 with comments like line 1, ending before the backslash
   1. change all `#' into `n' within the macro
   1. insert a new first line with the text "#define CHIP(X)\"
   1. optionally insert tabs/spaces for visual formatting:
      - I started with tabs before the commas
      - then inserting a space after `(' except for pin 100 and above
      - then manual changes and at the end converting tabs to spaces
   1. save the file as "RX65x100_X.h".

Reason to format the header like in the last step of 3:
- separator comma after whitespace is more visible then at field ending,
  especially when in step 2 not all columns E..M were removed.
- pins possibly used with analog functionality have longer lines
