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

int InitCompiler()
{
}

ParserInfo compile(char *dir_name)
{
	ParserInfo p;
	char cmd[128] = "cd ";
	// write your code below
	strcat(cmd, dir_name);
	strcat(cmd, "; ls *.jack > files.txt");
	system(cmd);

	char files_path[128] = "";
	strcat(files_path, dir_name);
	strcat(files_path, "/files.txt");

	FILE *fptr = NULL;
	int i = 0;

	char jack_files[128][128];
	fptr = fopen(files_path, "r");
	while (fgets(jack_files[i], 128, fptr))
	{
		jack_files[i][strlen(jack_files[i]) - 1] = '\0';
		i++;
	}
	/*
	int num_files = i;
	for (i = 0; i < num_files; i++)
		printf("%s\n", jack_files[i]);
	*/
	p.er = none;
	return p;
}

int StopCompiler()
{
	StopParser();
	return 1;
}

#ifndef TEST_COMPILER
int main()
{
	InitCompiler();
	ParserInfo p = compile("Pong");
	PrintError(p);
	StopCompiler();
	return 1;
}
#endif
