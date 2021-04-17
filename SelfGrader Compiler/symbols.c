/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
The Symbol Tables Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name: Habib Chanchwelia
Student ID: 201329422
Email: sc19hc@leeds.ac.uk
Date Work Commenced: 1/04/2021
*************************************************************************/

#include <stdio.h>
#include <string.h>
#include "symbols.h"

Symbol_table *create_symbol_table()
{
    Symbol_table *t = malloc(sizeof(Symbol_table));

    t->table_size = 0;
    t->index.index_arg = 0;
    t->index.index_field = 0;
    t->index.index_static = 0;
    t->index.index_var = 0;
    t->num_children = 0;
    for (int i = 0; i < 32; i++)
    {
        t->table[i] = NULL;
    }
    for (int i = 0; i < 12; i++)
    {
        t->children[i] = NULL;
    }
    t->parent = NULL;
    return t;
}

Symbol_table *new_scope(Symbol_table *scope)
{
    Symbol_table *t = create_symbol_table();
    scope->children[scope->num_children++] = t;
    t->parent = scope;
    current_scope = t;
    return t;
}

int lookup_symbol_scope(Symbol_table *t, Symbol *symbol)
{
    for (int i = 0; i < t->table_size; i++)
    {
        if (!strcmp(t->table[i]->name, symbol->name))
            return 1;
    }
    return 0;
}

int lookup_symbol_global(Symbol_table *t, Symbol *symbol)
{
    for (int i = 0; i < t->table_size; i++)
    {
        if (!strcmp(t->table[i]->name, symbol->name))
            return 1;
    }
    if (t->parent == NULL)
        return 0;
    return lookup_symbol_global(t->parent, symbol);
}

int add_symbol(Symbol_table *t, Symbol *symbol)
{
    t->table[t->table_size++] = symbol;
    Symbol *s = t->table[t->table_size - 1];
/*
        printf("\nName: %s\n", s->name);
        printf("Type: %s\n", s->type);
        printf("Kind: %s\n", s->kind);
        printf("Index: %d\n", s->kind_index);
*/
}
