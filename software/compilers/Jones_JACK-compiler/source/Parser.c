#include <stdio.h>
#include <string.h>
#include "Parser.h"

int parse(char * command, Parser_t * parser_data)
{
	int err = 0;

	if( command == NULL ) err = 1;

	if( err == 0 )
	{
		if( parser_data == NULL ) err = 2;
	}

	if( err == 0 )
	{
		int matched = sscanf(command, "%s %s %d", parser_data->command_str, parser_data->arg1, &(parser_data->arg2));
		if( matched > 0 )
		{
			if( strcmp(parser_data->command_str, "push") == 0 ) parser_data->command = C_PUSH;
			else if( strcmp(parser_data->command_str, "pop") == 0 ) parser_data->command = C_POP;
			else if(	( strcmp(parser_data->command_str, "add") == 0 ) ||
						( strcmp(parser_data->command_str, "sub") == 0 ) ||
						( strcmp(parser_data->command_str, "neg") == 0 ) ||
						( strcmp(parser_data->command_str, "eq") == 0 ) ||
						( strcmp(parser_data->command_str, "gt") == 0 ) ||
						( strcmp(parser_data->command_str, "lt") == 0 ) ||
						( strcmp(parser_data->command_str, "and") == 0 ) ||
						( strcmp(parser_data->command_str, "or") == 0 ) ||
						( strcmp(parser_data->command_str, "not") == 0 ) ) parser_data->command = C_ARITHMETIC;
			else if( strcmp(parser_data->command_str, "label") == 0 ) parser_data->command = C_LABEL;
			else if( strcmp(parser_data->command_str, "goto") == 0 ) parser_data->command = C_GOTO;
			else if( strcmp(parser_data->command_str, "if-goto") == 0 ) parser_data->command = C_IF;
			else if( strcmp(parser_data->command_str, "function") == 0 ) parser_data->command = C_FUNCTION;
			else if( strcmp(parser_data->command_str, "return") == 0 ) parser_data->command = C_RETURN;
			else if( strcmp(parser_data->command_str, "call") == 0 ) parser_data->command = C_CALL;
			else if( strcmp(parser_data->command_str, "//") == 0 ) parser_data->command = C_IGNORE;
			else parser_data->command = C_UNKNOWN_COMMAND;
		}
		else parser_data->command = C_IGNORE;
	}

	return err;
}
