#ifndef PARSER_H
#define PARSER_H

#define TEST_PARSER  // uncomment to run selfgrader for the parser

#include "lexer.h"

typedef enum {
	none,					// no errors
	lexerErr,				// lexer error
	classExpected,			// keyword class expected
	idExpected,				// identifier expected
	openBraceExpected,		// { expected
	closeBraceExpected,		// } expected
	memberDeclarErr,		// class member declaration must begin with static, field, constructor , function , or method
	classVarErr,			// class variables must begin with field or static
	illegalType,			// a type must be int, char, boolean, or identifier
	semicolonExpected,		// ; expected
	subroutineDeclarErr,	// subrouting declaration must begin with constructor, function, or method
	openParenExpected,		// ( expected
	closeParenExpected,		// ) expected
	closeBracketExpected,	// ] expected
	equalExpected,			// = expected
	syntaxError,			// any other kind of syntax error
	// extend this list to include two types of semantic errors
	undecIdentifier,		// undeclared identifier (e.g. class, subroutine, or variable)
	redecIdentifier			// redeclaration of identifier in the same scope
} SyntaxErrors;


// every parsing function should return this struct
// the struct contains an error type field (er)
// and a token field (tk) that is set to the token at or near which the error was encountered
// if no errors are encountered er should be set to none
typedef struct
{
	SyntaxErrors er;
	Token tk;
	// other info for semantic and code generation purposes

} ParserInfo;

int InitParser (char* file_name); // initialise the parser to parse source code in file_name
ParserInfo Parse (); // parse the input file (the one passed to InitParser)
int StopParser (); // stop the parser and do any necessary clean up
char* ErrorString (SyntaxErrors e);
void PrintError (ParserInfo pn);
#endif
