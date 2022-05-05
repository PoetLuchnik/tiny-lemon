# tiny-lemon
## memory
### RAM
256 bytes
### Stack
256 bytes
### registers
8 registers each 1 byte
## cpu
### registers
name | full name | description
--- | --- | ---
A | accumulator | general purpose
D | D register | general purpose
F | flags | every bit means something
I | input port index | for the input callback
K | K register | general purpose
O | output port index | for the output callback
P | program counter | stores the address of the next instruction-byte
S | stack pointer | stores the address of the top of the stack
### F register
#### bits
n | value | name | full name | description
--- | --- | --- | --- | ---
1 | 1 | Z | zero | result of operation is zero
2 | 2 | C | carry | result of operation exceeds byte limit (from any side)
3 | 4 | N | negative | result of operation is negative
4 | 8 | O | over signed | negative number + negative number = positive number or vice versa
5 | 16 | I | no interrupt | disables optional interrupts 
6 | 32 | B | break | BRK operation sets it
7 | 64 | S | stack overflow | push & pop operations set it if stack overflow happens
8 | 128 | U | unused | may be used in the future
### address modes
name | address from | value from | P 
--- | --- | --- | --- 
`none` |  | `A` |  
`__A` | `A` | `RAM[address]` |  
`__D` | `D` | `RAM[address]` |  
`__K` | `K` | `RAM[address]` |  
`__P` | `P` | `RAM[address]` | `P + 1`
`_LA` | `RAM[A]` | `RAM[address]` |  
`_LD` | `RAM[D]` | `RAM[address]` |  
`_LK` | `RAM[K]` | `RAM[address]` |  
`_LP` | `RAM[P]` | `RAM[address]` | `P + 1`
### operations
a - address from **address mode**

v - value from **address mode**

r - any register

f - bit-flag

b - T or F (true or false)
#### table
name | full name | do
--- | --- | --- 
NOP | nope | nothing
MVx | move | A = x; where x is D, F, I, K, O, P or S
LDr | load | r = v
STA | store A | RAM\[a\] = A
XCr | exchange | swap r with A
XCB | exchange byte | swap RAM\[a\] with A
ZRr | zero | r = 0
ZRB | zero byte | RAM\[a\] = 0
Fbf | set flag | f = b
Jbf | jump if | if f is b then P = a
JMP | jump | P = a
JSR | jump to subroutine | push P and P = a
LUP | loop | A - 1 and if A != 0 then P = a
PSR | push registers | push: A, D, I, K, O, F
PPR | pop registers | pop:  F, O, K, I, D, A
RTI | return from interrupt | pop P and pop: F, O, K, I, D, A
BRK | break | do interrupt and flag B = ture
INA | input A | calls input callback
OUT | output A | calls output callback
HLT | halt | P - 1
PSr | push | push r
PSH | push byte | push v
PPr | pop | pop r
POP | pop byte | pop RAM\[a\]
PKr | peek | peek r
PIK | peek byte | peek RAM\[a\]
CMP | compare | set flags by result of subtracting: A - v
TST | test | set flags by result of bitwise-AND: A & v
ADD | add | A += v
ADC | add with carry | A += v + flag C
SUB | subtract | A -= v
SBC | subtract with carry | A -= v + 1 - flag C
AND | bitwise AND | A = A and v
ORR | bitwise OR | A = A or v
NOR | bitwise NOR | A = A nor v
XOR | bitwise XOR | A = A xor v
ICx | increment | x + 1; where x is A, D or K
DCx | decrement | x - 1; where x is A, D or K
NTA | bitwise not A | A = not A
NGA | negative A | A = -A
INC | increment byte | RAM\[a\] + 1
DEC | decrement byte | RAM\[a\] - 1
NOT | bitwise not byte | RAM\[a\] = not RAM\[a\]
NEG | negative byte | RAM\[a\] = -RAM\[a\]
SHL | bitwise shift left | A << 1
SHR | bitwise shift right | A >> 1
SLC | bitwise shift left with carry | A >> 1 and A + flag C * 128
SRC | bitwise shift right with carry | A << 1 and A + flag C
### instructions
instruction is combination of some operation and some address mode
#### table
 \ | 00 | 01 | 02 | 03 | 04 | 05 | 06 | 07 | 08 | 09 | 0A | 0B | 0C | 0D | 0E | 0F
--- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | ---
00 | NOP <br/> none | MVD <br/> none | MVF <br/> none | MVI <br/> none | MVK <br/> none | MVO <br/> none | MVP <br/> none | MVS <br/> none | LDA <br/> none | LDD <br/> none | LDF <br/> none | LDI <br/> none | LDK <br/> none | LDO <br/> none | LDP <br/> none | LDS <br/> none
10 | LDA <br/> \_\_A | LDA <br/> \_LA | LDA <br/> \_\_D | LDA <br/> \_LD | LDA <br/> \_\_K | LDA <br/> \_LK | LDA <br/> \_\_P | LDA <br/> \_LP | STA <br/> \_\_A | STA <br/> \_LA | STA <br/> \_\_D | STA <br/> \_LD | STA <br/> \_\_K | STA <br/> \_LK | STA <br/> \_\_P | STA <br/> \_LP 
20 | XCA <br/> none | XCD <br/> none | XCF <br/> none | XCI <br/> none | XCK <br/> none | XCO <br/> none | XCP <br/> none | XCS <br/> none | XCB <br/> \_\_A | XCB <br/> \_LA | XCB <br/> \_\_D | XCB <br/> \_LD | XCB <br/> \_\_K | XCB <br/> \_LK | XCB <br/> \_\_P | XCB <br/> \_LP
30 | ZRA <br/> none | ZRD <br/> none | ZRF <br/> none | ZRI <br/> none | ZRK <br/> none | ZRO <br/> none | ZRP <br/> none | ZRS <br/> none | ZRB <br/> \_\_A | ZRB <br/> \_LA | ZRB <br/> \_\_D | ZRB <br/> \_LD | ZRB <br/> \_\_K | ZRB <br/> \_LK | ZRB <br/> \_\_P | ZRB <br/> \_LP
40 | FFZ <br/> none | FFC <br/> none | FFN <br/> none | FFO <br/> none | FFI <br/> none | FFB <br/> none | FFS <br/> none | FFU <br/> none | FTZ <br/> none | FTC <br/> none | FTN <br/> none | FTO <br/> none | FTI <br/> none | FTB <br/> none | FTS <br/> none | FTU <br/> none 
50 | JFZ <br/> \_LP | JFC <br/> \_LP | JFN <br/> \_LP | JFO <br/> \_LP | JFI <br/> \_LP | JFB <br/> \_LP | JFS <br/> \_LP | JFU <br/> \_LP | JTZ <br/> \_LP | JTC <br/> \_LP | JTN <br/> \_LP | JTO <br/> \_LP | JTI <br/> \_LP | JTB <br/> \_LP | JTS <br/> \_LP | JTU <br/> \_LP 
60 | JMP <br/> \_\_A | JMP <br/> \_LA | JMP <br/> \_\_D | JMP <br/> \_LD | JMP <br/> \_\_K | JMP <br/> \_LK | JMP <br/> \_\_P | JMP <br/> \_LP | JSR <br/> \_\_A | JSR <br/> \_LA | JSR <br/> \_\_D | JSR <br/> \_LD | JSR <br/> \_\_K | JSR <br/> \_LK | JSR <br/> \_\_P | JSR <br/> \_LP
70 | LUP <br/> \_\_A | LUP <br/> \_LA | LUP <br/> \_\_D | LUP <br/> \_LD | LUP <br/> \_\_K | LUP <br/> \_LK | LUP <br/> \_\_P | LUP <br/> \_LP | PSR <br/> none | PPR <br/> none | RTI <br/> none | BRK <br/> none | INA <br/> none | OUT <br/> none | HLT <br/> none
80 | PSA <br/> none | PSD <br/> none | PSF <br/> none | PSI <br/> none | PSK <br/> none | PSO <br/> none | PSP <br/> none | PSS <br/> none | PSH <br/> \_\_A | PSH <br/> \_LA | PSH <br/> \_\_D | PSH <br/> \_LD | PSH <br/> \_\_K | PSH <br/> \_LK | PSH <br/> \_\_P | PSH <br/> \_LP
90 | PPA <br/> none | PPD <br/> none | PPF <br/> none | PPI <br/> none | PPK <br/> none | PPO <br/> none | PPP <br/> none | PPS <br/> none | POP <br/> \_\_A | POP <br/> \_LA | POP <br/> \_\_D | POP <br/> \_LD | POP <br/> \_\_K | POP <br/> \_LK | POP <br/> \_\_P | POP <br/> \_LP
A0 | PKA <br/> none | PKD <br/> none | PKF <br/> none | PKI <br/> none | PKK <br/> none | PKO <br/> none | PKP <br/> none | PKS <br/> none | PIK <br/> \_\_A | PIK <br/> \_LA | PIK <br/> \_\_D | PIK <br/> \_LD | PIK <br/> \_\_K | PIK <br/> \_LK | PIK <br/> \_\_P | PIK <br/> \_LP
B0 | CMP <br/> \_\_A | CMP <br/> \_LA | CMP <br/> \_\_D | CMP <br/> \_LD | CMP <br/> \_\_K | CMP <br/> \_LK | CMP <br/> \_\_P | CMP <br/> \_LP | TST <br/> \_\_A | TST <br/> \_LA | TST <br/> \_\_D | TST <br/> \_LD | TST <br/> \_\_K | TST <br/> \_LK | TST <br/> \_\_P | TST <br/> \_LP
C0 | ADD <br/> \_\_P | ADC <br/> \_\_P | SUB <br/> \_\_P | SBC <br/> \_\_P | AND <br/> \_\_P | ORR <br/> \_\_P | NOR <br/> \_\_P | XOP <br/> \_\_P | ADD <br/> \_LP | ADC <br/> \_LP | SUB <br/> \_LP | SBC <br/> \_LP | AND <br/> \_LP | ORR <br/> \_LP | NOR <br/> \_LP | XOP <br/> \_LP
D0 | ICA <br/> none | DCA <br/> none | NTA <br/> none | NGA <br/> none | SHL <br/> none | SLC <br/> none | SHR <br/> none | SRC <br/> none | INC <br/> \_LP | DEC <br/> \_LP | NOT <br/> \_LP | NEG <br/> \_LP | ICD <br/> none | DCD <br/> none | ICK <br/> none  | DCK <br/> none
E0 | 
F0 | 

## functions
### void tlOnClockFunc(TLonClockFunc callback)
sets `onclock` callback

`TLonClockFunc` defined type: (TLbyte pc) -> void
### void tlInputFunc(TLinputFunc callback)
sets `input` callback

`TLinputFunc` function type: (TLbyte inputport) -> TLbyte
### void tlOutputFunc(TLoutputFunc callback)
sets `output` callback

`TLoutputFunc` function type: (TLbyte value, TLbyte outputport) -> void
### void tlRun()
starts cpu loop with infinite clocking
#### every clock
1) calls `onclock` callback
2) load instruction at P
3) P + 1
4) gets address from instruction address mode
5) gets value at address
6) do instruction operation
7) clock_counter + 1
### void tlReset()
all register = 0

clock counter = 0
### TLbyte tlGetByte(TLbyte address)
load byte from RAM

*address - address of byte*

`TLbyte` is 8-bit unsigned integer
### void tlSetByte(TLbyte address, TLbyte value)
store value to RAM

*address - address of byte*

*value - value to store*
### TLbyte tlStackGetByte(TLbyte address)
load byte from Stack Memory

*address - address of byte*
### void tlStackSetByte(TLbyte address, TLbyte value)
store value to Stack Memory

*address - address of byte*

*value - value to store*
### TLbool tlGetFlag(TLflag flag)
returns flag state

`TLflag` is enumerator: `TL_ZERO`, `TL_CARRY`, `TL_NEGATIVE`, `TL_OVERSIGNED`, `TL_NO_INTERRUPT`, `TL_BREAK`, `TL_STACK_OVERFLOW`, `TL_UNKNOWN`

`TLbool` is integer (0 = false, not 0 = true)

### void tlSetFlag(TLflag flag, TLbool value)
sets flag state
### void tlTryInterrupt()
calls `tlDoInterrupt()` if `I flag` is false

otherwise do nothing
### void tlDoInterrupt()
push A, D, I, K, O, F, P

P = interrupt address

*interrupt address stored in RAM\[0xFF\]*

F = 0

### TLbyte tlGetRegA() and D, F, I, K, O, P, S
get value of register
### void tlSetRegA(TLbyte value) and D, F, I, K, O, P, S
set value of register

### TLcounter tlGetClockCount()
returns `clock_counter`

`TLcounter` defined type: `size_t`

