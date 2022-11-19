#ifndef CODEWRITER_H
#define CODEWRITER_H

#include "Parser.h"

typedef struct CodeWriter_t * CodeWriter;

/*********************************************
 * Function: codeWriter_new
 *****************************************//**
* \b Description:
*
* Allocates dynamic memory for and nitializes a CodeWriter_t object.
*
* @param[in] 	asm_file 		File handle for where to write the assembly
* 								code. File should already have been opened
* 								with a valid file path.
* @param[out]	newCodeWriter	Pointer to a CodeWriter_t object. Will be set
* 								to a spot in the heap for the actual object
* 								and initialized with the asm_file.
*
* @return       int 			Error code.
* 									0: No error
* 									1: asm_file == NULL
* 									2: newCodeWriter == NULL
* 									3: Memory allocation failed
**/
int codeWriter_new(FILE * asm_file, CodeWriter * newCodeWriter);

/*********************************************
 * Function: codeWriter_writeArithmetic
 *****************************************//**
* \b Description:
*
* Writes a series of Hack CPU assembly instruction to the asm_file
* which performs the arithmetic or logical operation specified by "op".
*
* @param[in] 	thisCodeWriter	CodeWriter_t object to do the writing with.
* @param[in]	op				Arithemtic or logical operation to perform.
* 								Valid values are:
* 									- "add"
* 									- "sub"
* 									- "neg"
* 									- "eq"
* 									- "gt"
* 									- "lt"
* 									- "and"
* 									- "or"
* 									- "not"
*
* @return       int 			Error code.
* 									0: No error
* 									1: thisCodeWriter == NULL
* 									2: op == NULL
* 									3: Unrecognized operation
**/
int codeWriter_writeArithmetic(CodeWriter thisCodeWriter, char * op);

/*********************************************
 * Function: codeWriter_writePushPop
 *****************************************//**
* \b Description:
*
* Writes a series of Hack CPU assembly instruction to the asm_file
* which performs the stack operation specified by "command" (either
* C_PUSH or C_POP). "memory" and "offset" specify either the place
* where the popped value will be stored or where to find the value
* that will be pushed on to the stack.
*
* @param[in] 	thisCodeWriter	CodeWriter_t object to do the writing with.
* @param[in]	command			Must be either C_PUSH or C_POP (see "Parser.h").
* @param[in]	memory			Memory segment to be pushed from or popped to.
* 								Valid values are:
* 									- "local"
* 									- "argument"
* 									- "this"
* 									- "that"
* @param[in]	offset			Offset from the base memory segment where the value
* 								will be pushed from or popped to.
*
* @return       int 			Error code.
* 									0: No error
* 									1: thisCodeWriter == NULL
* 									2: memory == NULL
* 									3: Unrecognized memory segment
* 									4: Invalid "pop"; cannot pop into constant memory segment
* 									5: Command did not match either C_PUSH or C_POP
**/
int codeWriter_writePushPop(CodeWriter thisCodeWriter, commandType command, char * memory, int offset);

#endif //CODEWRITER_H
