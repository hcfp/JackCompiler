/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
The Compiler Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name: Habib Chanchwelia
Student ID: 201329422
Email:	sc19hc@leeds.ac.uk
Date Work Commenced: 1/04/2021
*************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "compiler.h"
#include "parser.h"
#include "symbols.h"

int pass;
int parse_num;
int InitCompiler()
{
	all_symbol_tables.table_size = 0;
}

ParserInfo compile(char *dir_name)
{
	ParserInfo p;
	p.er = none;
	char cmd[128] = "ls *.jack > libs.txt; cd ";
	strcat(cmd, dir_name);
	strcat(cmd, "; ls *.jack > files.txt");
	system(cmd);

	char files_path[32] = "";
	strcat(files_path, dir_name);
	strcat(files_path, "/files.txt");

	FILE *fptr = NULL;
	int i = 0;
	pass = 0;
	char lib_files[8][16];
	fptr = fopen("libs.txt", "r");
	while (fgets(lib_files[i], 32, fptr))
	{
		lib_files[i][strlen(lib_files[i]) - 1] = '\0';
		FILE *vm_ptr = NULL;
		char vm_name[16];
		int j = 0;
		while (lib_files[i][j] != '.' && j < strlen(lib_files[i]))
		{
			vm_name[j] = lib_files[i][j];
			j++;
		}
		int len = j;
		vm_name[j] = '\0';
		strcat(vm_name, ".vm");
		vm_ptr = fopen(vm_name, "w");
		fclose(vm_ptr);
		strcpy(vm_name, "");
		i++;
	}
	fclose(fptr);

	for (i = 0; i < 8; i++)
	{
		InitParser(lib_files[i]);
		p = Parse();
	}

	i = 0;
	char jack_files[32][32];
	fptr = fopen(files_path, "r");
	while (fgets(jack_files[i], 32, fptr))
	{
		jack_files[i][strlen(jack_files[i]) - 1] = '\0';
		FILE *vm_ptr = NULL;
		char vm_name[16];
		strcpy(vm_name, dir_name);
		strcat(vm_name, "/");
		vm_name[strlen(vm_name)] = '\0';
		int j = 0;
		int k = strlen(vm_name);
		while (jack_files[i][j] != '.' && j < strlen(jack_files[i]))
		{
			vm_name[k] = jack_files[i][j];
			j++;
			k++;
		}
		int len = k;
		vm_name[len] = '\0';
		strcat(vm_name, ".vm");
		printf("%s\n",vm_name);
		vm_ptr = fopen(vm_name, "w");
		fclose(vm_ptr);
		strcpy(vm_name, "");
		i++;
	}
	fclose(fptr);

	int num_files = i;
	printf("Compiling: ");
	for (i = 0; i < num_files; i++)
		printf("%s ", jack_files[i]);
	printf("\n");

	char path[32] = "";
	strcpy(path, dir_name);

	for (i = 0; i < num_files; i++)
	{
		strcat(path, "/");
		strcat(path, jack_files[i]);
		InitParser(path);
		p = Parse();
		strcpy(path, dir_name);
	}
	if (p.er != none)
	{
		return p;
	}

	pass = 1;
	for (parse_num = 0; parse_num < num_files; parse_num++)
	{
		strcat(path, "/");
		strcat(path, jack_files[parse_num]);
		InitParser(path);
		p = Parse();
		strcpy(path, dir_name);
	}
	return p;
}

int StopCompiler()
{
	StopParser();
	return 1;
}

#ifndef TEST_COMPILER
int main_compiler()
{
	InitCompiler();
	ParserInfo p = compile("Pong");
	//PrintError(p);
	StopCompiler();
	return 1;
}
#endif