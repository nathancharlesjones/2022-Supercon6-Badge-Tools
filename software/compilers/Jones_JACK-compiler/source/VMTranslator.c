#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include <libgen.h>
#include "VMTranslator.h"
#include "Parser.h"
#include "CodeWriter.h"
#include "assert.h"

int main( int argc, char ** argv )
{
	FILE *asm_file;
	char slash = 0;
	char ** list_of_vm_files = NULL;
	int num_vm_files = 0;
	char project_folder[MAX_FOLDER_NAME_LEN] = {0};
	char asm_filename[MAX_FOLDER_NAME_LEN + MAX_FILE_NAME_LEN] = {0};

	ASSERT( argc == 2, "Invalid usage of VMTranslator. Correct usage is './VMTranslator ARG', where ARG is the path & name for a .vm file or a folder containing one or more .vm files.\n" );
	
	// STEP 1: Determine if 

	// Attempt to open argv[1] as a folder
	DIR * folder = opendir(argv[1]);
	
	// if success:
	if( folder != NULL )
	{
		extract_project_folder_and_asm_filename_from_argv1(argv[1], project_folder, asm_filename);

		slash = type_of_slash_used_in_argv1(project_folder);		

		DEBUG_PRINTF("Project folder: %s\n", project_folder);
		DEBUG_PRINTF("ASM filename: %s\n", asm_filename);

		extract_vm_files_from_directory(folder, project_folder, &list_of_vm_files, &num_vm_files);

		// Print "Found %list_len .vm files"
		DEBUG_PRINTF("Found %d .vm files.\n", num_vm_files);
		
		// Close directory
		closedir(folder);
	}
	// else:
	else
	{
		// argv[1] should be a .vm file

		char * extension = rindex(argv[1], '.');
		// Check that argv[1] ends in ".vm":
		ASSERT(strcmp(extension,".vm") == 0 , "Invalid usage of VMTranslator. Correct usage is './VMTranslator ARG', where ARG is the path & name for a .vm file or a folder containing one or more .vm files.\n");
		
		// list = argv
		list_of_vm_files = argv+1;

		// list_len = 1
		num_vm_files = 1;
		
		// .asm name = vm_file.asm
		// Copy all but the "vm" at the end of the file
		strncpy(asm_filename, argv[1], strlen(argv[1])-2);
		DEBUG_PRINTF("ASM filename: %s\n", asm_filename);
		strcat(asm_filename, "asm");
	}

	DEBUG_PRINTF("VM files:\n");
	for( int i = 0; i < num_vm_files; i++ )
	{
		DEBUG_PRINTF("\t%s\n", list_of_vm_files[i]);
	}
	DEBUG_PRINTF("ASM file: %s\n", asm_filename);
	
	// Initialize codeWriter (creates ".asm" file and writes bootstrap code)
	ASSERT(codeWriter_init(asm_filename) == NO_ERR, "Could not initialize codeWriter.\n");
	DEBUG_PRINTF("Created new codeWriter.\n");

	// for each .vm file in list:
	for( int i = 0; i < num_vm_files; i++ )
	{
	
		// Attempt to open .vm file
		FILE * vm_file = fopen(list_of_vm_files[i], "r");
		ASSERT( vm_file != NULL, "Could not open .vm file.\n");

		set_codeWriter_vm_filename(list_of_vm_files[i]);

		// for each line in .vm file:
		while( true )
		{
			char buffer[MAX_VM_COMMAND_LENGTH];
	        char * done = fgets(buffer, MAX_VM_COMMAND_LENGTH, vm_file);
	        if( done != NULL )
        	{
        		DEBUG_PRINTF("vm file has more lines\n");

				// parse data
        		Parser_t parser_data = {0};						
        		parse(buffer, &parser_data);
        		DEBUG_PRINTF("Current command: %d\t%s\t%d\n", parser_data.command, parser_data.arg1, parser_data.arg2);
				
				// call appropriate codeWriter function
        		switch( parser_data.command )
				{
					case C_ARITHMETIC:
						ASSERT(codeWriter_writeArithmetic(parser_data.command_str) == NO_ERR, "Could not write arithmetic command.\n");
						break;
					// Intentional fall-through of C_PUSH/C_POP
					case C_PUSH:
					case C_POP:
						ASSERT(codeWriter_writePushPop(parser_data.command, parser_data.arg1, parser_data.arg2) == NO_ERR, "Could not write push/pop command.\n");
						break;
					case C_LABEL:
						ASSERT(codeWriter_writeLabel(parser_data.arg1) == NO_ERR, "Could not write label.\n");
						break;
					case C_GOTO:
						ASSERT(codeWriter_writeGoto(parser_data.arg1) == NO_ERR, "Could not write goto.\n");
						break;
					case C_IF:
						ASSERT(codeWriter_writeIf(parser_data.arg1) == NO_ERR, "Could not write if-goto.\n");
						break;
					case C_FUNCTION:
						ASSERT(codeWriter_writeFunction(parser_data.arg1, parser_data.arg2) == NO_ERR, "Could not write function.\n");
						break;
					case C_RETURN:
						ASSERT(codeWriter_writeReturn() == NO_ERR, "Could not write return command.\n");
						break;
					case C_CALL:
						ASSERT(codeWriter_writeCall(parser_data.arg1, parser_data.arg2) == NO_ERR, "Could not write call command.\n");
						break;
					case C_IGNORE:
						//Ignore comments and newlines
						break;
					default:
						ASSERT(0, "Unrecognized VM command.\n");
				}
    		}
	        else break;
	    }

		// close .vm file
		fclose(vm_file);
	}
	
	codeWriter_close();

	return 0;
}

char type_of_slash_used_in_argv1(char * argv1)
{
	return ( index(argv1, '/') != NULL ) ? '/' : '\\';
}

void extract_project_folder_and_asm_filename_from_argv1(char * argv1, char * project_folder, char * asm_filename)
{
	// Expand directory to full path and copy to project_folder
	realpath(argv1, project_folder);

	// Determine if forward or backward slashes are used
	char slash = ( index(project_folder, '/') != NULL ) ? '/' : '\\';

	// Add trailing slash back to project_folder
	strncat(project_folder, &slash, 1);

	// Copy project_folder to asm_filename
	strcpy(asm_filename, project_folder);
	
	// Copy directory name to asm_filename
	char buffer[MAX_FOLDER_NAME_LEN+MAX_FILE_NAME_LEN] = {0};
	strcpy(buffer, project_folder);
	strcat(asm_filename, basename(buffer));

	// Add file extension to asm_filename
	strcat(asm_filename, ".asm");
}

void extract_vm_files_from_directory(DIR * folder, char * project_folder, char *** list_of_vm_files, int * num_vm_files)
{
	struct dirent * entry;
				
	// Read contents of folder
	while( ( entry = readdir(folder) ) != NULL )
	{
		char * extension = rindex(entry->d_name, '.');

		// if item is not a folder and item ends in ".vm":
		if( ( entry->d_type != DT_DIR ) && ( strcmp(extension,".vm") == 0 ) )
		{
			(*list_of_vm_files) = realloc((*list_of_vm_files), (*num_vm_files)+1);
			ASSERT((*list_of_vm_files) != NULL, "Memory allocation failure.\n");
			
			// Make space for file name in the heap
			char * temp = calloc(1, strlen(project_folder)+strlen(entry->d_name)+1);
			ASSERT(temp != NULL, "Memory allocation failure.\n");

			// Copy folder+string to heap
			strcpy(temp, project_folder);
			strcat(temp, entry->d_name);

			// list[list_len++] = string in heap
			(*list_of_vm_files)[(*num_vm_files)++] = temp;
		}
	}
}

void set_codeWriter_vm_filename(char * vm_full)
{
	char vm_filename[MAX_FILE_NAME_LEN] = {0};
	// Set CodeWriter file name (.vm file name)
	strcpy(vm_filename, basename(vm_full));
	// Remove file extension
	vm_filename[strlen(vm_filename)-3] = '\0';
	DEBUG_PRINTF("vm_filename: %s\n", vm_filename);
	codeWriter_setFilename(vm_filename);
}