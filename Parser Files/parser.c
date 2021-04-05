#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"

// you can declare prototypes of parser functions below

ParserInfo Type();
ParserInfo relationalExpression();
ParserInfo expression();
ParserInfo expressionList();
ParserInfo term();
ParserInfo factor();
ParserInfo operand();
ParserInfo arithmeticExpression();
ParserInfo varDeclarStatement();
ParserInfo letStatement();
ParserInfo ifStatement();
ParserInfo whileStatement();
ParserInfo subroutineCall();
ParserInfo doStatement();
ParserInfo returnStatement();
ParserInfo subroutineBody();
ParserInfo statement();
ParserInfo paramList();
ParserInfo subroutineDeclar();
ParserInfo classVarDeclar();
ParserInfo memberDeclar();
ParserInfo classDeclar();

void error(char *s)
{
	printf("%s\n", s);
	//exit(0);
}

ParserInfo type()
{
	Token next_token = GetNextToken();
	if (next_token.tp == RESWORD)
	{
		if (strcmp(next_token.lx, "int") == 0 || strcmp(next_token.lx, "char") == 0 || strcmp(next_token.lx, "boolean") == 0)
		{
			;
		}
	}
	else if (next_token.tp == ID)
	{
		;
	}
	else
	{
		error("Expected a type\n");
	}
}

ParserInfo operand()
{
	Token next_token = GetNextToken();
	if (next_token.tp == INT || next_token.tp == ID || !strcmp(next_token.lx, "(") || next_token.tp == STRING || !strcmp(next_token.lx, "true") || !strcmp(next_token.lx, "false") || !strcmp(next_token.lx, "null") || !strcmp(next_token.lx, "this"))
	{
		if (next_token.tp == ID)
		{
			next_token = PeekNextToken();
			if (!strcmp(next_token.lx, "."))
			{
				GetNextToken();
				next_token = GetNextToken();
				if (next_token.tp == ID)
				{
					;
				}
				else
				{
					error("Expected an ID");
				}
			}
			next_token = PeekNextToken();
			if (!strcmp(next_token.lx, "[") || !strcmp(next_token.lx, "("))
			{
				if (!strcmp(next_token.lx, "[")) {
					GetNextToken();
					expression();
					next_token = GetNextToken();
					if (!strcmp(next_token.lx, "]")) {
						;
					}
					else {
						error("Expected ]\n");
					}
				}
				if (!strcmp(next_token.lx, "(")) {
					GetNextToken();
					expressionList();
					next_token = GetNextToken();
					if (!strcmp(next_token.lx, ")")) {
						;
					}
					else {
						error("Expected )\n");
					}
				}
			}
		}
		else if (!strcmp(next_token.lx, "("))
		{
			GetNextToken();
			expression();
			next_token = GetNextToken();
			if (!strcmp(next_token.lx, ")"))
			{
				;
			}
			else
			{
				error("expected )\n");
			}
		}
	}
	else
	{
		error("Expected an operand\n");
	}
}

ParserInfo factor()
{
	Token next_token = PeekNextToken();
	if (!strcmp(next_token.lx, "-") || !strcmp(next_token.lx, "~"))
	{
		GetNextToken();
		operand();
	}
	else
	{
		operand();
	}
}

ParserInfo term()
{
	factor();
	Token next_token = PeekNextToken();
	while (!strcmp(next_token.lx, "*") || !strcmp(next_token.lx, "/"))
	{
		GetNextToken();
		factor();
		next_token = PeekNextToken();
	}
}

ParserInfo arithmeticExpression()
{
	term();
	Token next_token = PeekNextToken();
	while (!strcmp(next_token.lx, "+") || !strcmp(next_token.lx, "-"))
	{
		GetNextToken();
		term();
		next_token = PeekNextToken();
	}
}

ParserInfo relationalExpression()
{
	arithmeticExpression();
	Token next_token = PeekNextToken();
	while (!strcmp(next_token.lx, "=") || !strcmp(next_token.lx, ">") || !strcmp(next_token.lx, "<"))
	{
		GetNextToken();
		arithmeticExpression();
		next_token = PeekNextToken();
	}
}

ParserInfo expression()
{
	relationalExpression();
	Token next_token = PeekNextToken();
	if (next_token.tp == SYMBOL && (!strcmp(next_token.lx, "&") || !strcmp(next_token.lx, "|")))
	{
		while (!strcmp(next_token.lx, "&"), !strcmp(next_token.lx, "|"))
		{
			GetNextToken();
			relationalExpression();
			next_token = PeekNextToken();
		}
	}
}

ParserInfo varDeclarStatement()
{
	Token next_token = GetNextToken();
	if (strcmp(next_token.lx, "var") == 0)
	{
		type();
		Token next_token = GetNextToken();
		if (next_token.tp == ID)
		{
			;
		}
		else
		{
			error("Expected identifier");
		}
		next_token = PeekNextToken();
		while (next_token.tp == SYMBOL && next_token.lx == ",")
		{
			GetNextToken();
			next_token = PeekNextToken();
			if (next_token.tp == ID)
			{
				GetNextToken();
				next_token = PeekNextToken();
			}
			else
			{
				error("Expected ID\n");
			}
		}
	}
	else
	{
		error("Expected var\n");
	}
}

ParserInfo letStatement()
{
	Token next_token = GetNextToken();
	if (strcmp(next_token.lx, "let") == 0)
	{
		next_token = GetNextToken();
		if (next_token.tp == ID)
		{
			;
		}
		else
		{
			error("Expected ID\n");
		}

		next_token = PeekNextToken();
		if (!strcmp(next_token.lx, "["))
		{
			expression();
		}
		next_token = GetNextToken();
		if (!strcmp(next_token.lx, "]"))
		{
			next_token = GetNextToken();
			if (!strcmp(next_token.lx, "="))
			{
				expression();
			}
			else
			{
				error("Expected =\n");
			}
		}
		else
		{
			error("Expected }\n");
		}
	}
	else
	{
		error("Let expected\n");
	}
}

ParserInfo ifStatement()
{
	Token next_token = GetNextToken();
	if (strcmp(next_token.lx, "if") == 0)
	{
		next_token = GetNextToken();
		if (!strcmp(next_token.lx, "("))
		{
			expression();
		}
		else
		{
			error("Expected (\n");
		}
		next_token = GetNextToken();
		if (!strcmp(next_token.lx, ")"))
		{
			;
		}
		else
		{
			error("Expected )\n");
		}
		next_token = PeekNextToken();
		if (!strcmp(next_token.lx, "{"))
		{
			subroutineBody();
		}
		next_token = PeekNextToken();
		if (!strcmp(next_token.lx, "else"))
		{
			GetNextToken();
			next_token = GetNextToken();
			if (!strcmp(next_token.lx, "{"))
			{
				subroutineBody();
			}
			else
			{
				error("Expected {\n");
			}
		}
	}
}

ParserInfo whileStatement()
{
	Token next_token = GetNextToken();
	if (!strcmp(next_token.lx, "while"))
	{
		next_token = GetNextToken();
		if (!strcmp(next_token.lx, "("))
		{
			expression();
		}
		else
		{
			error("Expected a (\n");
		}
		next_token = GetNextToken();
		if (!strcmp(next_token.lx, ")"))
		{
			;
		}
		else
		{
			error("Expected )\n");
		}
		next_token = PeekNextToken();
		if (!strcmp(next_token.lx, "{"))
		{
			subroutineBody();
		}
	}
}

ParserInfo subroutineCall()
{
	Token next_token = GetNextToken();
	if (next_token.tp == ID)
	{
		next_token = PeekNextToken();
		if (!strcmp(next_token.lx, "."))
		{
			GetNextToken();
			next_token = GetNextToken();
			if (next_token.tp == ID)
			{
				;
			}
			else
			{
				error("Expected an ID");
			}
		}
		next_token = GetNextToken();
		if (!strcmp(next_token.lx, "("))
		{
			expressionList();
		}
		else
		{
			error("Expected (\n");
		}
		next_token = GetNextToken();
		if (!strcmp(next_token.lx, ")"))
		{
			;
		}
		else
		{
			error("Expected )\n");
		}
	}
	else
	{
		error("Expected an identifier\n");
	}
}

ParserInfo doStatement()
{
	Token next_token = GetNextToken();
	if (strcmp(next_token.lx, "do") == 0)
	{
		subroutineCall();
		next_token = GetNextToken();
		if (!strcmp(next_token.lx, ";"))
		{
			;
		}
		else
		{
			error("Expected ;\n");
		}
	}
}

ParserInfo returnStatement()
{
	Token next_token = GetNextToken();
	if (!strcmp(next_token.lx, "return"))
	{
		next_token = GetNextToken();
		if (!strcmp(next_token.lx, ";"))
		{
			;
		}
		else
		{
			expression();
		}
		next_token = GetNextToken();
		if (!strcmp(next_token.lx, ";"))
		{
			;
		}
		else
		{
			error("Expected ;\n");
		}
	}
}

ParserInfo subroutineBody()
{
	Token next_token = GetNextToken();
	if (!strcmp(next_token.lx, "{"))
	{
		next_token = PeekNextToken();
		if (!strcmp(next_token.lx, "var") || !strcmp(next_token.lx, "let") || !strcmp(next_token.lx, "if") || !strcmp(next_token.lx, "while") || !strcmp(next_token.lx, "do") || !strcmp(next_token.lx, "return"))
		{
			statement();
		}
		next_token = GetNextToken();
		if (!strcmp(next_token.lx, "}"))
		{
			;
		}
		else
		{
			error("Expected }");
		}
	}
	else
	{
		error("Expected {\n");
	}
}

ParserInfo statement()
{
	Token next_token = PeekNextToken();
	if (strcmp(next_token.lx, "var") == 0)
	{
		varDeclarStatement();
	}
	else if (strcmp(next_token.lx, "let") == 0)
	{
		letStatement();
	}
	else if (strcmp(next_token.lx, "if") == 0)
	{
		ifStatement();
	}
	else if (strcmp(next_token.lx, "while") == 0)
	{
		whileStatement();
	}
	else if (strcmp(next_token.lx, "do") == 0)
	{
		doStatement();
	}
	else if (strcmp(next_token.lx, "return") == 0)
	{
		returnStatement();
	}
}

ParserInfo paramList()
{
	Token next_token = GetNextToken();
	if (strcmp(next_token.lx, ")") == 0)
	{
		;
	}
	else
	{
		type();
		Token next_token = GetNextToken();
		if (next_token.tp == ID)
		{
			;
		}
		else
		{
			error("Expected identifier");
		}
		next_token = PeekNextToken();
		while (next_token.tp == SYMBOL && next_token.lx == ",")
		{
			GetNextToken();
			next_token = PeekNextToken();
			if (next_token.tp == ID)
			{
				GetNextToken();
				next_token = PeekNextToken();
			}
			else
			{
				error("Expected ID\n");
			}
		}
	}
}

ParserInfo subroutineDeclar()
{
	Token next_token = GetNextToken();
	if (!strcmp(next_token.lx, "constructor") || !strcmp(next_token.lx, "function") || !strcmp(next_token.lx, "method"))
	{
		next_token = GetNextToken();
		if (!strcmp(next_token.lx, "void"))
		{
			;
		}
		else
		{
			type();
		}
		next_token = GetNextToken();
		if (next_token.tp == ID)
		{
			next_token = GetNextToken();
			if (strcmp(next_token.lx, "(") == 0)
			{
				paramList();
				next_token = GetNextToken();
				if (strcmp(next_token.lx, ")") == 0)
				{
					next_token = PeekNextToken();
					if (!strcmp(next_token.lx, "{"))
					{
						subroutineBody();
					}
				}
				else
				{
					error("Expected )\n");
				}
			}
			else
			{
				error("Expected (\n");
			}
		}
		else
		{
			error("Expected ID\n");
		}
	}
}

ParserInfo classVarDeclar()
{
	Token next_token = GetNextToken();
	if (strcmp(next_token.lx, "static") == 0 || strcmp(next_token.lx, "field"))
	{
		type();
		next_token = GetNextToken();
		if (next_token.tp == ID)
		{
			;
		}
		else
		{
			error("Expected identifier");
		}
		next_token = PeekNextToken();
		while (next_token.tp == SYMBOL && next_token.lx == ",")
		{
			GetNextToken();
			next_token = PeekNextToken();
			if (next_token.tp == ID)
			{
				GetNextToken();
				next_token = PeekNextToken();
			}
			else
			{
				error("Expected ID\n");
			}
		}
		next_token = GetNextToken();
		if (strcmp(next_token.lx, ";") == 0)
		{
			;
		}
		else
		{
			error("Expected a ;\n");
		}
	}
	else
	{
		error("Expected static or field\n");
	}
}

ParserInfo memberDeclar()
{
	Token next_token = PeekNextToken();
	if (strcmp(next_token.lx, "static") == 0 || strcmp(next_token.lx, "field") == 0)
	{
		classVarDeclar();
	}
	else if (strcmp(next_token.lx, "constructor") == 0 || strcmp(next_token.lx, "function") == 0 || strcmp(next_token.lx, "method") == 0)
	{
		subroutineDeclar();
	}
	else
	{
		error("Expected class or subroutine declaration\n");
	}
}

ParserInfo classDeclar()
{
	Token next_token = GetNextToken();
	if (strcmp(next_token.lx, "class") == 0)
	{
		next_token = GetNextToken();
		if (next_token.tp == ID)
		{
			next_token = GetNextToken();
			if (strcmp(next_token.lx, "{") == 0)
			{
				memberDeclar();
				next_token = GetNextToken();
				if (strcmp(next_token.lx, "}") == 0)
				{
					error("Empty class\n");
				}
			}
			else
			{
				;
			}
		}
		else
		{
			error("id expected\n");
		}
	}
	else
	{
		error("class expected\n");
	}
}

int InitParser(char *file_name)
{
	InitLexer(file_name);
	StopLexer();
	return 1;
}

ParserInfo Parse()
{
	ParserInfo pi;
	pi.er = none;
	InitParser("Ball.jack");
	StopParser();
	pi = classDeclar();
	return pi;
}

int StopParser()
{
	return 1;
}

#ifndef TEST_PARSER
int main()
{
	Parse();
	return 1;
}
#endif
