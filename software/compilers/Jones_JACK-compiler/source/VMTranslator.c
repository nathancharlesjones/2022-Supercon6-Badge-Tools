#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "Parser.h"
#include "CodeWriter.h"

#define MAX_STRING_LENGTH 128
#define MAX_VM_FILENAME 64

#ifdef DEBUG
#	define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#	define DEBUG_PRINTF(...)
#endif

int main( int argc, char ** argv )
{
	int err = 0;
	FILE *vm_file, *asm_file;
	char vm_filename[MAX_VM_FILENAME] = {0};
	bool asm_file_created = false;

	// Error checking:
	//    - No arguments or more than one argument given
	//    - Filename doesn't end in .vm or
	//    - No .vm files in directory
	if( argc != 2 )
	{
		err = -1;
		fprintf(stderr, "Invalid usage of VMTranslator. Correct usage is './VMTranslator ARG', where ARG is the path & name for a .vm file or a folder containing one or more .vm files.\n");
	}
	

	if( err == 0 )
	{
		DEBUG_PRINTF("strlen .vm filename: %ld\n", strlen(argv[1]));
		// Add 2; one for nul (not included in strlen) and one to go from ".vm" to ".asm"
		char * asm_filename = calloc(1, strlen(argv[1])+2);
		if( asm_filename == NULL ) err = -1;
		else
		{
			// Copy all but the "vm" at the end of the file
			strncpy(asm_filename, argv[1], strlen(argv[1])-2);
			DEBUG_PRINTF("ASM filename: %s\tstrlen: %ld\n", asm_filename, strlen(asm_filename));
			strcat(asm_filename, "asm");
			DEBUG_PRINTF("ASM filename: %s\tstrlen: %ld\n", asm_filename, strlen(asm_filename));
			asm_file = fopen(asm_filename, "w");
			if( asm_file == NULL )
			{
				err = -1;
				fprintf(stderr, "Could not open .asm file.\n");
			}
			else
			{
				asm_file_created = true;
				DEBUG_PRINTF("Created asm file\n");
			}
		}
	}

	if( err == 0 )
	{
		err = codeWriter_init(asm_file);
		DEBUG_PRINTF("Created new codeWriter. err = %d\n", err);
	}

	if( err == 0 )
	{
		DEBUG_PRINTF("Before fopen\n");
		vm_file = fopen(argv[1], "r");
		DEBUG_PRINTF("After fopen\n");
		if( vm_file == NULL )
		{
			DEBUG_PRINTF("vm_file is NULL\n");
			err = -1;
			fprintf(stderr, "Could not open .vm file.\n");
		}
		else
		{
			char * slash = rindex(argv[1], '\\');
			if( slash == NULL )
			{
				slash = rindex(argv[1], '/');
				if( slash == NULL )
				{
					err = -1;
					fprintf(stderr, "Invalid filepath for .vm file??\n");
				}
			}
			if( slash != NULL )
			{
				strncpy(vm_filename, slash+1, strlen(argv[1]) - (slash - argv[1]) - 4);
				DEBUG_PRINTF("vm_filename: %s\n", vm_filename);
			}
		}
	}

	if( err == 0 )
	{
		DEBUG_PRINTF("Processing vm file\n");
		// Iterate through .vm file
		codeWriter_setFilename(vm_filename);
		while( true )
		{
			char buffer[MAX_STRING_LENGTH];
	        char * done = fgets(buffer, MAX_STRING_LENGTH, vm_file);
	        if( done != NULL )
        	{
        		DEBUG_PRINTF("vm file has more lines\n");
        		Parser_t parser_data = {0};
        		parse(buffer, &parser_data);
        		DEBUG_PRINTF("Current command: %d\t%s\t%d\n", parser_data.command, parser_data.arg1, parser_data.arg2);
        		switch( parser_data.command )
				{
					case C_ARITHMETIC:
						err = codeWriter_writeArithmetic(parser_data.command_str);
						break;
					// Intentional fall-through of C_PUSH/C_POP
					case C_PUSH:
					case C_POP:
						err = codeWriter_writePushPop(parser_data.command, parser_data.arg1, parser_data.arg2);
						break;
					case C_LABEL:
						err = codeWriter_writeLabel(parser_data.arg1);
						break;
					case C_GOTO:
						err = codeWriter_writeGoto(parser_data.arg1);
						break;
					case C_IF:
						err = codeWriter_writeIf(parser_data.arg1);
						break;
					case C_FUNCTION:
						err = codeWriter_writeFunction(parser_data.arg1, parser_data.arg2);
						break;
					case C_RETURN:
						err = codeWriter_writeReturn();
					case C_IGNORE:
						//Ignore comments and newlines
						break;
					default:
						err = -1;
						fprintf(stderr, "Unrecognized VM command: %s.\n", buffer);
				}
    		}
	        else break;
	    }

		// Close .asm file
		if( asm_file_created ) fclose(asm_file);
	}

	return err;
}