#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "lexer.h"
#include "parser.h"

// define your own types and function prototypes for the symbol table(s) module below

typedef struct
{
    char name[128];
    char type[16];
    char kind[16];
    int number_of_kind;
} Symbol;

typedef struct
{
    Symbol table[128];
    int table_size;
} Symbol_table;

typedef struct
{
    Symbol_table stack[128];
    int num_tables;
} Symbol_table_stack;

int lookup_symbol(Symbol symbol);

int add_symbol(Symbol symbol);

#endif
