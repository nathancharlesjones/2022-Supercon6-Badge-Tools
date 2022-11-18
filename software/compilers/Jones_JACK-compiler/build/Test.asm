// Push LCL 2
@2		// Load D with value to push
D=A
@LCL
A=M
A=D+A
D=M
@SP		// A = 0 / M = SP
A=M		// A = SP / M = TOS
M=D		// TOS <-- D
@SP
M=M+1		// SP++
// POP ARG 10
@10		// Compute dest. memory address
D=A
@ARG
A=M
D=D+A
@SP		// Store temporarily at TOS
A=M		// A = SP / M = TOS
M=D
A=A-1		// A = SP - 1 / M = *(TOS - 1)
D=M		// Load D with TOS - 1
A=A+1		// A = SP / M = TOS
A=M		// Load A with memory + offset
M=D
@SP
M=M-1		// SP--
// add
@SP		// Load D with TOS - 1
M=M-1		// SP--
A=M		// A = SP / M = TOS
D=M
A=A-1		// Point A at TOS - 2
M=D+M
