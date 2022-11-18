#ifndef CODEWRITER_H
#define CODEWRITER_H

#include "Parser.h"

typedef struct CodeWriter_t * CodeWriter;

int codeWriter_new(FILE * asm_file, CodeWriter * newCodeWriter);
int codeWriter_writeArithmetic(CodeWriter thisCodeWriter, char * op);
int codeWriter_writePushPop(CodeWriter thisCodeWriter, commandType command, char * memory, int offset);

#endif //CODEWRITER_H
