#ifndef CODEWRITER_H
#define CODEWRITER_H

#include "Parser.h"

#define MAX_FCN_NAME_LEN 64

/*********************************************
 * Function: codeWriter_init
 *****************************************//**
* \b Description:
*
* Initializes the CodeWriter.
*
* @param[in] 	asm_file 		File handle for where to write the assembly
* 								code. File should already have been opened
* 								with a valid file path.
*
* @return       int 			Error code.
* 									0: No error
* 									1: CodeWriter already initialized
*                                   2: this_asm_file == NULL
**/
int codeWriter_init(FILE * asm_file);

/*********************************************
 * Function: codeWriter_setFilename
 *****************************************//**
* \b Description:
*
* Sets the .vm filename for the CodeWriter.
*
* @param[in]    vm_filename     File name for .vm file currently being processed.
*                               Used to differentiate static variables used in
*                               separate .vm files.
*
* @return       int             Error code.
*                                   0: No error
*                                   1: CodeWriter not initialized
*                                   2: vm_filename == NULL
**/
int codeWriter_setFilename(const char * vm_filename);

/*********************************************
 * Function: codeWriter_writeArithmetic
 *****************************************//**
* \b Description:
*
* Writes a series of Hack CPU assembly instruction to the asm_file
* which performs the arithmetic or logical operation specified by "op".
*
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
* 									1: CodeWriter not initialized
* 									2: op == NULL
* 									3: Unrecognized operation
**/
int codeWriter_writeArithmetic(char * op);

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
* 									1: CodeWriter not initialized
*                                   3: memory == NULL
* 									4: Unrecognized memory segment
* 									5: Invalid "pop"; cannot pop into constant memory segment
* 									6: Command did not match either C_PUSH or C_POP
**/
int codeWriter_writePushPop(commandType command, char * memory, int offset);

/*********************************************
 * Function: codeWriter_writeLabel
 *****************************************//**
* \b Description:
*
* Emits a label to the assembly file that matches the corresponding
* label in the .vm file.
*
* @param[in]    label           Label name to emit to the assembly file.
*
* @return       int             Error code.
*                                   0: No error
*                                   1: CodeWriter not initialized
*                                   2: label == NULL
**/
int codeWriter_writeLabel(char * label);

/*********************************************
 * Function: codeWriter_writeGoto
 *****************************************//**
* \b Description:
*
* Enacts a jump to the label given. Assumes that the label and the goto
* exist in the same function.
*
* @param[in]    label           Label name of destination for 'goto'.
*
* @return       int             Error code.
*                                   0: No error
*                                   1: CodeWriter not initialized
*                                   2: label == NULL
**/
int codeWriter_writeGoto(char * label);

/*********************************************
 * Function: codeWriter_writeIf
 *****************************************//**
* \b Description:
*
* Enacts a conditional jump to the label given. Pops the top value off the
* stack and executes the jump if the popped value is 'true' (-1). Assumes
* that the label and the goto exist in the same function.
*
* @param[in]    label           Label name of destination for 'if-goto'.
*
* @return       int             Error code.
*                                   0: No error
*                                   1: CodeWriter not initialized
*                                   2: label == NULL
**/
int codeWriter_writeIf(char * label);

/*********************************************
 * Function: codeWriter_writeFunction
 *****************************************//**
* \b Description:
*
* Emits a function with "name", preparing the local stack by executing
* "push 0" argc times.
*
* @param[in]    name            Name of the function.
* @param[in]    argc            Number of arguments to the function.
*
* @return       int             Error code.
*                                   0: No error
*                                   1: CodeWriter not initialized
*                                   2: name == NULL
**/
int codeWriter_writeFunction(char * name, int argc);

/*********************************************
 * Function: codeWriter_writeReturn
 *****************************************//**
* \b Description:
*
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
*
* @return       int             Error code.
*                                   0: No error
*                                   1: CodeWriter not initialized
**/
int codeWriter_writeReturn(void);

#endif //CODEWRITER_H
