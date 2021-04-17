#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "lexer.h"
#include "parser.h"
#include <stdlib.h>

// define your own types and function prototypes for the symbol table(s) module below

char current_class[32];

typedef struct
{
    char name[16];
    char type[16];
    char kind[16];
    int kind_index;
} Symbol;

typedef struct
{
    int index_static;
    int index_field;
    int index_arg;
    int index_var;
} Index;

typedef enum
{
    Static,
    Field,
    Arg,
    Var,
    Class,
    Function,
    Method,
    Constructor
} Kind;

struct Symbol_table
{
    Symbol *table[32];
    int table_size;
    Index index;
    struct Symbol_table *parent;
    struct Symbol_table* children[12];
    int num_children;
};

typedef struct Symbol_table Symbol_table;

int lookup_symbol_scope(Symbol_table *t, Symbol *symbol);

int lookup_symbol_global(Symbol_table *t, Symbol *symbol);

int lookup_symbol_files(Symbol *symbol);

int add_symbol(Symbol_table *t, Symbol *symbol);

Symbol_table *create_symbol_table();

Symbol_table *new_scope(Symbol_table *scope);

typedef struct
{
    Symbol_table *table[15];
    int table_size;
} All_symbol_tables;

All_symbol_tables all_symbol_tables;

Symbol_table *current_scope;

#endif
