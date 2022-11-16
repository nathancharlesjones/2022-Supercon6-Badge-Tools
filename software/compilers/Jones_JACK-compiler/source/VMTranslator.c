#include <stdio.h>
#include "Parser.h"
#include "CodeWriter.h"

int main( int argc, char ** argv )
{
	int err = 0;

	// Error checking
	if( argc != 2 )
	{
		err = -1;
		perror("Incorrect usage of VMTranslator. Correct usage is './VMTranslator FileName.vm'.\n\r");
	}

	// 
	if( err == 0 )
	{
		// Make new parser
		Parser parser_new(argv[1]);

		// Make new code writer
		CodeWriter codeWriter

		// Iterate through .vm file
		while( parser_hasMoreLines(parser) )
		{
			parser_advance(parser);
			switch( parser_commandType(parser) )
			{
				case C_ARITHMETIC:
					codeWriter_writeArithmetic(codeWriter, parser_arg1(parser));
					break;
				case C_PUSH:
				case C_POP:
					codeWriter_writePushPop(codeWriter, parser_commandType(parser), parser_arg1(parser), parser_arg2(parser));
					break;
				default:
					err = -1;
					perror("Unrecognized VM command: %s", parser_commandType(parser));
					break;
			}
		}

		// Close .asm file
		codeWriter_close(codeWriter);
	}

	return err;
}