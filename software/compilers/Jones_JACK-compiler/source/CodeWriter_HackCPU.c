#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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

bool 		initialized = false;
FILE 		*asm_file;
const char 	*vm_filename;
char		current_function[MAX_FCN_NAME_LEN] = {0};

int codeWriter_init(FILE * new_asm_file)
{
	int err = 0;

	if( initialized ) err = 1;

	if( err == 0 )
	{
		if( new_asm_file == NULL ) err = 2;
	}

	if( err == 0 )
	{
		asm_file = new_asm_file;
		initialized = true;
	}
	
	return err;
}

int codeWriter_setFilename(const char * this_vm_filename)
{
	int err = 0;

	if( !initialized ) err = 1;

	if( err == 0 )
	{
		if( this_vm_filename == NULL ) err = 2;
	}

	if( err == 0 ) vm_filename = this_vm_filename;

	return err;
}

int codeWriter_writeArithmetic(char * op)
{
	int err = 0;

	if( !initialized ) err = 1;

	if( err == 0 )
	{
		if( op == NULL ) err == 2;
	}

	if( err == 0 )
	{
		if( strcmp(op, "add") == 0 ) fprintf(asm_file,
			"// add\n"
			"@SP\t\t// Load D with TOS - 1\n"
			"M=M-1\t\t// SP--\n"
			"A=M\t\t// A = SP / M = TOS\n"
			"D=M\n"
			"A=A-1\t\t// Point A at TOS - 2\n"
			"M=D+M\n");
		else if( strcmp(op, "sub") == 0 ) fprintf(asm_file,
			"// sub\n"
			"@SP\t\t// Load D with TOS - 1\n"
			"M=M-1\t\t// SP--\n"
			"A=M\t\t// A = SP / M = TOS\n"
			"D=M\n"
			"A=A-1\t\t// Point A at TOS - 2\n"
			"M=M-D\n");
		else if( strcmp(op, "neg") == 0 ) fprintf(asm_file,
			"// neg\n"
			"@SP\t\t// A = 0 / M = SP\n"
			"A=M\t\t// A = SP / M = TOS\n"
			"A=A-1\t\t// Point A at TOS - 1\n"
			"M=-M\n");
		else if( strcmp(op, "eq") == 0 )
		{
			fpos_t pos;
			fgetpos(asm_file, &pos);
			fprintf(asm_file,
			"// eq\n"
			"@SP\t\t// Load D with TOS - 1\n"
			"M=M-1\t\t// SP--\n"
			"A=M\t\t// A = SP / M = TOS\n"
			"D=M\n"
			"A=A-1\t\t// Point A at TOS - 2\n"
			"D=D-M\n"
			"@EQUAL_%ld\n"
			"D;JEQ\n"
			"(NOT_EQUAL_%ld)\n"
			"D=0\n"
			"@DONE_%ld\n"
			"0;JEQ\n"
			"(EQUAL_%ld)\n"
			"D=-1\n"
			"@DONE_%ld\n"
			"0;JEQ\n"
			"(DONE_%ld)\n"
			"@SP\n"
			"A=M\n"
			"A=A-1\n"
			"M=D\n", pos.__pos, pos.__pos, pos.__pos, pos.__pos, pos.__pos, pos.__pos);
		}
		else if( strcmp(op, "gt") == 0 )
		{
			fpos_t pos;
			fgetpos(asm_file, &pos);
			fprintf(asm_file,
			"// gt\n"
			"@SP\t\t// Load D with TOS - 1\n"
			"M=M-1\t\t// SP--\n"
			"A=M\t\t// A = SP / M = TOS\n"
			"D=-M\n"
			"A=A-1\t\t// Point A at TOS - 2\n"
			"D=D+M\n"
			"@GREATER_THAN_%ld\n"
			"D;JGT\n"
			"(NOT_GREATER_THAN_%ld)\n"
			"D=0\n"
			"@DONE_%ld\n"
			"0;JEQ\n"
			"(GREATER_THAN_%ld)\n"
			"D=-1\n"
			"@DONE_%ld\n"
			"0;JEQ\n"
			"(DONE_%ld)\n"
			"@SP\n"
			"A=M\n"
			"A=A-1\n"
			"M=D\n", pos.__pos, pos.__pos, pos.__pos, pos.__pos, pos.__pos, pos.__pos);
		}
		else if( strcmp(op, "lt") == 0 )
		{
			fpos_t pos;
			fgetpos(asm_file, &pos);
			fprintf(asm_file,
			"// lt\n"
			"@SP\t\t// Load D with TOS - 1\n"
			"M=M-1\t\t// SP--\n"
			"A=M\t\t// A = SP / M = TOS\n"
			"D=-M\n"
			"A=A-1\t\t// Point A at TOS - 2\n"
			"D=D+M\n"
			"@LESS_THAN_%ld\n"
			"D;JLT\n"
			"(NOT_LESS_THAN_%ld)\n"
			"D=0\n"
			"@DONE_%ld\n"
			"0;JEQ\n"
			"(LESS_THAN_%ld)\n"
			"D=-1\n"
			"@DONE_%ld\n"
			"0;JEQ\n"
			"(DONE_%ld)\n"
			"@SP\n"
			"A=M\n"
			"A=A-1\n"
			"M=D\n", pos.__pos, pos.__pos, pos.__pos, pos.__pos, pos.__pos, pos.__pos);
		}
		else if( strcmp(op, "and") == 0 ) fprintf(asm_file,
			"// and\n"
			"@SP\t\t// Load D with TOS - 1\n"
			"M=M-1\t\t// SP--\n"
			"A=M\t\t// A = SP / M = TOS\n"
			"D=M\n"
			"A=A-1\t\t// Point A at TOS - 2\n"
			"M=D&M\n");
		else if( strcmp(op, "or") == 0 ) fprintf(asm_file,
			"// or\n"
			"@SP\t\t// Load D with TOS - 1\n"
			"M=M-1\t\t// SP--\n"
			"A=M\t\t// A = SP / M = TOS\n"
			"D=M\n"
			"A=A-1\t\t// Point A at TOS - 2\n"
			"M=D|M\n");
		else if( strcmp(op, "not") == 0 ) fprintf(asm_file,
			"// neg\n"
			"@SP\t\t// A = 0 / M = SP\n"
			"A=M\t\t// A = SP / M = TOS\n"
			"A=A-1\t\t// Point A at TOS - 1\n"
			"M=!M\n");
		else err = 3;
	}

	return err;
}

int codeWriter_writePushPop(commandType command, char * segment, int value)
{
	int err = 0;
	enum { LCL_ARG_THS_THT, TEMP, PTR, CONSTANT, STATIC } segment_type_t;
	
	if( !initialized ) err = 1;

	if( err == 0 )
	{
		if( segment == NULL ) err == 3;
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
		else if( strcmp(segment, "static") == 0 )
		{
			segment_type_t = STATIC;
		}
		else
		{
			err = 4;
		}
	}

	if( err == 0 )
	{
		if( command == C_PUSH )
		{
			fprintf(asm_file, "// push %s %d\n", segment, value);
			
			switch( segment_type_t )
			{
				case LCL_ARG_THS_THT:
					fprintf(asm_file,
						"@%s\t\t// Load D with value to push\n"
						"D=M\n"
						"@%d\n"
						"A=D+A\n"
						"D=M\n", segment, value);
					break;
				case TEMP:
					fprintf(asm_file,
						"@5\t\t// Load D with value to push\n"
						"D=A\n"
						"@%d\n"
						"A=D+A\n"
						"D=M\n", value);
					break;
				case PTR:
					fprintf(asm_file,
						"@3\t\t// Load D with value to push\n"
						"D=A\n"
						"@%d\n"
						"A=D+A\n"
						"D=M\n", value);
					break;
				case CONSTANT:
					fprintf(asm_file,
						"@%d\t\t// Load D with value to push\n"
						"D=A\n", value);
					break;
				case STATIC:
					fprintf(asm_file,
						"@%s.%d\t\t// Load D with value to push\n"
						"D=M\n", vm_filename, value);
					break;
			}

			fprintf(asm_file,
				"@SP\t\t// A = 0 / M = SP\n"
				"M=M+1\t\t// SP++\n"
				"A=M-1\t\t// A = old SP / M = TOS - 1\n"
				"M=D\t\t// (TOS - 1) <-- D\n");
		}

		else if( command == C_POP )
		{
			if( segment_type_t == CONSTANT ) err = 5;
			else
			{
				fprintf(asm_file, "// pop %s %d\n", segment, value);

				switch( segment_type_t )
				{
					case LCL_ARG_THS_THT:
						fprintf(asm_file,
							"@%s\t\t// Compute dest. memory address\n"
							"D=M\n"
							"@%d\n"
							"D=D+A\n", segment, value);
						break;
					case TEMP:
						fprintf(asm_file,
							"@5\t\t// Compute dest. memory address\n"
							"D=A\n"
							"@%d\n"
							"D=D+A\n", value);
						break;
					case PTR:
						fprintf(asm_file,
							"@3\t\t// Compute dest. memory address\n"
							"D=A\n"
							"@%d\n"
							"D=D+A\n", value);
						break;
					case STATIC:
						fprintf(asm_file,
							"@%s.%d\t\t// Compute dest. memory address\n"
							"D=A\n", vm_filename, value);
						break;
				}

				fprintf(asm_file,
					"@R13\n"
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
		else err == 6;
	}

	return err;
}

int codeWriter_writeLabel(char * label)
{
	int err = 0;

	if( !initialized ) err = 1;

	if( err == 0 )
	{
		if( label == NULL ) err = 2;
	}

	/*
	if( err == 0 )
	{
		if( strlen(current_function) == 0 ) err = 3;
	}
	*/

	if( err == 0 )
	{
		fprintf(asm_file, "(%s.%s$%s)\n", vm_filename, current_function, label);
	}

	return err;
}

int codeWriter_writeGoto(char * label)
{
	int err = 0;

	if( !initialized ) err = 1;

	if( err == 0 )
	{
		if( label == NULL ) err = 2;
	}

	/*
	if( err == 0 )
	{
		if( strlen(current_function) == 0 ) err = 3;
	}
	*/

	if( err == 0 )
	{
		fprintf(asm_file,
			"// goto %s\n"
			"@%s.%s$%s\n"
			"0;JEQ\n", label, vm_filename, current_function, label);
	}

	return err;
}

int codeWriter_writeIf(char * label)
{
	int err = 0;

	if( !initialized ) err = 1;

	if( err == 0 )
	{
		if( label == NULL ) err = 2;
	}

	/*
	if( err == 0 )
	{
		if( strlen(current_function) == 0 ) err = 3;
	}
	*/

	if( err == 0 )
	{
		fprintf(asm_file,
			"// if-goto %s\n"
			"@SP\n"
			"M=M-1\t\t// SP--\n"
			"A=M\n"
			"D=M\n"
			"@%s.%s$%s\n"
			"D;JNE\n", label, vm_filename, current_function, label);
	}

	return err;
}

int codeWriter_writeFunction(char * name, int argc)
{
	int err = 0;

	if( !initialized ) err = 1;

	if( err == 0 )
	{
		if( name == NULL ) err = 2;
	}

	/*
	if( err == 0 )
	{
		if( strlen(current_function) == 0 ) err = 3;
	}
	*/

	if( err == 0 )
	{
		strcpy(current_function, name);

		fprintf(asm_file,
			"// function %s %d\n"
			"(%s.%s)\n"
			"@SP\n"
			"A=M\n", current_function, argc, vm_filename, current_function);
		for(int i = 0; i < argc; i++)
		{
			fprintf(asm_file,
				"M=0\t\t// push 0\n"
				"A=A+1\n");
		}
		fprintf(asm_file,
			"D=A\n"
			"@SP\n"
			"M=D\n");
	}

	return err;
}

int codeWriter_writeReturn(void)
{

	int err = 0;

	if( !initialized ) err = 1;

	/*
	if( err == 0 )
	{
		if( strlen(current_function) == 0 ) err = 3;
	}
	*/

	/*
	* Executes a return from a function using the following pseudocode:
	*   frame = LCL
	*   retAddr = *(frame-5)
	*   *ARG = pop()
	*   SP = ARG+1
	*   THAT = *(frame-1)
	*   THIS = *(frame-2)
	*   ARG = *(frame-3)
	*   LCL = *(frame-4)
	*   goto retAddr
	*/

	if( err == 0 )
	{
		memset(current_function, 0, strlen(current_function));

		fprintf(asm_file,
			"// return\n"
			"@LCL\t\t// frame = LCL\n"
			"D=M\n"
			"@R13\n"
			"M=D\t\t// frame in R13\n"
			"@5\t\t// retAddr = *(frame-5)\n"
			"A=D-A\n"
			"D=M\n"
			"@14\n"
			"M=D\t\t// retAddr in R14\n"
			"@SP\t\t// *ARG = pop()\n"
			"A=M\n"
			"A=A-1\n"
			"D=M\n"
			"@ARG\n"
			"A=M\n"
			"M=D\n"
			"@ARG\t\t// SP = ARG+1\n"
			"D=M+1\n"
			"@SP\n"
			"M=D\n"
			"@R13\t\t// THAT = *(frame-1)\n"
			"M=M-1\t\t// frame--\n"
			"A=M\n"
			"D=M\n"
			"@THAT\n"
			"M=D\n"
			"@R13\t\t// THIS = *(frame-2)\n"
			"M=M-1\t\t// frame--\n"
			"A=M\n"
			"D=M\n"
			"@THIS\n"
			"M=D\n"
			"@R13\t\t// ARG = *(frame-3)\n"
			"M=M-1\t\t// frame--\n"
			"A=M\n"
			"D=M\n"
			"@ARG\n"
			"M=D\n"
			"@R13\t\t// LCL = *(frame-4)\n"
			"M=M-1\t\t// frame--\n"
			"A=M\n"
			"D=M\n"
			"@LCL\n"
			"M=D\n"
			"@R14\t\t// goto retAddr\n"
			"A=M\n"
			"0;JEQ\n");
	}

	return err;
}