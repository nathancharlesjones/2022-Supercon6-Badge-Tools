#ifndef PARSER_H
#define PARSER_H

#define MAX_CMD_LENGTH 10
#define MAX_MEM_LENGTH 10

typedef enum commandType
{
	C_ARITHMETIC,
	C_PUSH,
	C_POP,
	C_LABEL,
	C_GOTO,
	C_IF,
	C_FUNCTION,
	C_RETURN,
	C_CALL,
	C_IGNORE,
	C_UNKNOWN_COMMAND, // For error reporting purposes
	NUM_COMMAND_TYPES
} commandType;

typedef struct Parser_t
{
	commandType command;
	char		command_str[MAX_CMD_LENGTH];
	char		memory[MAX_MEM_LENGTH];
	int 		segment;
} Parser_t;

/*********************************************
 * Function: parse
 *****************************************//**
* \b Description:
*
* Fills in a Parser_t object from a command string.
*
* @param[in] 	command 		Command string to be scanned.
* @param[out]	parser_data		Pointer to Parser_t object in which to
* 								populate the data from the command string.
*
* @return       int 			Error code.
* 									0: No error
* 									1: command == NULL
* 									2: parser_data == NULL
**/
int parse(char * command, Parser_t * parser_data);

#endif // PARSER_H