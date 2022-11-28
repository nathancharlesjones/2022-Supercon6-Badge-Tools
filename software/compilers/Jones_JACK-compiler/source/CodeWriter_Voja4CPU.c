#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "CodeWriter.h"
#include "Parser.h"

bool 		initialized = false;
FILE 		*asm_file;
const char 	*vm_filename;
char		current_function[MAX_FCN_NAME_LEN] = {0};
int  		return_count = 0;

int codeWriter_init(char * new_asm_file)
{
	int err = 0;

	if( initialized ) err = 1;

	if( err == 0 )
	{
		if( new_asm_file == NULL ) err = 2;
	}

	if( err == 0 )
	{
		asm_file = fopen(new_asm_file, "w");
		if( asm_file )
		{
			initialized = true;
			// Write bootstrap code:
			//    SP = xDF
			//    call Sys.init
			fprintf(asm_file,
				"// Bootstrap code\n"
				"MOV R9, 0xF\t\t// SP = 0xDF\n"
				"MOV R8, 0xD\n"
				"jr Sys.init\n");
		}
		else err = 3;
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
			"ADD R9, 4\t\t// SP++\n"
			"ADC R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"MOV R3, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"MOV R2, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"MOV R1, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"ADD R9, 3\n"
			"ADC R8, 0\n"
			"ADD R7, R3\t\t// *TOS = *TOS + *(TOS+1)\n"
			"ADC R6, R2\n"
			"ADC R5, R1\n"
			"ADC R4, R0\n");
		else if( strcmp(op, "sub") == 0 ) fprintf(asm_file,
			"// sub\n"
			"ADD R9, 4\t\t// SP++\n"
			"ADC R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"MOV R3, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"MOV R2, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"MOV R1, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"ADD R9, 3\n"
			"ADC R8, 0\n"
			"SUB R3, R7\t\t// *(TOS+1) = *(TOS+1) - *TOS\n"
			"SBB R2, R6\n"
			"SBB R1, R5\n"
			"SBB R0, R4\n"
			"MOV R7, R3\t\t// *(TOS) <-- *(TOS+1)\n"
			"MOV R6, R2\n"
			"MOV R5, R1\n"
			"MOV R4, R0\n");
		else if( strcmp(op, "neg") == 0 ) fprintf(asm_file,
			"// neg\n"
			"XOR R7, 1\n"
			"XOR R6, 1\n"
			"XOR R5, 1\n"
			"XOR R4, 1\n"
			"ADD R7, 1\n"
			"ADC R6, 0\n"
			"ADC R5, 0\n"
			"ADC R4, 0\n");
		else if( strcmp(op, "eq") == 0 )
		{
			fpos_t pos;
			fgetpos(asm_file, &pos);
			fprintf(asm_file,
			"// eq\n"
			"ADD R9, 4\t\t// SP++\n"
			"ADC R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"MOV R3, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"MOV R2, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"MOV R1, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"ADD R9, 3\n"
			"ADC R8, 0\n"
			"SUB R3, R7\n"
			"SKIP Z, 1\n"
			"jr not_equal_%ld\n"
			"SBB R2, R6\n"
			"SKIP Z, 1\n"
			"jr not_equal_%ld\n"
			"SBB R1, R5\n"
			"SKIP Z, 1\n"
			"jr not_equal_%ld\n"
			"SBB R0, R4\n"
			"SKIP Z, 1\n"
			"jr not_equal_%ld\n"
			"MOV R7, 0xF\n"
			"MOV R6, 0xF\n"
			"MOV R5, 0xF\n"
			"MOV R4, 0xF\n"
			"jr done_%ld\n"
			"not_equal_%ld:\n"
			"MOV R7, 0x0\n"
			"MOV R6, 0x0\n"
			"MOV R5, 0x0\n"
			"MOV R4, 0x0\n"
			"done_%ld:\n", pos.__pos, pos.__pos, pos.__pos, pos.__pos, pos.__pos, pos.__pos);
		}
		else if( strcmp(op, "gt") == 0 )
		{
			fpos_t pos;
			fgetpos(asm_file, &pos);
			fprintf(asm_file,
			"// gt\n"
			"ADD R9, 4\t\t// SP++\n"
			"ADC R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"MOV R3, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"MOV R2, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"MOV R1, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"ADD R9, 3\n"
			"ADC R8, 0\n"
			"SUB R7, R3\n"
			"SBB R6, R2\n"
			"SBB R5, R1\n"
			"SBB R4, R0\n"
			"MOV R0, R4\n"
			"BIT R0, 3\n"
			"SKIP Z, 1\n"
			"jr not_greater_than_%ld\n"
			"MOV R7, 0xF\n"
			"MOV R6, 0xF\n"
			"MOV R5, 0xF\n"
			"MOV R4, 0xF\n"
			"jr done_%ld\n"
			"not_greater_than_%ld:\n"
			"MOV R7, 0x0\n"
			"MOV R6, 0x0\n"
			"MOV R5, 0x0\n"
			"MOV R4, 0x0\n"
			"done_%ld:\n", pos.__pos, pos.__pos, pos.__pos, pos.__pos);
		}
		else if( strcmp(op, "lt") == 0 )
		{
			fpos_t pos;
			fgetpos(asm_file, &pos);
			fprintf(asm_file,
			"// lt\n"
			"ADD R9, 4\t\t// SP++\n"
			"ADC R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"MOV R3, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"MOV R2, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"MOV R1, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"ADD R9, 3\n"
			"ADC R8, 0\n"
			"SUB R3, R7\n"
			"SBB R2, R6\n"
			"SBB R1, R5\n"
			"SBB R0, R4\n"
			"BIT R0, 3\n"
			"SKIP Z, 1\n"
			"jr not_less_than_%ld\n"
			"MOV R7, 0xF\n"
			"MOV R6, 0xF\n"
			"MOV R5, 0xF\n"
			"MOV R4, 0xF\n"
			"jr done_%ld\n"
			"not_less_than_%ld:\n"
			"MOV R7, 0x0\n"
			"MOV R6, 0x0\n"
			"MOV R5, 0x0\n"
			"MOV R4, 0x0\n"
			"done_%ld:\n", pos.__pos, pos.__pos, pos.__pos, pos.__pos);
		}
		else if( strcmp(op, "and") == 0 ) fprintf(asm_file,
			"// and\n"
			"ADD R9, 4\t\t// SP++\n"
			"ADC R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"MOV R3, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"MOV R2, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"MOV R1, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"ADD R9, 3\n"
			"ADC R8, 0\n"
			"AND R7, R3\t\t// *TOS = *TOS & *(TOS+1)\n"
			"AND R6, R2\n"
			"AND R5, R1\n"
			"AND R4, R0\n");
		else if( strcmp(op, "or") == 0 ) fprintf(asm_file,
			"// or\n"
			"ADD R9, 4\t\t// SP++\n"
			"ADC R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"MOV R3, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"MOV R2, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"MOV R1, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R0-3 with *(TOS + 1)\n"
			"ADD R9, 3\n"
			"ADC R8, 0\n"
			"OR R7, R3\t\t// *TOS = *TOS | *(TOS+1)\n"
			"OR R6, R2\n"
			"OR R5, R1\n"
			"OR R4, R0\n");
		else if( strcmp(op, "not") == 0 ) fprintf(asm_file,
			"// neg\n"
			"XOR R7, 1\n"
			"XOR R6, 1\n"
			"XOR R5, 1\n"
			"XOR R4, 1\n");
		else err = 3;
	}

	return err;
}

int codeWriter_writePushPop(commandType command, char * segment, int value)
{
	int err = 0;
	enum { LOCAL, ARGUMENT, THIS, THAT, TEMP, PTR, CONSTANT, STATIC } segment_type_t;
	
	if( !initialized ) err = 1;

	if( err == 0 )
	{
		if( segment == NULL ) err == 3;
	}

/*
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
	*/

	if( err == 0 )
	{
		if( command == C_PUSH )
		{
			fprintf(asm_file, "// push %s %d\n", segment, value);
			fprintf(asm_file,
				"MOV R0, R7\t\t// Push down TOS\n"
				"MOV R0, [R8:R9]\n"
				"INC R9\n"
				"ADC R8, 0\n"
				"MOV R0, R6\n"
				"MOV R0, [R8:R9]\n"
				"INC R9\n"
				"ADC R8, 0\n"
				"MOV R0, R5\n"
				"MOV R0, [R8:R9]\n"
				"INC R9\n"
				"ADC R8, 0\n"
				"MOV R0, R4\n"
				"MOV R0, [R8:R9]\n"
				"INC R9\n"
				"ADC R8, 0\n");

			if( strcmp(segment, "constant") == 0 )
			{
				fprintf(asm_file, "MOV R7, %d\n", value & 0xF);
				fprintf(asm_file, "MOV R6, %d\n", (value & 0xF0)>>4);
				fprintf(asm_file, "MOV R5, %d\n", (value & 0xF00)>>8);
				fprintf(asm_file, "MOV R4, %d\n", (value & 0xF000)>>12);
			}
			else if (	( strcmp(segment, "local") == 0 ) ||
						( strcmp(segment, "argument") == 0 ) ||
						( strcmp(segment, "this") == 0 ) ||
						( strcmp(segment, "that") == 0 ) ||
						( strcmp(segment, "pointer") == 0 ) ||
						( strcmp(segment, "temp") == 0 ) ||
						( strcmp(segment, "static") == 0 ) )
			{
				if (	( strcmp(segment, "local") == 0 ) ||
						( strcmp(segment, "argument") == 0 ) ||
						( strcmp(segment, "this") == 0 ) ||
						( strcmp(segment, "that") == 0 ) )
				{
					if( strcmp(segment, "local") == 0 )
					{
						fprintf(asm_file,
							"MOV R2, 0x1\n"
							"MOV R1, 0x1\n");
					}
					else if( strcmp(segment, "argument") == 0 )
					{
						fprintf(asm_file,
							"MOV R2, 0x3\n"
							"MOV R1, 0x1\n");
					}
					else if( strcmp(segment, "this") == 0 )
					{
						fprintf(asm_file,
							"MOV R2, 0x5\n"
							"MOV R1, 0x1\n");
					}
					else if( strcmp(segment, "that") == 0 )
					{
						fprintf(asm_file,
							"MOV R2, 0x7\n"
							"MOV R1, 0x1\n");
					}
					fprintf(asm_file,
						"MOV R4, %d\n"
						"MOV R3, %d\n"
						"ADD R2, R4\n"
						"ADC R1, R3\n", (value & 0xF), ((value & 0xF0)>>4));
				}
				else if( strcmp(segment, "pointer") == 0 )
				{
					/*
					if( value == 1 )
					{
						fprintf(asm_file,
							"MOV R2, 0x1\n"
							"MOV R1, 0x1\n");
					}
					*/
				}
				else if( strcmp(segment, "temp") == 0 )
				{
					fprintf(asm_file,
						"MOV R2, %d\t\t// Load value\n"
						"ADD R2, R2\t\t// value = 2 * value\n"
						"ADC R1, 0\n"
						"ADD R2, R2\t\t// value = 4 * value\n"
						"ADC R1, 0\n"
						"ADD R2, 0xB\t\t// value = value + 0x1B (base addr of TEMP section)\n"
						"ADC R1, 0x1\n", (value & 0xF));
				}
				else if( strcmp(segment, "static") == 0 )
				{
					// ???
					fprintf(asm_file,
						"MOV R2, %d\t\t// Load value\n"
						"ADD R2, R2\t\t// value = 2 * value\n"
						"ADC R1, 0\n"
						"ADD R2, R2\t\t// value = 4 * value\n"
						"ADC R1, 0\n"
						"ADD R2, 0xB\t\t// value = value + 0x3B (base addr of TEMP section)\n"
						"ADC R1, 0x3\n", (value & 0xF));
				}

				fprintf(asm_file,
					"MOV R0, [R1:R2]\n"
					"MOV R7, R0\n"
					"INC R2\n"
					"ADC R1, 0\n"
					"MOV R0, [R1:R2]\n"
					"MOV R6, R0\n"
					"INC R2\n"
					"ADC R1, 0\n"
					"MOV R0, [R1:R2]\n"
					"MOV R5, R0\n"
					"INC R2\n"
					"ADC R1, 0\n"
					"MOV R0, [R1:R2]\n"
					"MOV R4, R0\n");
			}
			else
			{
				err = 4;
			}
		else
		{
			//Pop
		}
	}

	if( err == 0 )
	{
		if( command == C_PUSH )
		{
			fprintf(asm_file, "// push %s %d\n", segment, value);
			fprintf(asm_file,
				"MOV R0, R7\t\t// Push down TOS\n"
				"MOV R0, [R8:R9]\n"
				"INC R9\n"
				"ADC R8, 0\n"
				"MOV R0, R6\n"
				"MOV R0, [R8:R9]\n"
				"INC R9\n"
				"ADC R8, 0\n"
				"MOV R0, R5\n"
				"MOV R0, [R8:R9]\n"
				"INC R9\n"
				"ADC R8, 0\n"
				"MOV R0, R4\n"
				"MOV R0, [R8:R9]\n"
				"INC R9\n"
				"ADC R8, 0\n");
			
			switch( segment_type_t )
			{
				case LOCAL:
					fprintf(asm_file,
						"MOV R2, 0x1\n"
						"MOV R1, 0x1\n"
						"MOV R4, %d\n"
						"MOV R3, %d\n"
						"ADD R2, R4\n"
						"ADC R1, R3\n"
						"MOV R0, [R1:R2]\n"
						"MOV R7, R0\n"
						"INC R2\n"
						"ADC R1, 0\n"
						"MOV R0, [R1:R2]\n"
						"MOV R6, R0\n"
						"INC R2\n"
						"ADC R1, 0\n"
						"MOV R0, [R1:R2]\n"
						"MOV R5, R0\n"
						"INC R2\n"
						"ADC R1, 0\n"
						"MOV R0, [R1:R2]\n"
						"MOV R4, R0\n", (value & 0xF), ((value & 0xF0)>>4));
					break;
				case ARGUMENT:
				case THIS:
				case THAT:
				case TEMP:
					fprintf(asm_file,
						"MOV R2, %d\t\t// Load value\n"
						"ADD R2, R2\t\t// value = 2 * value\n"
						"ADC R1, 0\n"
						"ADD R2, R2\t\t// value = 4 * value\n"
						"ADC R1, 0\n"
						"ADD R2, 0xB\t\t// value = value + 0x1B (base addr of TEMP section)\n"
						"ADC R1, 0x1\n", (value & 0xF));
					// load from mem
					break;
				case PTR:
					fprintf(asm_file,
						"MOV R2, 0x\n"
						"D=A\n"
						"@%d\n"
						"A=D+A\n"
						"D=M\n", value);
					break;
				case CONSTANT:
					fprintf(asm_file, "MOV R7, %d\n", value & 0xF);
					fprintf(asm_file, "MOV R6, %d\n", (value & 0xF0)>>4);
					fprintf(asm_file, "MOV R5, %d\n", (value & 0xF00)>>8);
					fprintf(asm_file, "MOV R4, %d\n", (value & 0xF000)>>12);
					break;
				case STATIC:
					fprintf(asm_file,
						"@%s.%d\t\t// Load D with value to push\n"
						"D=M\n", vm_filename, value);
					break;
			}
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
		fprintf(asm_file, "%s.%s$%s:\n", vm_filename, current_function, label);
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
			"jr %s.%s$%s\n", label, vm_filename, current_function, label);
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
			"AND R1, 0\n"
			"OR R1, R7\n"
			"OR R1, R6\n"
			"OR R1, R5\n"
			"OR R1, R4\n"
			"ADD R9, 4\t\t// SP++\n"
			"ADC R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R4-7 with *(TOS + 1)\n"
			"MOV R7, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R4-7 with *(TOS + 1)\n"
			"MOV R6, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R4-7 with *(TOS + 1)\n"
			"MOV R5, R0\n"
			"DEC R9\n"
			"SBB R8, 0\n"
			"MOV R0, [R8:R9]\t\t// Load R4-7 with *(TOS + 1)\n"
			"MOV R4, R0\n"
			"ADD R9, 3\n"
			"ADC R8, 0\n"
			"ADD R1, 0\n"
			"SKIP Z, 1\n"
			"jr %s.%s$%s\n", label, vm_filename, current_function, label);
	}

	return err;
}

int codeWriter_writeFunction(char * name, int nVars)
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
		memset(current_function, 0, strlen(current_function));
		strcpy(current_function, name);

		/*
		fprintf(asm_file,
			"// function %s %d\n"
			"(%s.%s)\n"
			"@SP\n"
			"A=M\n", current_function, nVars, vm_filename, current_function);
		*/
		fprintf(asm_file,
			"// function %s %d\n"
			"%s:\n", current_function, nVars, current_function);
		for(int i = 0; i < nVars; i++)
		{
			fprintf(asm_file,
				"MOV R0, R7\t\t// push 0\n"
				"MOV [R8:R9], R0\n"
				"INC R9\n"
				"ADC R8, 0\n"
				"MOV R0, R6\n"
				"MOV [R8:R9], R0\n"
				"INC R9\n"
				"ADC R8, 0\n"
				"MOV R0, R5\n"
				"MOV [R8:R9], R0\n"
				"INC R9\n"
				"ADC R8, 0\n"
				"MOV R0, R4\n"
				"MOV [R8:R9], R0\n"
				"INC R9\n"
				"ADC R8, 0\n"
				"AND R7, 0\n"
				"AND R6, 0\n"
				"AND R5, 0\n"
				"AND R4, 0\n");
		}
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

int codeWriter_writeCall(char * name, int nArgs)
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
		fprintf(asm_file, "// call %s %d\n", name, nArgs);
		fprintf(asm_file, "@%s$ret.%d\t\t// push retAddr\n", name, return_count);
		fprintf(asm_file, 
			"D=A\n"
			"@SP\n"
			"A=M\n"
			"M=D\n"
			"@SP\n"
			"M=M+1\n"
			"@LCL\t\t// push LCL\n"
			"D=M\n"
			"@SP\n"
			"A=M\n"
			"M=D\n"
			"@SP\n"
			"M=M+1\n"
			"@ARG\t\t// push ARG\n"
			"D=M\n"
			"@SP\n"
			"A=M\n"
			"M=D\n"
			"@SP\n"
			"M=M+1\n"
			"@THIS\t\t// push THIS\n"
			"D=M\n"
			"@SP\n"
			"A=M\n"
			"M=D\n"
			"@SP\n"
			"M=M+1\n"
			"@THAT\t\t// push THAT\n"
			"D=M\n"
			"@SP\n"
			"A=M\n"
			"M=D\n"
			"@SP\n"
			"M=M+1\n"
			"@SP\t\t// ARG = SP - 5 - nArgs\n"
			"D=M\n"
			"@5\n"
			"D=D-A\n"
			"@%d\n"
			"D=D-A\n"
			"@ARG\n"
			"M=D\n"
			"@SP\t\t// LCL = SP\n"
			"D=M\n"
			"@LCL\n"
			"M=D\n"
			"@%s\t\t// goto name\n"
			"0;JEQ\n"
			"(%s$ret.%d)\t\t// (retAddr)\n", nArgs, name, name, return_count);
		return_count++;
	}

	return err;
}

int codeWriter_close(void)
{
	int err = 0;

	if( !initialized ) err = 1;

	if( err == 0 )
	{
		fclose(asm_file);
	}

	return err;
}