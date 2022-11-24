# Standard Mapping on the HACK Platform

Describes the manner in which the JACK Virtual Machine maps to the HACK CPU.

## JACK Virtual Machine Language

Supports 17 commands:

1) `push <memory> <offset>`
	- int16_t
2) `pop <memory> <offset>`
	- int16_t
3) Arithmetic commands
	- `add`
	- `sub`
	- `neg`
	- `eq`
		- false = 0; true != 0
	- `gt`
		- false = 0; true != 0
	- `lt`
		- false = 0; true != 0
	- `and`
	- `or`
	- `not`
4) `label <name>`
5) `goto <label>`
6) `if-goto <label>`
7) `function <name> <nVars>`
8) `return`
9) `call <function> <nArgs>`

Memory segments:

- SP
- LCL
- ARG
- THIS
- THAT
- Pointer
- Constant
- Static
- Temp

## HACK CPU

Harvard architecture with 2 GPRs.

Picture of computer
Picture of CPU

## Mapping

SP = RAM[0] (initialized to 256)
LCL = RAM[1]
ARG = RAM[2]
THIS = RAM[3]
THAT = RAM[4]
Temp = RAM[13-15]
Static = RAM[16-255]