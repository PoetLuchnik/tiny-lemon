# tiny-lemon
static library written in C programming language

represents a virtual imagined computer
* 256 bytes of RAM
* 256 bytes of Stack
* 8 registers (8 bits each)
* 9 address modes
* 132 operations
* 223 instructions
* von neumann architecture

library contains some functions to managing internal data
* get/set RAM bytes
* get/set Stack bytes
* get/set registers
* get clock counter
* set some callback functions

## why?

education - not the best for it, but you can study how processor works,
how to implement features wich no implemented by default,
how assembly languages works and how to create your own language

challenges - what do you think you can implement in 256 bytes of RAM?
hello world? cat program? 99 bottles of beer on the wall? calculator?
snake? tetris? pac-man? doom? skyrim? minecraft? OS? compiler? VM?

fun - have a good time doing some tricks and experiments

## how to begin?

you can immediately call `tlRun` function - it is starts CPU's loop

but all bytes of RAM equals 0x00 by default (NOP instruction) - BORING!

use `tlSetByte` function to load your program - byte by byte

for example: 0x7C 0x7D 0x67 0x00

this program read 1 byte from input port 0, write it's value to output port 0, and jump to 0x00 - and never stops

run it and oops - nothing happens

you need to write own functions for input and output

how about that:
```c
 TLbyte my_tlin(TLbyte inputport) {
 	unsigned int n = 0;
 	return scanf("%u", &n) ? n : 0;
 }
 void my_tlout(TLbyte value, TLbyte outputport) {
 	printf("%u\n", value);
 }
```
use `tlInputFunc` and `tlOutputFunc` to set callbacks

well done! see more in header file
