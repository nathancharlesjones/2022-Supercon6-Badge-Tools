// push LCL 2
@LCL		// Load D with value to push
D=M
@2
A=D+A
D=M
@SP		// A = 0 / M = SP
M=M+1		// SP++
A=M-1		// A = old SP / M = TOS - 1
M=D		// (TOS - 1) <-- D
// push LCL 3
@LCL		// Load D with value to push
D=M
@3
A=D+A
D=M
@SP		// A = 0 / M = SP
M=M+1		// SP++
A=M-1		// A = old SP / M = TOS - 1
M=D		// (TOS - 1) <-- D
// eq
@SP		// Load D with TOS - 1
M=M-1		// SP--
A=M		// A = SP / M = TOS
D=M
A=A-1		// Point A at TOS - 2
D=D-M
@EQUAL
D;JEQ
(NOT_EQUAL)
D=0
@DONE
0;JEQ
(EQUAL)
D=-1
@DONE
0;JEQ
(DONE)
@SP
A=M
A=A-1
M=D
// pop ARG 10
@ARG		// Compute dest. memory address
D=M
@10
D=D+A
@13
M=D		// Store dest. addr. in RAM[13] (temp reg)
@SP		// Load D with value at TOS
M=M-1		// SP--
A=M		// A = SP - 1 / M = *(TOS - 1)
D=M		// Load D with TOS - 1
@13
A=M		// Load A with memory + offset
M=D
// push constant 3
@3		// Load D with value to push
D=A
@SP		// A = 0 / M = SP
M=M+1		// SP++
A=M-1		// A = old SP / M = TOS - 1
M=D		// (TOS - 1) <-- D
// push constant 5
@5		// Load D with value to push
D=A
@SP		// A = 0 / M = SP
M=M+1		// SP++
A=M-1		// A = old SP / M = TOS - 1
M=D		// (TOS - 1) <-- D
// add
@SP		// Load D with TOS - 1
M=M-1		// SP--
A=M		// A = SP / M = TOS
D=M
A=A-1		// Point A at TOS - 2
M=D+M
// pop temp 0
@5		// Compute dest. memory address
D=A
@0
D=D+A
@13
M=D		// Store dest. addr. in RAM[13] (temp reg)
@SP		// Load D with value at TOS
M=M-1		// SP--
A=M		// A = SP - 1 / M = *(TOS - 1)
D=M		// Load D with TOS - 1
@13
A=M		// Load A with memory + offset
M=D
