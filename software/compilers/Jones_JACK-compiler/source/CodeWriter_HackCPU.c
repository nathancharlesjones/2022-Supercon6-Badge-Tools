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
			"@EQUAL\n"
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
			"@GREATER_THAN\n"
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
			"@LESS_THAN\n"
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

int codeWriter_writePushPop(CodeWriter thisCodeWriter, commandType command, char * segment, int value)
{
	int err = 0;
	enum { LCL_ARG_THS_THT, TEMP, PTR, CONSTANT } segment_type_t;
	
	if( thisCodeWriter == NULL ) err = 1;

	if( err == 0 )
	{
		if( segment == NULL ) err == 2;
	}

	if( err == 0 )
	{
		if(	( strcmp(segment, "local") == 0 ) ||
			( strcmp(segment, "argument") == 0 ) ||
			( strcmp(segment, "this") == 0 ) ||
			( strcmp(segment, "that") == 0 ) )
		{
			segment_type_t = LCL_ARG_THS_THT;
			if( strcmp(segment, "local") == 0 )
			{
				strcpy(segment, "LCL\0");
			}
			else if( strcmp(segment, "argument") == 0 )
			{
				strcpy(segment, "ARG\0");
			}
			else if( strcmp(segment, "this") == 0 )
			{
				strcpy(segment, "THIS\0");
			}
			else if( strcmp(segment, "that") == 0 )
			{
				strcpy(segment, "THAT\0");
			}
		}
		else if( strcmp(segment, "temp") == 0 )
		{
			segment_type_t = TEMP;
		}
		else if( strcmp(segment, "pointer") == 0 )
		{
			segment_type_t = PTR;
		}
		else if( strcmp(segment, "constant") == 0 )
		{
			segment_type_t = CONSTANT;
		}
		else
		{
			err = 3;
		}
	}

	if( err == 0 )
	{
		if( command == C_PUSH )
		{
			fprintf(thisCodeWriter->asm_file, "// push %s %d\n", segment, value);
			
			switch( segment_type_t )
			{
				case LCL_ARG_THS_THT:
					fprintf(thisCodeWriter->asm_file,
						"@%s\t\t// Load D with value to push\n"
						"D=M\n"
						"@%d\n"
						"A=D+A\n"
						"D=M\n", segment, value);
					break;
				case TEMP:
					fprintf(thisCodeWriter->asm_file,
						"@5\t\t// Load D with value to push\n"
						"D=A\n"
						"@%d\n"
						"A=D+A\n"
						"D=M\n", value);
					break;
				case PTR:
					fprintf(thisCodeWriter->asm_file,
						"@3\t\t// Load D with value to push\n"
						"D=A\n"
						"@%d\n"
						"A=D+A\n"
						"D=M\n", value);
					break;
				case CONSTANT:
					fprintf(thisCodeWriter->asm_file,
						"@%d\t\t// Load D with value to push\n"
						"D=A\n", value);
					break;
			}

			fprintf(thisCodeWriter->asm_file,
				"@SP\t\t// A = 0 / M = SP\n"
				"M=M+1\t\t// SP++\n"
				"A=M-1\t\t// A = old SP / M = TOS - 1\n"
				"M=D\t\t// (TOS - 1) <-- D\n");
		}

		else if( command == C_POP )
		{
			if( segment_type_t == CONSTANT ) err = 4;
			else
			{
				fprintf(thisCodeWriter->asm_file, "// pop %s %d\n", segment, value);

				switch( segment_type_t )
				{
					case LCL_ARG_THS_THT:
						fprintf(thisCodeWriter->asm_file,
							"@%s\t\t// Compute dest. memory address\n"
							"D=M\n"
							"@%d\n"
							"D=D+A\n", segment, value);
						break;
					case TEMP:
						fprintf(thisCodeWriter->asm_file,
							"@5\t\t// Compute dest. memory address\n"
							"D=A\n"
							"@%d\n"
							"D=D+A\n", value);
						break;
					case PTR:
						fprintf(thisCodeWriter->asm_file,
							"@3\t\t// Compute dest. memory address\n"
							"D=A\n"
							"@%d\n"
							"D=D+A\n", value);
						break;
				}

				fprintf(thisCodeWriter->asm_file,
					"@13\n"
					"M=D\t\t// Store dest. addr. in RAM[13] (temp reg)\n"
					"@SP\t\t// Load D with value at TOS\n"
					"M=M-1\t\t// SP--\n"
					"A=M\t\t// A = SP - 1 / M = *(TOS - 1)\n"
					"D=M\t\t// Load D with TOS - 1\n"
					"@13\n"
					"A=M\t\t// Load A with memory + offset\n"
					"M=D\n");
			}
		}
		else err == 5;
	}

	return err;
}
