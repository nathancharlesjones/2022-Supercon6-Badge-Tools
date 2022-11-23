#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include "Parser.h"
#include "CodeWriter.h"

#define MAX_VM_COMMAND_LENGTH 128
#define MAX_FOLDER_NAME_LEN 128
#define MAX_FILE_NAME_LEN 64

#ifdef DEBUG
#	define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#	define DEBUG_PRINTF(...)
#endif

int main( int argc, char ** argv )
{
	int err = 0;
	FILE *asm_file;
	char vm_filename[MAX_FILE_NAME_LEN] = {0};
	bool asm_file_created = false;
	char slash = 0;

	if( argc != 2 )
	{
		err = -1;
		fprintf(stderr, "Invalid usage of VMTranslator. Correct usage is './VMTranslator ARG', where ARG is the path & name for a .vm file or a folder containing one or more .vm files.\n");
	}

	// Create list_of_vm_files (char **) and num_vm_files (0 to start)
	// Create project folder and .asm name strings
	// Attempt to open argv[1] as a folder
	// if success:
	//   Read contents of folder
	//   if item is not a folder and item ends in ".vm":
	//     Reallocate list (extend by one)
	//     Copy folder+string to heap
	//     list[list_len++] = string in heap
	//   Print "Found %list_len .vm files"
	//   .asm name = folder/folder_name.asm
	// else:
	//   if argv[1] ends in ".vm":
	//     list = argv
	//     list_len = 1
	//     .asm name = vm_file.asm
	//   else: error
	// Initialize codeWriter (creates ".asm" file and writes bootstrap code)
	// if success:
	//   asm_file_created == true
	//   for each .vm file in list:
	//     Attempt to open .vm file
	//     if success:
	//       Set CodeWriter file name (.vm file name)
	//       for each line in .vm file:
	//         parse data
	//         call appropriate codeWriter function
	//       close .vm file
	//     else: error
	// else: error
	// if asm_file_created:
	//   close asm_file

	// Create list_of_vm_files (char **) and num_vm_files (0 to start)
	char ** list_of_vm_files = NULL;
	int num_vm_files = 0;

	// Create project folder and .asm name strings
	char project_folder[MAX_FOLDER_NAME_LEN] = {0};
	char asm_filename[MAX_FOLDER_NAME_LEN + MAX_FILE_NAME_LEN] = {0};
	
	// Attempt to open argv[1] as a folder
	DIR * folder = opendir(argv[1]);
	
	// if success:
	if( folder != NULL )
	{
		slash = ( index(argv[1], '/') != NULL ) ? '/' : '\\';
		strcpy(project_folder, argv[1]);

		if( project_folder[strlen(project_folder)-1] == slash )
		{
			project_folder[strlen(project_folder)-1] = '\0';
		}
		char * last_slash = rindex(project_folder, slash);
		size_t name_len = strlen(last_slash+1);
		project_folder[strlen(project_folder)] = slash;
		strcpy(asm_filename, project_folder);
		strncat(asm_filename, last_slash+1, name_len);
		strcat(asm_filename, ".asm");
		DEBUG_PRINTF("Project folder: %s\n", project_folder);
		DEBUG_PRINTF("ASM filename: %s\n", asm_filename);

		struct dirent * entry;
				
		// Read contents of folder
		while( ( entry = readdir(folder) ) != NULL )
		{
			char * extension = rindex(entry->d_name, '.');

			// if item is not a folder and item ends in ".vm":
			if( ( entry->d_type != DT_DIR ) && ( strcmp(extension,".vm") == 0 ) )
			{
				list_of_vm_files = realloc(list_of_vm_files, num_vm_files+1);
				if( list_of_vm_files != NULL )
				{
					// Make space for file name in the heap
					char * temp = calloc(1, strlen(project_folder)+strlen(entry->d_name)+1);

					if( temp != NULL )
					{
						// Copy folder+string to heap
						strcpy(temp, project_folder);
						strcat(temp, entry->d_name);

						// list[list_len++] = string in heap
						list_of_vm_files[num_vm_files++] = temp;
					}
					else
					{
						err = -1;
						fprintf(stderr, "Memory allocation failure.\n");
					}
				}
				else
				{
					err = -1;
					fprintf(stderr, "Memory allocation failure.\n");
				}
			}
		}
		// Print "Found %list_len .vm files"
		DEBUG_PRINTF("Found %d .vm files.\n", num_vm_files);
		
		// Close directory
		closedir(folder);
	}
	// else:
	else
	{
		char * extension = rindex(argv[1], '.');
		// if argv[1] ends in ".vm":
		if( strcmp(extension,".vm") == 0 )
		{
			// list = argv
			list_of_vm_files = argv+1;
			// list_len = 1
			num_vm_files = 1;
			// .asm name = vm_file.asm
			// Copy all but the "vm" at the end of the file
			strncpy(asm_filename, argv[1], strlen(argv[1])-2);
			DEBUG_PRINTF("ASM filename: %s\tstrlen: %ld\n", asm_filename, strlen(asm_filename));
			strcat(asm_filename, "asm");
		}
		// else: error
		else
		{
			err = -1;
			fprintf(stderr, "Invalid usage of VMTranslator. Correct usage is './VMTranslator ARG', where ARG is the path & name for a .vm file or a folder containing one or more .vm files.\n");
		}

	}

	DEBUG_PRINTF("VM files:\n");
	for( int i = 0; i < num_vm_files; i++ )
	{
		DEBUG_PRINTF("\t%s\n", list_of_vm_files[i]);
	}
	DEBUG_PRINTF("ASM file: %s\n", asm_filename);

	// Initialize codeWriter (creates ".asm" file and writes bootstrap code)
	if( err == 0 )
	{
		err = codeWriter_init(asm_filename);
		if( err ) fprintf(stderr, "Could not initialize codeWriter. err = %d\n", err);
	}

	// if success:
	if( err == 0 )
	{
		DEBUG_PRINTF("Created new codeWriter.\n");

		// asm_file_created == true
		asm_file_created = true;
		
		// for each .vm file in list:
		for( int i = 0; i < num_vm_files; i++ )
		{
		
			// Attempt to open .vm file
			FILE * vm_file = fopen(list_of_vm_files[i], "r");
		
			// if success:
			if( vm_file != NULL )
			{
		
				// Set CodeWriter file name (.vm file name)
				char * last_slash = rindex(list_of_vm_files[i], slash);
				char * dot = rindex(list_of_vm_files[i], '.');
				strncpy(vm_filename, last_slash+1, dot-last_slash-1);
				DEBUG_PRINTF("vm_filename: %s\n", vm_filename);
				codeWriter_setFilename(vm_filename);
		
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
								break;
							case C_CALL:
								err = codeWriter_writeCall(parser_data.arg1, parser_data.arg2);
								break;
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

				// close .vm file
				fclose(vm_file);
				memset(vm_filename, 0, MAX_FILE_NAME_LEN);
			}

			// else: error
			else
			{
				err = -1;
				fprintf(stderr, "Could not open .vm file: %s\n", list_of_vm_files[i]);
			}
		}
	}
	
	// if asm_file_created: close asm_file
	if( asm_file_created ) codeWriter_close();

	/*
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
	*/

	return err;
}