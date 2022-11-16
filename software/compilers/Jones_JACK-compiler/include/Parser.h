#ifndef PARSER_H
#define PARSER_H

#define MAX_CHARS_FILENAME 128

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
	NUM_COMMAND_TYPES
};

typedef Parser_t * Parser;

/*********************************************
 * Function: parser_new
 *****************************************//**
* \b Description:
*
* Creates a new Parser object from the heap.
*
* @param[in] 	vm_file 		File name for the .vm file to be parsed.
* @param[out]	newParser		Pointer to new Parser object.
*
* @return       int 			Error code.
* 									0: No error
* 									1: vm_file == NULL
* 									2: Memory allocation failed
* 									3: vm_file too long
* 									4: newParser == NULL
**/
int parser_new(char * vm_file, Parser newParser);

/*********************************************
 * Function: parser_hasMoreLines
 *****************************************//**
* \b Description:
*
* Identifies if there are more lines remaining to be processed in
* the provided .vm file. Opens the .vm file on first call; closes
* the .vm file automatically when there are no more lines.
*
* @param[in] 	thisParser 		Parser object to be checked
* @param[out] 	hasMoreLines 	Pointer to bool variable where result will
* 								will be stored ('true' if the .vm file has
* 								more lines to be read; 'false' otherwise).
*
* @return       int 			Error code. A value is only written to hasMoreLines
* 								if return value == 0.
* 									0: No error
* 									1: thisParser == NULL
* 									2: hasMoreLines == NULL
* 									3: Error in opening file
**/
int parser_hasMoreLines(Parser thisParser, bool * hasMoreLines);

/*********************************************
 * Function: parser_advance
 *****************************************//**
* \b Description:
*
* Advances the command being processed to the next line/command in the
* .vm file.
*
* @param[in] 	thisParser 		Parser object to be advanced
*
* @return       int 			Error code.
* 									0: No error
* 									1: thisParser == NULL
* 									2: Error reading .vm file
**/
int parser_advance(Parser thisParser);

/*********************************************
 * Function: parser_commandType
 *****************************************//**
* \b Description:
*
* Identifies the type of the current command being process.
*
* @param[in] 	thisParser 		Parser object to be checked
* @param[out] 	thisCommand 	Pointer to commandType variable where result
* 								will be stored.
*
* @return       int 			Error code. A value is only written to thisCommand
* 								if return value == 0.
* 									0: No error
* 									1: thisParser == NULL
* 									2: thisCommand == NULL
* 									3: Unidentified command
**/
int parser_commandType(Parser thisParser, commandType * thisCommand);

/*********************************************
 * Function: parser_arg1
 *****************************************//**
* \b Description:
*
* Identifies the first argument in the current command being processed.
* In the case of arithmetic commands, this value is the command itself
* (e.g. "add", "not", etc). In the case of PUSH/POP commands, this value
* is the memory segment being addressed (e.g. in "push local 2", this
* function would return "local").
*
* @param[in] 	thisParser 		Parser object to be checked
* @param[out] 	arg1 		 	Pointer to char array where result will
* 								will be stored. Up to n chars will be 
* 								written. Array should be >9; large enough to
* 								hold the largest possible argument ("argument")
* 								plus the nul terminator.
* @param[in] 	n 				Number of characters to write to arg1. n 
* 								should be >9 to accommodate the largest 
* 								possible argument ("argument") plus the 
* 								nul terminator.
*
* @return       int 			Error code. For codes marked with an asterisk
* 								(*), no value is written to arg1.
* 									0: No error
* 									1*: thisParser == NULL
* 									2*: arg1 == NULL
* 									3: n chars written, but more were available
**/
int parser_arg1(Parser thisParser, char * arg1, int n);

/*********************************************
 * Function: parser_arg2
 *****************************************//**
* \b Description:
*
* Identifies the second argument in the current command being processed.
* In the case of "push local 2", this function puts "2" (as an integer)
* into arg2. No value is placed in arg2 if the command being processed
* does not have a second argument (e.g. in the case of "add").
*
* @param[in] 	thisParser 		Parser object to be checked
* @param[out] 	arg2 		 	Pointer to int variable where result will
* 								will be stored.
*
* @return       int 			Error code. A value is only written to arg2
* 								if return value == 0.
* 									0: No error
* 									1: thisParser == NULL
* 									2: arg2 == NULL
* 									3: Could not convert arg2 to int
**/
int parser_arg2(Parser thisParser, int * arg2);


#endif // PARSER_H