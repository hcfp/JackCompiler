#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"
#include "symbols.h"

// you can declare prototypes of parser functions below

ParserInfo type();
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
}

ParserInfo type()
{
	ParserInfo parser_info;
	parser_info.er = none;
	Token next_token = GetNextToken();
	if (next_token.tp == ERR)
	{
		parser_info.er = lexerErr;
		parser_info.tk = next_token;
		return parser_info;
	}
	if (next_token.tp == RESWORD)
	{
		if (!strcmp(next_token.lx, "int") || !strcmp(next_token.lx, "char") || !strcmp(next_token.lx, "boolean"))
		{
			;
		}
		else
		{
			error("invalid type");
			parser_info.er = illegalType;
			parser_info.tk = next_token;
			return parser_info;
		}
	}
	else if (next_token.tp == ID)
	{
		;
	}
	else
	{
		error("Expected a type\n");
		parser_info.er = syntaxError;
		parser_info.tk = next_token;
		return parser_info;
	}
	return parser_info;
}

ParserInfo expressionList()
{
	ParserInfo parser_info;
	parser_info.er = none;
	Token next_token = PeekNextToken();
	if (next_token.tp == ERR)
	{
		parser_info.er = lexerErr;
		parser_info.tk = next_token;
		return parser_info;
	}
	if (!strcmp(next_token.lx, ")"))
	{
		;
	}
	else
	{
		parser_info = expression();
		if (parser_info.er != none)
		{
			return parser_info;
		}
		next_token = PeekNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		while (!strcmp(next_token.lx, ","))
		{
			GetNextToken();
			parser_info = expression();
			if (parser_info.er != none)
			{
				return parser_info;
			}
			next_token = PeekNextToken();
			if (next_token.tp == ERR)
			{
				parser_info.er = lexerErr;
				parser_info.tk = next_token;
				return parser_info;
			}
		}
	}
	return parser_info;
}

ParserInfo operand()
{
	ParserInfo parser_info;
	parser_info.er = none;

	Token next_token = GetNextToken();
	if (next_token.tp == ERR)
	{
		parser_info.er = lexerErr;
		parser_info.tk = next_token;
		return parser_info;
	}

	if (next_token.tp == INT || next_token.tp == ID || !strcmp(next_token.lx, "(") || next_token.tp == STRING || !strcmp(next_token.lx, "true") || !strcmp(next_token.lx, "false") || !strcmp(next_token.lx, "null") || !strcmp(next_token.lx, "this"))
	{
		if (next_token.tp == INT || next_token.tp == STRING || !strcmp(next_token.lx, "true") || !strcmp(next_token.lx, "false") || !strcmp(next_token.lx, "null") || !strcmp(next_token.lx, "this"))
		{
			;
		}
		else if (next_token.tp == ID)
		{
			//TODO Check if declared - global
			next_token = PeekNextToken();
			if (next_token.tp == ERR)
			{
				parser_info.er = lexerErr;
				parser_info.tk = next_token;
				return parser_info;
			}
			if (!strcmp(next_token.lx, "."))
			{
				GetNextToken();
				next_token = GetNextToken();
				if (next_token.tp == ERR)
				{
					parser_info.er = lexerErr;
					parser_info.tk = next_token;
					return parser_info;
				}
				if (next_token.tp == ID)
				{
					;
				}
				else
				{
					error("Expected an ID");
					parser_info.er = idExpected;
					parser_info.tk = next_token;
					return parser_info;
				}
			}
			next_token = PeekNextToken();
			if (next_token.tp == ERR)
			{
				parser_info.er = lexerErr;
				parser_info.tk = next_token;
				return parser_info;
			}
			if (!strcmp(next_token.lx, "["))
			{
				GetNextToken();
				parser_info = expression();
				if (parser_info.er != none)
				{
					return parser_info;
				}
				next_token = GetNextToken();
				if (next_token.tp == ERR)
				{
					parser_info.er = lexerErr;
					parser_info.tk = next_token;
					return parser_info;
				}
				if (!strcmp(next_token.lx, "]"))
				{
					;
				}
				else
				{
					error("Expected ]\n");
					parser_info.er = closeBracketExpected;
					parser_info.tk = next_token;
					return parser_info;
				}
			}
			else
			{
				if (!strcmp(next_token.lx, "("))
				{
					GetNextToken();
					parser_info = expressionList();
					if (parser_info.er != none)
					{
						return parser_info;
					}
					next_token = GetNextToken();
					if (next_token.tp == ERR)
					{
						parser_info.er = lexerErr;
						parser_info.tk = next_token;
						return parser_info;
					}
					if (!strcmp(next_token.lx, ")"))
					{
						;
					}
					else
					{
						error("Expected )\n");
						parser_info.er = closeParenExpected;
						parser_info.tk = next_token;
						return parser_info;
					}
				}
			}
		}
		else if (!strcmp(next_token.lx, "("))
		{
			//GetNextToken(); this line caused 10 hours of debugging
			parser_info = expression();
			if (parser_info.er != none)
			{
				return parser_info;
			}
			next_token = GetNextToken();
			if (next_token.tp == ERR)
			{
				parser_info.er = lexerErr;
				parser_info.tk = next_token;
				return parser_info;
			}
			if (!strcmp(next_token.lx, ")"))
			{
				;
			}
			else
			{
				error("expected )\n");
				parser_info.er = closeParenExpected;
				parser_info.tk = next_token;
				return parser_info;
			}
		}
	}
	else
	{
		error("Expected an operand\n");
		parser_info.er = syntaxError;
		parser_info.tk = next_token;
		return parser_info;
	}
	return parser_info;
}

ParserInfo factor()
{
	ParserInfo parser_info;
	parser_info.er = none;
	Token next_token = PeekNextToken();
	if (next_token.tp == ERR)
	{
		parser_info.er = lexerErr;
		parser_info.tk = next_token;
		return parser_info;
	}

	if ((!strcmp(next_token.lx, "-") || !strcmp(next_token.lx, "~")) && next_token.tp == SYMBOL)
	{
		GetNextToken();
		parser_info = operand();
		if (parser_info.er != none)
		{
			return parser_info;
		}
	}
	else
	{
		parser_info = operand();
		if (parser_info.er != none)
		{
			return parser_info;
		}
	}
	return parser_info;
}

ParserInfo term()
{
	ParserInfo parser_info;
	parser_info.er = none;
	parser_info = factor();
	if (parser_info.er != none)
	{
		return parser_info;
	}
	Token next_token = PeekNextToken();
	if (next_token.tp == ERR)
	{
		parser_info.er = lexerErr;
		parser_info.tk = next_token;
		return parser_info;
	}
	if (next_token.tp == SYMBOL)
	{
		while (!strcmp(next_token.lx, "*") || !strcmp(next_token.lx, "/"))
		{
			GetNextToken();
			parser_info = factor();
			if (parser_info.er != none)
			{
				return parser_info;
			}
			next_token = PeekNextToken();
			if (next_token.tp == ERR)
			{
				parser_info.er = lexerErr;
				parser_info.tk = next_token;
				return parser_info;
			}
		}
	}
	return parser_info;
}

ParserInfo arithmeticExpression()
{
	ParserInfo parser_info;
	parser_info.er = none;
	parser_info = term();
	if (parser_info.er != none)
	{
		return parser_info;
	}

	Token next_token = PeekNextToken();
	if (next_token.tp == ERR)
	{
		parser_info.er = lexerErr;
		parser_info.tk = next_token;
		return parser_info;
	}
	if (next_token.tp == SYMBOL)
	{
		while (!strcmp(next_token.lx, "+") || !strcmp(next_token.lx, "-"))
		{
			GetNextToken();
			parser_info = term();
			if (parser_info.er != none)
			{
				return parser_info;
			}
			next_token = PeekNextToken();
			if (next_token.tp == ERR)
			{
				parser_info.er = lexerErr;
				parser_info.tk = next_token;
				return parser_info;
			}
		}
	}
	return parser_info;
}

ParserInfo relationalExpression()
{
	ParserInfo parser_info;
	parser_info.er = none;
	parser_info = arithmeticExpression();
	if (parser_info.er != none)
	{
		return parser_info;
	}
	Token next_token = PeekNextToken();
	if (next_token.tp == ERR)
	{
		parser_info.er = lexerErr;
		parser_info.tk = next_token;
		return parser_info;
	}
	if (next_token.tp == SYMBOL)
	{
		while (!strcmp(next_token.lx, "=") || !strcmp(next_token.lx, ">") || !strcmp(next_token.lx, "<"))
		{
			GetNextToken();
			parser_info = arithmeticExpression();
			if (parser_info.er != none)
			{
				return parser_info;
			}
			next_token = PeekNextToken();
			if (next_token.tp == ERR)
			{
				parser_info.er = lexerErr;
				parser_info.tk = next_token;
				return parser_info;
			}
		}
	}
	return parser_info;
}

ParserInfo expression()
{
	ParserInfo parser_info;
	parser_info.er = none;
	parser_info = relationalExpression();
	if (parser_info.er != none)
	{
		return parser_info;
	}
	Token next_token = PeekNextToken();
	if (next_token.tp == ERR)
	{
		parser_info.er = lexerErr;
		parser_info.tk = next_token;
		return parser_info;
	}
	if (next_token.tp == SYMBOL)
	{
		while (!strcmp(next_token.lx, "&") || !strcmp(next_token.lx, "|"))
		{
			GetNextToken();
			parser_info = relationalExpression();
			if (parser_info.er != none)
			{
				return parser_info;
			}
			next_token = PeekNextToken();
			if (next_token.tp == ERR)
			{
				parser_info.er = lexerErr;
				parser_info.tk = next_token;
				return parser_info;
			}
		}
	}
	return parser_info;
}

ParserInfo varDeclarStatement()
{
	ParserInfo parser_info;
	parser_info.er = none;
	Token next_token = GetNextToken();
	if (next_token.tp == ERR)
	{
		parser_info.er = lexerErr;
		parser_info.tk = next_token;
		return parser_info;
	}
	if (!strcmp(next_token.lx, "var"))
	{
		Symbol *symbol = (Symbol *)malloc(sizeof(Symbol));
		strcpy(symbol->kind, next_token.lx);
		char kind[16];
		strcpy(kind, next_token.lx);
		Token symbol_token = PeekNextToken();
		char tp[16];
		strcpy(tp, symbol_token.lx);
		parser_info = type();
		strcpy(symbol->type, symbol_token.lx);
		if (parser_info.er != none)
		{
			return parser_info;
		}
		next_token = GetNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		if (next_token.tp == ID)
		{
			//TODO check if already declared - in scope
			strcpy(symbol->name, next_token.lx);
			symbol->kind_index = current_scope->index.index_var++;
			add_symbol(current_scope, symbol);
		}
		else
		{
			error("Expected identifier");
			parser_info.er = idExpected;
			parser_info.tk = next_token;
			return parser_info;
		}
		next_token = PeekNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		while (next_token.tp == SYMBOL && !strcmp(next_token.lx, ","))
		{
			Symbol *symbol = (Symbol *)malloc(sizeof(Symbol));
			strcpy(symbol->kind, kind);
			strcpy(symbol->type, tp);
			GetNextToken();
			next_token = GetNextToken();
			if (next_token.tp == ERR)
			{
				parser_info.er = lexerErr;
				parser_info.tk = next_token;
				return parser_info;
			}
			if (next_token.tp == ID)
			{
				//TODO check if already declared - in scope
				strcpy(symbol->name, next_token.lx);
				symbol->kind_index = current_scope->index.index_var++;
				add_symbol(current_scope, symbol);
				next_token = PeekNextToken();
				if (next_token.tp == ERR)
				{
					parser_info.er = lexerErr;
					parser_info.tk = next_token;
					return parser_info;
				}
			}
			else
			{
				error("Expected ID\n");
				parser_info.er = idExpected;
				parser_info.tk = next_token;
				return parser_info;
			}
		}
		next_token = GetNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		if (!strcmp(next_token.lx, ";"))
		{
			;
		}
		else
		{
			error("Expected ;\n");
			parser_info.er = semicolonExpected;
			parser_info.tk = next_token;
			return parser_info;
		}
	}
	else
	{
		error("Expected var\n");
		parser_info.er = syntaxError;
		parser_info.tk = next_token;
		return parser_info;
	}
	return parser_info;
}

ParserInfo letStatement()
{
	ParserInfo parser_info;
	parser_info.er = none;
	Token next_token = GetNextToken();
	if (next_token.tp == ERR)
	{
		parser_info.er = lexerErr;
		parser_info.tk = next_token;
		return parser_info;
	}
	if (!strcmp(next_token.lx, "let"))
	{
		next_token = GetNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		if (next_token.tp == ID)
		{
			//TODO: Check ID is declared here - class scope
			next_token = PeekNextToken();
			if (next_token.tp == ERR)
			{
				parser_info.er = lexerErr;
				parser_info.tk = next_token;
				return parser_info;
			}
			if (!strcmp(next_token.lx, "["))
			{
				GetNextToken();
				parser_info = expression();
				if (parser_info.er != none)
				{
					return parser_info;
				}
				next_token = GetNextToken();
				if (next_token.tp == ERR)
				{
					parser_info.er = lexerErr;
					parser_info.tk = next_token;
					return parser_info;
				}
				if (!strcmp(next_token.lx, "]"))
				{
					;
				}
				else
				{
					error("Expected ]\n");
					parser_info.er = closeBracketExpected;
					parser_info.tk = next_token;
					return parser_info;
				}
			}
			next_token = GetNextToken();
			if (next_token.tp == ERR)
			{
				parser_info.er = lexerErr;
				parser_info.tk = next_token;
				return parser_info;
			}
			if (!strcmp(next_token.lx, "="))
			{
				parser_info = expression();
				if (parser_info.er != none)
				{
					return parser_info;
				}
			}
			else
			{
				error("Expected =\n");
				parser_info.er = equalExpected;
				parser_info.tk = next_token;
				return parser_info;
			}
			next_token = GetNextToken();
			if (next_token.tp == ERR)
			{
				parser_info.er = lexerErr;
				parser_info.tk = next_token;
				return parser_info;
			}
			if (!strcmp(next_token.lx, ";"))
			{
				;
			}
			else
			{
				error("Expected ;\n");
				parser_info.er = semicolonExpected;
				parser_info.tk = next_token;
				return parser_info;
			}
		}
		else
		{
			error("Expected ID\n");
			parser_info.er = idExpected;
			parser_info.tk = next_token;
			return parser_info;
		}
	}
	else
	{
		error("Let expected\n");
		parser_info.er = syntaxError;
		parser_info.tk = next_token;
		return parser_info;
	}
	return parser_info;
}

ParserInfo ifStatement()
{
	ParserInfo parser_info;
	parser_info.er = none;
	Token next_token = GetNextToken();
	if (next_token.tp == ERR)
	{
		parser_info.er = lexerErr;
		parser_info.tk = next_token;
		return parser_info;
	}
	if (!strcmp(next_token.lx, "if"))
	{
		next_token = GetNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		if (!strcmp(next_token.lx, "("))
		{
			parser_info = expression();
			if (parser_info.er != none)
			{
				return parser_info;
			}
		}
		else
		{
			error("Expected (");
			parser_info.er = openParenExpected;
			parser_info.tk = next_token;
			return parser_info;
		}
		next_token = GetNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		if (!strcmp(next_token.lx, ")"))
		{
			;
		}
		else
		{
			error("Expected )\n");
			parser_info.er = closeParenExpected;
			parser_info.tk = next_token;
			return parser_info;
		}
		next_token = PeekNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		if (!strcmp(next_token.lx, "{"))
		{
			current_scope = new_scope(current_scope);
			parser_info = subroutineBody();
			if (parser_info.er != none)
			{
				return parser_info;
			}
		}
		next_token = PeekNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		if (!strcmp(next_token.lx, "else"))
		{
			GetNextToken();
			next_token = PeekNextToken();
			if (next_token.tp == ERR)
			{
				parser_info.er = lexerErr;
				parser_info.tk = next_token;
				return parser_info;
			}
			if (!strcmp(next_token.lx, "{"))
			{
				current_scope = new_scope(current_scope);
				parser_info = subroutineBody();
				if (parser_info.er != none)
				{
					return parser_info;
				}
			}
			else
			{
				error("Expected {\n");
				parser_info.er = openBraceExpected;
				parser_info.tk = next_token;
				return parser_info;
			}
		}
	}
	return parser_info;
}

ParserInfo whileStatement()
{
	ParserInfo parser_info;
	parser_info.er = none;
	Token next_token = GetNextToken();
	if (next_token.tp == ERR)
	{
		parser_info.er = lexerErr;
		parser_info.tk = next_token;
		return parser_info;
	}
	if (!strcmp(next_token.lx, "while"))
	{
		next_token = GetNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		if (!strcmp(next_token.lx, "("))
		{
			parser_info = expression();
			if (parser_info.er != none)
			{
				return parser_info;
			}
		}
		else
		{
			error("Expected a (\n");
			parser_info.er = openParenExpected;
			parser_info.tk = next_token;
			return parser_info;
		}
		next_token = GetNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		if (!strcmp(next_token.lx, ")"))
		{
			;
		}
		else
		{
			error("Expected )\n");
			parser_info.er = closeParenExpected;
			parser_info.tk = next_token;
			return parser_info;
		}
		next_token = PeekNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		if (!strcmp(next_token.lx, "{"))
		{
			current_scope = new_scope(current_scope);
			parser_info = subroutineBody();
			if (parser_info.er != none)
			{
				return parser_info;
			}
		}
		else
		{
			error("Expected a {\n");
			parser_info.er = openBraceExpected;
			parser_info.tk = next_token;
			return parser_info;
		}
	}
	return parser_info;
}

ParserInfo subroutineCall()
{
	ParserInfo parser_info;
	parser_info.er = none;
	Token next_token = GetNextToken();
	if (next_token.tp == ERR)
	{
		parser_info.er = lexerErr;
		parser_info.tk = next_token;
		return parser_info;
	}
	if (next_token.tp == ID)
	{
		//TODO check subroutine is declared - global
		next_token = PeekNextToken();
		if (!strcmp(next_token.lx, "."))
		{
			GetNextToken();
			next_token = GetNextToken();
			if (next_token.tp == ERR)
			{
				parser_info.er = lexerErr;
				parser_info.tk = next_token;
				return parser_info;
			}
			if (next_token.tp == ID)
			{
				//TODO check if declared - global
				;
			}
			else
			{
				error("Expected an ID");
				parser_info.er = idExpected;
				parser_info.tk = next_token;
				return parser_info;
			}
		}
		next_token = GetNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		if (!strcmp(next_token.lx, "("))
		{
			parser_info = expressionList();
			if (parser_info.er != none)
			{
				return parser_info;
			}
		}
		else
		{
			error("Expected (\n");
			parser_info.er = openParenExpected;
			parser_info.tk = next_token;
			return parser_info;
		}
		next_token = GetNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		if (!strcmp(next_token.lx, ")"))
		{
			;
		}
		else
		{
			error("Expected )\n");
			parser_info.er = closeParenExpected;
			parser_info.tk = next_token;
			return parser_info;
		}
	}
	else
	{
		error("Expected an identifier\n");
		parser_info.er = idExpected;
		parser_info.tk = next_token;
		return parser_info;
	}
	return parser_info;
}

ParserInfo doStatement()
{
	ParserInfo parser_info;
	parser_info.er = none;
	Token next_token = GetNextToken();
	if (next_token.tp == ERR)
	{
		parser_info.er = lexerErr;
		parser_info.tk = next_token;
		return parser_info;
	}
	if (!strcmp(next_token.lx, "do"))
	{
		parser_info = subroutineCall();
		if (parser_info.er != none)
		{
			return parser_info;
		}
		next_token = GetNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		if (!strcmp(next_token.lx, ";"))
		{
			;
		}
		else
		{
			error("Expected ;\n");
			parser_info.er = semicolonExpected;
			parser_info.tk = next_token;
			return parser_info;
		}
	}
	return parser_info;
}

ParserInfo returnStatement()
{
	ParserInfo parser_info;
	parser_info.er = none;
	Token next_token = GetNextToken();
	if (next_token.tp == ERR)
	{
		parser_info.er = lexerErr;
		parser_info.tk = next_token;
		return parser_info;
	}
	if (!strcmp(next_token.lx, "return"))
	{
		next_token = PeekNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		if (next_token.tp == INT || next_token.tp == ID || !strcmp(next_token.lx, "(") || next_token.tp == STRING || !strcmp(next_token.lx, "true") || !strcmp(next_token.lx, "false") || !strcmp(next_token.lx, "null") || !strcmp(next_token.lx, "this") || !strcmp(next_token.lx, "-") || !strcmp(next_token.lx, "~"))
		{
			parser_info = expression();
			if (parser_info.er != none)
			{
				return parser_info;
			}
			next_token = GetNextToken();
			if (next_token.tp == ERR)
			{
				parser_info.er = lexerErr;
				parser_info.tk = next_token;
				return parser_info;
			}
			if (!strcmp(next_token.lx, ";"))
			{
				;
			}
			else
			{
				error("Expected ;\n");
				parser_info.er = semicolonExpected;
				parser_info.tk = next_token;
				return parser_info;
			}
		}
		else
		{
			next_token = GetNextToken();
			if (next_token.tp == ERR)
			{
				parser_info.er = lexerErr;
				parser_info.tk = next_token;
				return parser_info;
			}
			if (!strcmp(next_token.lx, ";"))
			{
				;
			}
			else
			{
				error("Expected ;\n");
				parser_info.er = semicolonExpected;
				parser_info.tk = next_token;
				return parser_info;
			}
		}
	}
	return parser_info;
}

ParserInfo subroutineBody()
{
	ParserInfo parser_info;
	parser_info.er = none;
	Token next_token = GetNextToken();
	if (next_token.tp == ERR)
	{
		parser_info.er = lexerErr;
		parser_info.tk = next_token;
		return parser_info;
	}
	if (!strcmp(next_token.lx, "{"))
	{
		next_token = PeekNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		while (!strcmp(next_token.lx, "var") || !strcmp(next_token.lx, "let") || !strcmp(next_token.lx, "if") || !strcmp(next_token.lx, "while") || !strcmp(next_token.lx, "do") || !strcmp(next_token.lx, "return"))
		{
			if (next_token.tp == ERR)
			{
				parser_info.er = lexerErr;
				parser_info.tk = next_token;
				return parser_info;
			}
			parser_info = statement();
			if (parser_info.er != none)
			{
				return parser_info;
			}
			next_token = PeekNextToken();
			if (next_token.tp == ERR)
			{
				parser_info.er = lexerErr;
				parser_info.tk = next_token;
				return parser_info;
			}
		}
		next_token = GetNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		if (!strcmp(next_token.lx, "}"))
		{
			if (current_scope->parent != NULL)
				current_scope = current_scope->parent;
		}
		else
		{
			error("Expected }");
			parser_info.er = closeBraceExpected;
			parser_info.tk = next_token;
			return parser_info;
		}
	}
	else
	{
		error("Expected {\n");
		parser_info.er = openBraceExpected;
		parser_info.tk = next_token;
		return parser_info;
	}
	return parser_info;
}

ParserInfo statement()
{
	ParserInfo parser_info;
	parser_info.er = none;
	Token next_token = PeekNextToken();
	if (next_token.tp == ERR)
	{
		parser_info.er = lexerErr;
		parser_info.tk = next_token;
		return parser_info;
	}
	if (!strcmp(next_token.lx, "var"))
	{
		parser_info = varDeclarStatement();
		if (parser_info.er != none)
		{
			return parser_info;
		}
	}
	else if (!strcmp(next_token.lx, "let"))
	{
		parser_info = letStatement();
		if (parser_info.er != none)
		{
			return parser_info;
		}
	}
	else if (!strcmp(next_token.lx, "if"))
	{
		parser_info = ifStatement();
		if (parser_info.er != none)
		{
			return parser_info;
		}
	}
	else if (!strcmp(next_token.lx, "while"))
	{
		parser_info = whileStatement();
		if (parser_info.er != none)
		{
			return parser_info;
		}
	}
	else if (!strcmp(next_token.lx, "do"))
	{
		parser_info = doStatement();
		if (parser_info.er != none)
		{
			return parser_info;
		}
	}
	else if (!strcmp(next_token.lx, "return"))
	{
		parser_info = returnStatement();
		if (parser_info.er != none)
		{
			return parser_info;
		}
	}
	return parser_info;
}

ParserInfo paramList()
{
	ParserInfo parser_info;
	parser_info.er = none;
	Token next_token = PeekNextToken();
	if (next_token.tp == ERR)
	{
		parser_info.er = lexerErr;
		parser_info.tk = next_token;
		return parser_info;
	}
	if (!(!strcmp(next_token.lx, "int") || !strcmp(next_token.lx, "char") || !strcmp(next_token.lx, "boolean") || next_token.tp == ID))
	{
		;
	}
	else
	{
		Symbol *symbol = (Symbol *)malloc(sizeof(Symbol));
		parser_info = type();
		strcpy(symbol->type, next_token.lx);
		strcpy(symbol->kind, "arg");

		if (parser_info.er != none)
		{
			return parser_info;
		}
		next_token = GetNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		if (next_token.tp == ID)
		{
			strcpy(symbol->name, next_token.lx);
			symbol->kind_index = current_scope->index.index_arg++;
			add_symbol(current_scope, symbol);
		}
		else
		{
			error("Expected identifier");
			parser_info.er = idExpected;
			parser_info.tk = next_token;
			return parser_info;
		}
		next_token = PeekNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		while (next_token.tp == SYMBOL && !strcmp(next_token.lx, ","))
		{
			GetNextToken();
			Symbol *symbol = (Symbol *)malloc(sizeof(Symbol));
			Token symbol_token;
			symbol_token = PeekNextToken();
			parser_info = type();
			strcpy(symbol->type, symbol_token.lx);
			strcpy(symbol->kind, "arg");
			if (parser_info.er != none)
			{
				return parser_info;
			}
			next_token = GetNextToken();
			if (next_token.tp == ERR)
			{
				parser_info.er = lexerErr;
				parser_info.tk = next_token;
				return parser_info;
			}
			if (next_token.tp == ID)
			{
				strcpy(symbol->name, next_token.lx);
				symbol->kind_index = current_scope->index.index_arg++;
				add_symbol(current_scope, symbol);
				next_token = PeekNextToken();
				if (next_token.tp == ERR)
				{
					parser_info.er = lexerErr;
					parser_info.tk = next_token;
					return parser_info;
				}
			}
			else
			{
				error("Expected ID\n");
				parser_info.er = idExpected;
				parser_info.tk = next_token;
				return parser_info;
			}
		}
	}
	return parser_info;
}

ParserInfo subroutineDeclar()
{
	ParserInfo parser_info;
	parser_info.er = none;
	Token next_token = GetNextToken();
	if (next_token.tp == ERR)
	{
		parser_info.er = lexerErr;
		parser_info.tk = next_token;
		return parser_info;
	}
	if (!strcmp(next_token.lx, "constructor") || !strcmp(next_token.lx, "function") || !strcmp(next_token.lx, "method"))
	{
		int is_method = 0;
		Symbol *subroutine_symbol = (Symbol *)malloc(sizeof(Symbol));
		if (!strcmp(next_token.lx, "constructor"))
			strcpy(subroutine_symbol->kind, "constructor");
		else if (!strcmp(next_token.lx, "function"))
			strcpy(subroutine_symbol->kind, "function");
		else if (!strcmp(next_token.lx, "method"))
		{
			strcpy(subroutine_symbol->kind, "method");
			is_method = 1;
		}
		subroutine_symbol->kind_index = 0;
		next_token = PeekNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		if (!strcmp(next_token.lx, "void"))
		{
			strcpy(subroutine_symbol->type, "void");
			GetNextToken();
		}
		else
		{
			parser_info = type();
			strcpy(subroutine_symbol->type, next_token.lx);
			if (parser_info.er != none)
			{
				return parser_info;
			}
		}
		next_token = GetNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		if (next_token.tp == ID)
		{
			strcpy(subroutine_symbol->name, next_token.lx);
			add_symbol(current_scope, subroutine_symbol);
			next_token = GetNextToken();
			if (next_token.tp == ERR)
			{
				parser_info.er = lexerErr;
				parser_info.tk = next_token;
				return parser_info;
			}
			if (!strcmp(next_token.lx, "("))
			{
				if (is_method)
				{
					current_scope = new_scope(current_scope);
					Symbol *symbol = (Symbol *)malloc(sizeof(Symbol));
					strcpy(symbol->name, "this");
					strcpy(symbol->type, current_class);
					strcpy(symbol->kind, "arg");
					symbol->kind_index = symbol->kind_index = current_scope->index.index_arg++;
					add_symbol(current_scope, symbol);
				}
				else
				{
					current_scope = new_scope(current_scope);
				}
				parser_info = paramList();
				if (parser_info.er != none)
				{
					return parser_info;
				}
				next_token = GetNextToken();
				if (next_token.tp == ERR)
				{
					parser_info.er = lexerErr;
					parser_info.tk = next_token;
					return parser_info;
				}
				if (!strcmp(next_token.lx, ")"))
				{
					parser_info = subroutineBody();
					if (parser_info.er != none)
					{
						return parser_info;
					}
				}
				else
				{
					error("Expected )\n");
					parser_info.er = closeParenExpected;
					parser_info.tk = next_token;
					return parser_info;
				}
			}
			else
			{
				error("Expected (\n");
				parser_info.er = openParenExpected;
				parser_info.tk = next_token;
				return parser_info;
			}
		}
		else
		{
			error("Expected ID\n");
			parser_info.er = idExpected;
			parser_info.tk = next_token;
			return parser_info;
		}
	}
	return parser_info;
}

ParserInfo classVarDeclar()
{
	ParserInfo parser_info;
	parser_info.er = none;
	Token next_token = GetNextToken();
	if (next_token.tp == ERR)
	{
		parser_info.er = lexerErr;
		parser_info.tk = next_token;
		return parser_info;
	}
	if (!strcmp(next_token.lx, "static") || !strcmp(next_token.lx, "field"))
	{
		int static_field;
		if (!strcmp(next_token.lx, "static"))
			static_field = 0;
		else
			static_field = 1;

		Symbol *symbol = (Symbol *)malloc(sizeof(Symbol));
		strcpy(symbol->kind, next_token.lx);
		char kind[16];
		strcpy(kind, next_token.lx);
		Token symbol_token = PeekNextToken();
		parser_info = type();
		strcpy(symbol->type, symbol_token.lx);
		char tp[16];
		strcpy(tp, symbol->type);
		if (parser_info.er != none)
		{
			return parser_info;
		}
		next_token = GetNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		if (next_token.tp == ID)
		{
			strcpy(symbol->name, next_token.lx);
			if (static_field == 0)
			{
				symbol->kind_index = current_scope->index.index_static++;
			}
			else
			{
				symbol->kind_index = current_scope->index.index_field++;
			}
			add_symbol(current_scope, symbol);
		}

		else
		{
			error("Expected identifier");
			parser_info.er = idExpected;
			parser_info.tk = next_token;
			return parser_info;
		}
		next_token = PeekNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		while (next_token.tp == SYMBOL && !strcmp(next_token.lx, ","))
		{
			Symbol *symbol = (Symbol *)malloc(sizeof(Symbol));
			strcpy(symbol->kind, kind);
			strcpy(symbol->type, tp);
			GetNextToken();
			next_token = GetNextToken();
			if (next_token.tp == ERR)
			{
				parser_info.er = lexerErr;
				parser_info.tk = next_token;
				return parser_info;
			}
			if (next_token.tp == ID)
			{
				strcpy(symbol->name, next_token.lx);
				if (static_field == 0)
				{
					symbol->kind_index = current_scope->index.index_static++;
				}
				else
				{
					symbol->kind_index = current_scope->index.index_field++;
				}
				add_symbol(current_scope, symbol);
				next_token = PeekNextToken();
				if (next_token.tp == ERR)
				{
					parser_info.er = lexerErr;
					parser_info.tk = next_token;
					return parser_info;
				}
			}
			else
			{
				error("Expected ID\n");
				parser_info.er = idExpected;
				parser_info.tk = next_token;
				return parser_info;
			}
		}
		next_token = GetNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		if (!strcmp(next_token.lx, ";"))
		{
			;
		}
		else
		{
			error("Expected a ;\n");
			parser_info.er = semicolonExpected;
			parser_info.tk = next_token;
			return parser_info;
		}
	}
	else
	{
		error("Expected static or field\n");
		parser_info.er = classVarErr;
		parser_info.tk = next_token;
		return parser_info;
	}
	return parser_info;
}

ParserInfo memberDeclar()
{
	ParserInfo parser_info;
	parser_info.er = none;
	Token next_token = PeekNextToken();
	if (next_token.tp == ERR)
	{
		parser_info.er = lexerErr;
		parser_info.tk = next_token;
		return parser_info;
	}
	if (!strcmp(next_token.lx, "static") || !strcmp(next_token.lx, "field"))
	{
		parser_info = classVarDeclar();
		if (parser_info.er != none)
		{
			return parser_info;
		}
	}
	else if (!strcmp(next_token.lx, "constructor") || !strcmp(next_token.lx, "function") || !strcmp(next_token.lx, "method"))
	{
		parser_info = subroutineDeclar();
		if (parser_info.er != none)
		{
			return parser_info;
		}
	}
	else
	{
		error("Expected class or subroutine declaration\n");
		parser_info.er = memberDeclarErr;
		parser_info.tk = next_token;
		return parser_info;
	}
	return parser_info;
}

ParserInfo classDeclar()
{
	ParserInfo parser_info;
	parser_info.er = none;
	Token next_token = GetNextToken();
	if (next_token.tp == ERR)
	{
		parser_info.er = lexerErr;
		parser_info.tk = next_token;
		return parser_info;
	}
	if (!strcmp(next_token.lx, "class"))
	{
		Symbol_table *class_scope = create_symbol_table();
		current_scope = class_scope;
		all_symbol_tables.table[all_symbol_tables.table_size++] = current_scope;
		next_token = GetNextToken();
		if (next_token.tp == ERR)
		{
			parser_info.er = lexerErr;
			parser_info.tk = next_token;
			return parser_info;
		}
		if (next_token.tp == ID)
		{
			strcpy(current_class, next_token.lx);
			Symbol *symbol = (Symbol *)malloc(sizeof(Symbol));
			strcpy(symbol->name, current_class);
			strcpy(symbol->kind, "class");
			add_symbol(current_scope, symbol);
			next_token = GetNextToken();
			if (next_token.tp == ERR)
			{
				parser_info.er = lexerErr;
				parser_info.tk = next_token;
				return parser_info;
			}
			if (!strcmp(next_token.lx, "{"))
			{
				next_token = PeekNextToken();
				while (!strcmp(next_token.lx, "static") || !strcmp(next_token.lx, "field") || !strcmp(next_token.lx, "constructor") || !strcmp(next_token.lx, "function") || !strcmp(next_token.lx, "method"))
				{
					parser_info = memberDeclar();
					if (parser_info.er != none)
					{
						return parser_info;
					}
					next_token = PeekNextToken();
				}
				next_token = GetNextToken();
				if (next_token.tp == ERR)
				{
					parser_info.er = lexerErr;
					parser_info.tk = next_token;
					return parser_info;
				}
				if (!strcmp(next_token.lx, "}"))
				{
					;
				}
				else
				{
					error("expected }");
					parser_info.er = closeBraceExpected;
					parser_info.tk = next_token;
					return parser_info;
				}
			}
			else
			{
				error("Expected {\n");
				parser_info.er = openBraceExpected;
				parser_info.tk = next_token;
				return parser_info;
			}
		}
		else
		{
			error("id expected\n");
			parser_info.er = idExpected;
			parser_info.tk = next_token;
			return parser_info;
		}
	}
	else
	{
		error("class expected\n");
		parser_info.er = classExpected;
		parser_info.tk = next_token;
		return parser_info;
	}
	return parser_info;
}

int InitParser(char *file_name)
{
	int r = InitLexer(file_name);
	if (r == 0)
	{
		return 0;
	}
	return 1;
}

ParserInfo Parse()
{
	ParserInfo pi;
	pi.er = none;
	pi = classDeclar();
	StopParser();
	return pi;
}

int StopParser()
{
	StopLexer();
	return 1;
}

#ifndef TEST_PARSER
int main()
{
	Parse();
	return 1;
}
#endif
