#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CodeWriter.h"
#include "Parser.h"

/*
┌────┬─────┬─────┬─────┬─────┐
│Name│ SP  │     │     │ TOS │
├────┼─────┼─────┼─────┼─────┤
│Addr│  0  │ X-2 │ X-1 │  X  │
├────┼─────┼─────┼─────┼─────┤
│Data│ TOS │  #  │  #  │  0  │
└────┴─────┴─────┴─────┴─────┘
// &SP = 0
// int * SP = &TOS
// int TOS = 0  // *(&TOS-1) = top value
*/

typedef struct CodeWriter_t
{
	FILE 	*asm_file;
} CodeWriter_t;

int codeWriter_new(FILE * asm_file, CodeWriter * newCodeWriter)
{
	int err = 0;

	if( asm_file == NULL ) err = 1;

	if( err == 0 )
	{
		if( newCodeWriter == NULL ) err = 2;
	}

	if( err == 0 )
	{
		*newCodeWriter = calloc(1, sizeof(CodeWriter_t));
		if( *newCodeWriter == NULL ) err = 3;
		else (*newCodeWriter)->asm_file = asm_file;
	}
	
	return err;
}

int codeWriter_writeArithmetic(CodeWriter thisCodeWriter, char * op)
{
	int err = 0;

	if( thisCodeWriter == NULL ) err = 1;

	if( err == 0 )
	{
		if( op == NULL ) err == 2;
	}

	if( err == 0 )
	{
		if( strcmp(op, "add") == 0 ) fprintf(thisCodeWriter->asm_file,
			"// add\n"
			"@SP\t\t// Load D with TOS - 1\n"
			"M=M-1\t\t// SP--\n"
			"A=M\t\t// A = SP / M = TOS\n"
			"D=M\n"
			"A=A-1\t\t// Point A at TOS - 2\n"
			"M=D+M\n");
		else if( strcmp(op, "sub") == 0 ) fprintf(thisCodeWriter->asm_file,
			"// sub\n"
			"@SP\t\t// Load D with TOS - 1\n"
			"M=M-1\t\t// SP--\n"
			"A=M\t\t// A = SP / M = TOS\n"
			"D=M\n"
			"A=A-1\t\t// Point A at TOS - 2\n"
			"M=D-M\n");
		else if( strcmp(op, "neg") == 0 ) fprintf(thisCodeWriter->asm_file,
			"// neg\n"
			"@SP\t\t// A = 0 / M = SP\n"
			"A=M\t\t// A = SP / M = TOS"
			"A=A-1\t\t// Point A at TOS - 1\n"
			"M=-M\n");
		else if( strcmp(op, "eq") == 0 ) fprintf(thisCodeWriter->asm_file,
			"// eq\n"
			"@SP\t\t// Load D with TOS - 1\n"
			"M=M-1\t\t// SP--\n"
			"A=M\t\t// A = SP / M = TOS\n"
			"D=M\n"
			"A=A-1\t\t// Point A at TOS - 2\n"
			"D=D-M\n"
			"@EQUAL"
			"D;JEQ\n"
			"(NOT_EQUAL)\n"
			"D=0\n"
			"@DONE\n"
			"0;JEQ\n"
			"(EQUAL)\n"
			"D=-1\n"
			"@DONE\n"
			"0;JEQ\n"
			"(DONE)\n"
			"@SP\n"
			"A=M\n"
			"A=A-1\n"
			"M=D\n");
		else if( strcmp(op, "gt") == 0 ) fprintf(thisCodeWriter->asm_file,
			"// gt\n"
			"@SP\t\t// Load D with TOS - 1\n"
			"M=M-1\t\t// SP--\n"
			"A=M\t\t// A = SP / M = TOS\n"
			"D=-M\n"
			"A=A-1\t\t// Point A at TOS - 2\n"
			"D=D+M\n"
			"@GREATER_THAN"
			"D;JGT\n"
			"(NOT_GREATER_THAN)\n"
			"D=0\n"
			"@DONE\n"
			"0;JEQ\n"
			"(GREATER_THAN)\n"
			"D=-1\n"
			"@DONE\n"
			"0;JEQ\n"
			"(DONE)\n"
			"@SP\n"
			"A=M\n"
			"A=A-1\n"
			"M=D\n");
		else if( strcmp(op, "lt") == 0 ) fprintf(thisCodeWriter->asm_file,
			"// lt\n"
			"@SP\t\t// Load D with TOS - 1\n"
			"M=M-1\t\t// SP--\n"
			"A=M\t\t// A = SP / M = TOS\n"
			"D=-M\n"
			"A=A-1\t\t// Point A at TOS - 2\n"
			"D=D+M\n"
			"@LESS_THAN"
			"D;JLT\n"
			"(NOT_LESS_THAN)\n"
			"D=0\n"
			"@DONE\n"
			"0;JEQ\n"
			"(LESS_THAN)\n"
			"D=-1\n"
			"@DONE\n"
			"0;JEQ\n"
			"(DONE)\n"
			"@SP\n"
			"A=M\n"
			"A=A-1\n"
			"M=D\n");
		else if( strcmp(op, "and") == 0 ) fprintf(thisCodeWriter->asm_file,
			"// and\n"
			"@SP\t\t// Load D with TOS - 1\n"
			"M=M-1\t\t// SP--\n"
			"A=M\t\t// A = SP / M = TOS\n"
			"D=M\n"
			"A=A-1\t\t// Point A at TOS - 2\n"
			"M=D&M\n");
		else if( strcmp(op, "or") == 0 ) fprintf(thisCodeWriter->asm_file,
			"// or\n"
			"@SP\t\t// Load D with TOS - 1\n"
			"M=M-1\t\t// SP--\n"
			"A=M\t\t// A = SP / M = TOS\n"
			"D=M\n"
			"A=A-1\t\t// Point A at TOS - 2\n"
			"M=D|M\n");
		else if( strcmp(op, "not") == 0 ) fprintf(thisCodeWriter->asm_file,
			"// neg\n"
			"@SP\t\t// A = 0 / M = SP\n"
			"A=M\t\t// A = SP / M = TOS"
			"A=A-1\t\t// Point A at TOS - 1\n"
			"M=!M\n");
		else err = 3;
	}

	return err;
}

int codeWriter_writePushPop(CodeWriter thisCodeWriter, commandType command, char * memory, int offset)
{
	int err = 0;

	if( thisCodeWriter == NULL ) err = 1;

	if( err == 0 )
	{
		if( memory == NULL ) err == 2;
	}

	if( err == 0 )
	{
		if( strcmp(memory, "local") == 0 ) strcpy(memory, "LCL\0");
		else if( strcmp(memory, "argument") == 0 ) strcpy(memory, "ARG\0");
		else if( strcmp(memory, "this") == 0 ) strcpy(memory, "THIS\0");
		else if( strcmp(memory, "that") == 0 ) strcpy(memory, "THAT\0");
		else err = 3;

		if( command == C_PUSH ) fprintf(thisCodeWriter->asm_file,
			"// push %s %d\n"
			"@%d\t\t// Load D with value to push\n"
			"D=A\n"
			"@%s\n"
			"A=M\n"
			"A=D+A\n"
			"D=M\n"
			"@SP\t\t// A = 0 / M = SP\n"
			"A=M\t\t// A = SP / M = TOS\n"
			"M=D\t\t// TOS <-- D\n"
			"@SP\n"
			"M=M+1\t\t// SP++\n", memory, offset, offset, memory);
		else if( command == C_POP ) fprintf(thisCodeWriter->asm_file,
			"// pop %s %d\n"
			"@%d\t\t// Compute dest. memory address\n"
			"D=A\n"
			"@%s\n"
			"A=M\n"
			"D=D+A\n"
			"@SP\t\t// Store temporarily at TOS\n"
			"A=M\t\t// A = SP / M = TOS\n"
			"M=D\n"
			"A=A-1\t\t// A = SP - 1 / M = *(TOS - 1)\n"
			"D=M\t\t// Load D with TOS - 1\n"
			"A=A+1\t\t// A = SP / M = TOS\n"
			"A=M\t\t// Load A with memory + offset\n"
			"M=D\n"
			"@SP\n"
			"M=M-1\t\t// SP--\n", memory, offset, offset, memory);
		else err == 4;
	}

	return err;
}
