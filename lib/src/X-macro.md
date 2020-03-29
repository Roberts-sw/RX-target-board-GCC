Steps for getting the X-macro from RX65N-QFP100.csv:
---
1. Open "RX65N-QFP100.csv" in a spreadsheet program:
   a. choose comma as column separator
   b. set formatting type of column C: "I/O Port" to text
   c. click OK to import the file into a new spreadsheet.
   d. save the file as "RX65x100_X.csv"
   e. select the whole sheet with Ctrl+A
   f. assign an auto filter to this data
   g. set the filter of column D: "init" to "empty"
   h. delete the lines that are still visible, except for line 1
   i. remove the auto filter to have the other lines visible again
   j. save the file again (as "RX65x100_X.csv").
2. Open "RX65x100_X.csv" in a spreadsheet or continue from step 1.: 
   a. don't forget to have column C: "I/O Port" formatted as text
   b. optional step: remove columns you don't need
      - column M for the interrupt input pins
      - column L for the LCD controller pins
      - column K for multi-bit serial interfaces: QSPI SDHI SDSI MMCIF
	  - column J for other communication peripherals
	  - column I for timer peripherals
	  - column H for external bus controller
	  - column G for the RX65N cloud option board
	  - column F for documentation hints
	  - column E for the RX65N target board
   c. optional: change column D initialisation to values for your own board.
   d. remove column B as this column annoys the precompiler.
   e. save the file as a "RX65x100_X.h" with a comma as field separator.
3. Open "RX65x100_X.h" in a plain-text editor or code editor:
   a. remove all commas behind `\' as in lines 16 38 62 and 84 (4 lines)
   b. remove the backslash `\' from line 84
   c. replace line ends commas with backslash for line continuation (79 lines)
   c. surround line 2 with comments like line 1, ending before the backslash
   d. change all `#' into `n' within the macro
   e. insert a new first line with the text "#define CHIP(X)\"
   f. optionally insert tabs/spaces for visual formatting:
      - I started with tabs before the commas
      - then inserting a space after `(' except for pin 100 and above
      - then manual changes and at the end converting tabs to spaces
   f. save the file as "RX65x100_X.h".

Reason to format the header like in step 3.f:
- separator comma after whitespace is more visible then at field ending,
  especially when in step 2 not all columns E..M were removed.
- pins possibly used with analog functionality have longer lines
