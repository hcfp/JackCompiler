#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "lexer.h"
#include "parser.h"

// define your own types and function prototypes for the symbol table(s) module below

char current_class[32];

typedef struct
{
    char name[128];
    char type[16];
    char kind[16];
    int kind_index;
} Symbol;

typedef struct {
    int index_static;
    int index_field;
    int index_arg;
    int index_var;
} Index;

typedef struct
{
    Symbol table[128];
    int table_size;
    Index index;
} Symbol_table;

typedef struct
{
    Symbol_table stack[128];
    int top_of_stack;
} Symbol_table_stack;

Symbol_table_stack stack;

int lookup_symbol_scope(Symbol symbol);

int lookup_symbol_global(Symbol symbol);

int add_symbol(Symbol symbol);

void add_stack_table();

void remove_stack_table();

void wipe_stack();

#endif
