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

Symbol_table_stack stack;


void add_stack_table()
{
    Symbol_table new_table;

    new_table.table_size = 0;
    new_table.index.index_arg = 0;
    new_table.index.index_field = 0;
    new_table.index.index_static = 0;
    new_table.index.index_var = 0;
    stack.top_of_stack++;
    stack.stack[stack.top_of_stack] = new_table;
}

void remove_stack_table()
{
    stack.stack[stack.top_of_stack].table_size = 0;
    stack.stack[stack.top_of_stack].index.index_arg = 0;
    stack.stack[stack.top_of_stack].index.index_static = 0;
    stack.stack[stack.top_of_stack].index.index_field = 0;
    stack.stack[stack.top_of_stack].index.index_var = 0;
    stack.top_of_stack--;
}

int lookup_symbol_global(Symbol symbol)
{
    for (int i = stack.top_of_stack; i >= 0; i--)
    {
        for (int j = 0; j < stack.stack[i].table_size; j++)
        {
            if (!strcmp(stack.stack[i].table[j].name, symbol.name))
                return i;
        }
    }
    return -1;
}

int lookup_symbol_scope(Symbol symbol)
{
        for (int j = 0; j < stack.stack[stack.top_of_stack].table_size; j++)
        {
            if (!strcmp(stack.stack[stack.top_of_stack].table[j].name, symbol.name))
                return 1;
        }
    return 0;
}

int add_symbol(Symbol symbol)
{
    stack.stack[stack.top_of_stack].table[stack.stack[stack.top_of_stack].table_size] = symbol;
    stack.stack[stack.top_of_stack].table_size++;
    /*
    printf("\nName: %s\n", s.name);
    printf("Type: %s\n", s.type);
    printf("Kind: %s\n", s.kind);
    printf("Index: %d\n", s.kind_index);
    */
}

void wipe_stack()
{
    for (int i = stack.top_of_stack; i >= 0; i--)
    {
        remove_stack_table();
    }
}