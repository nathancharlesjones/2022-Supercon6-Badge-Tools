#include <stdio.h>
#include "Parser.h"

#define MAX_ARG1_LENGTH 10
#define MAX_CMD_LENGTH 25

typedef struct Parser_t
{
	char vm_file[MAX_CHARS_FILENAME],
	FILE * vm_file_h,
	bool opened,
	char command[MAX_CMD_LENGTH];
};

int parser_new(char * vm_file, Parser newParser)
{
	int err = 0;

	if( vm_file == NULL ) err = 1;

	if( err == 0 )
	{
		if( strlen(vm_file) > MAX_CHARS_FILENAME ) err = 3;
	}

	if( err == 0)
	{
		if( newParser == NULL ) err = 4;
	}

	if( err == 0 )
	{
		newParser = calloc(1, sizeof(Parser_t));
		if( newParser == NULL ) err = 2;
	}
	
	if( err == 0 )
	{
		strncpy(newParser.vm_file, vm_file, MAX_CHARS_FILENAME);
	}
	
	return err;
}

int parser_hasMoreLines(Parser thisParser, bool * hasMoreLines)
{
	int err = 0;

	if( thisParser == NULL ) err = 1;

	if( err == 0 )
	{
		if( hasMoreLines == NULL ) err = 2;
	}

	if( err == 0 )
	{
		if( thisParser.opened = false ) thisParser.vm_file_h = fopen(thisParser.vm_file, "r");
		if( thisParser.vm_file_h == NULL ) err = 3;
	}

	if( err == 0 )
	{
		if( feof(thisParser.vm_file_h) )
		{
			*hasMoreLines = false;
			fclose(thisParser.vm_file_h);
		}
		else
		{
			*hasMoreLines = true;
		}
	}

	return err;
}

int parser_advance(Parser thisParser)
{
	int err = 0;

	if( thisParser == NULL ) err = 1;

	if( err == 0 )
	{
		if( fgets(thisParser.command, MAX_CMD_LENGTH, thisParser.vm_file_h) == NULL ) err = 2;
	}

	return err;
}
int parser_commandType(Parser thisParser, commandType * thisCommand)
{
	int err = 0;

	if( thisParser == NULL ) err = 1;

	if( err == 0 )
	{
		if( thisCommand == NULL ) err = 2;
	}

	if( err == 0 )
	{
		asdf
	}

	return err;
}


int parser_arg1(Parser thisParser, char * arg1, int n);


int parser_arg2(Parser thisParser, int * arg2);