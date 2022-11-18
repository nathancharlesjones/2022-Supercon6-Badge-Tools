#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CodeWriter.h"
#include "Parser.h"

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
		if( newCodeWriter == NULL ) err = 3;
	}

	if( err == 0 )
	{
		*newCodeWriter = calloc(1, sizeof(CodeWriter_t));
		if( *newCodeWriter == NULL ) err = 2;
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
		if( strcmp(op, "add") == 0 ) fprintf(thisCodeWriter->asm_file, "ADD");
		else if( strcmp(op, "sub") == 0 ) fprintf(thisCodeWriter->asm_file, "ADD");
		else if( strcmp(op, "neg") == 0 ) fprintf(thisCodeWriter->asm_file, "ADD");
		else if( strcmp(op, "eq") == 0 ) fprintf(thisCodeWriter->asm_file, "ADD");
		else if( strcmp(op, "gt") == 0 ) fprintf(thisCodeWriter->asm_file, "ADD");
		else if( strcmp(op, "lt") == 0 ) fprintf(thisCodeWriter->asm_file, "ADD");
		else if( strcmp(op, "and") == 0 ) fprintf(thisCodeWriter->asm_file, "ADD");
		else if( strcmp(op, "or") == 0 ) fprintf(thisCodeWriter->asm_file, "ADD");
		else if( strcmp(op, "not") == 0 ) fprintf(thisCodeWriter->asm_file, "ADD");
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

		if( command == C_PUSH ) fprintf(thisCodeWriter->asm_file, "; Push %s %d\n@%d\nD=A\n@%s\nA=M\nA=D+A\nA=M\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n", memory, offset, offset, memory);
		else if( command == C_POP ) fprintf(thisCodeWriter->asm_file, "POP %s %d\n", memory, offset);
		else err == 4;
	}

	return err;
}
