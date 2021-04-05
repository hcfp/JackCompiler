/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
Lexer Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name: Habib Chanchwelia
Student ID: 201329422
Email: sc19hc@leeds.ac.uk
Date Work Commenced: 15/02/2021
*************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

// YOU CAN ADD YOUR OWN FUNCTIONS, DECLARATIONS AND VARIABLES HERE

int lineNum;
long pos = 0;
char *stream = NULL;
char *file_name_global;

// IMPLEMENT THE FOLLOWING functions
//***********************************

// Initialise the lexer to read from source file
// file_name is the name of the source file
// This requires opening the file and making any necessary initialisations of the lexer
// If an error occurs, the function should return 0
// if everything goes well the function should return 1
int InitLexer(char *file_name)
{
  file_name_global = file_name;
  long stream_size;
  size_t result;

  FILE *fp = fopen(file_name, "rb");
  if (fp == NULL)
  {
    printf("File not found\n");
    return 0;
  }

  fseek(fp, 0, SEEK_END);
  stream_size = ftell(fp);
  rewind(fp);

  stream = malloc((stream_size + 1) * sizeof(char));
  result = fread(stream, 1, stream_size, fp);

  if (result != stream_size)
  {
    return 0;
  }

  stream[stream_size] = '\0';
  lineNum = 1;

  return 1;
}

// Get the next token from the source file
Token GetNextToken()
{
  Token t;
  t.tp = ERR;
  t.lx[0] = '\0';
  char c = stream[pos];

  char reserved_words[21][15] = {
      "class", "constructor", "method",
      "function", "int", "boolean",
      "char", "void", "var",
      "static", "field", "let",
      "do", "if", "else",
      "while", "return", "true",
      "false", "null", "this"};

  char symbols[19][2] = {
      "(", ")", "[", "]",
      "{", "}", ",", ";", "=", ".", "+", "-",
      "*", "/", "&", "|", "~", "<", ">"};

  //consumes leading whitespace
  while (c != '\0' && isspace(c))
  {
    // handles both CRLF and LF
    if (c == '\r' && stream[pos + 1] == '\n')
    {
      lineNum++;
      pos += 2;
    }
    else if (c == '\n')
    {
      lineNum++;
      pos++;
    }
    else
    {
      pos++;
    }
    c = stream[pos];
  }

  // Skips the rest of the characters on a line
  // once a comment is found
  // First handle multi-line comments
  while ((c == '/' && stream[pos + 1] == '*') || (c == '/' && stream[pos + 1] == '/'))
  {
    if (c == '/' && stream[pos + 1] == '*')
    {
      pos += 2;
      c = stream[pos];
      if (c == '*')
      {
        pos++;
        c = stream[pos];
      }
      int inComment = 1;
      while (inComment)
      {
        if (c == '\0')
        {
          t.tp = ERR;
          strcpy(t.lx, "Error: unexpected eof in comment");
          t.ec = 0;
          t.ln = lineNum;
          strcpy(t.fl, file_name_global);
          return t;
        }
        while (c != '\0' && isspace(c))
        {
          // handles both CRLF and LF
          if (c == '\r' && stream[pos + 1] == '\n')
          {
            lineNum++;
            pos += 2;
          }
          else if (c == '\n')
          {
            lineNum++;
            pos++;
          }
          else
          {
            pos++;
          }
          c = stream[pos];
        }
        if (c == '*' && stream[pos + 1] == '/')
        {
          inComment = 0;
          pos += 2;
        }
        else
        {
          pos++;
          c = stream[pos];
        }
      }
      c = stream[pos];
      while (c != '\0' && isspace(c))
      {
        // handles both CRLF and LF
        if (c == '\r' && stream[pos + 1] == '\n')
        {
          lineNum++;
          pos += 2;
        }
        else if (c == '\n')
        {
          lineNum++;
          pos++;
        }
        else
        {
          pos++;
        }
        c = stream[pos];
      }
    }

    //skips single line comments
    // also checks if there are comments on the next line
    if (c == '/' && stream[pos + 1] == '/')
    {
      pos += 2;
      c = stream[pos];
      while ((c != '\n') && (c != '\r'))
      {
        pos++;
        c = stream[pos];
      }
      while (c != '\0' && isspace(c))
      {
        // handles both CRLF and LF
        if (c == '\r' && stream[pos + 1] == '\n')
        {
          lineNum++;
          pos += 2;
        }
        else if (c == '\n')
        {
          lineNum++;
          pos++;
        }
        else
        {
          pos++;
        }
        c = stream[pos];
      }
    }
  }

  // Find end of file
  if (c == '\0')
  {
    t.tp = EOFile;
    strcpy(t.lx, "End of File");
    t.ln = lineNum;
    strcpy(t.fl, file_name_global);
    pos++;
    return t;
  }

  // Get strings
  if (c == '"')
  {
    pos++;
    c = stream[pos];
    while (c != '"')
    {
      if (c == '\n' || c == '\r')
      {
        t.tp = ERR;
        strcpy(t.lx, "Error: new line in string constant");
        t.ec = 1;
        t.ln = lineNum;
        strcpy(t.fl, file_name_global);
        return t;
      }
      if(c == '\0') {
        t.tp = ERR;
        strcpy(t.lx, "Error: unexpected eof in string constant");
        t.ec = 2;
        t.ln = lineNum;
        strcpy(t.fl, file_name_global);
        return t;
      }
      int len = strlen(t.lx);
      t.lx[len] = c;
      t.lx[len + 1] = '\0';
      pos++;
      c = stream[pos];
    }
    t.tp = STRING;
    t.ln = lineNum;
    strcpy(t.fl, file_name_global);
    pos++;
    c = stream[pos];
    return t;
  }

  // Finds identifiers or reserved words
  if (isalpha(c) || c == '_')
  {
    while (c != '\0' && (isalnum(c) || c == '_'))
    {
      int len = strlen(t.lx);
      t.lx[len] = c;
      t.lx[len + 1] = '\0';
      pos++;
      c = stream[pos];
    }
    // Checks if t.lx is a reserved word
    for (int i = 0; i < 21; i++)
    {
      if (strcmp(t.lx, reserved_words[i]) == 0)
      {
        t.tp = RESWORD;
        t.lx[strlen(t.lx)] = '\0';
        t.ln = lineNum;
        strcpy(t.fl, file_name_global);
        return t;
      }
    }
    t.tp = ID;
    t.lx[strlen(t.lx)] = '\0';
    t.ln = lineNum;
    strcpy(t.fl, file_name_global);
    return t;
  }

  // Finds ints
  if (isdigit(c))
  {
    while ((c != '\0') && isdigit(c))
    {
      int len = strlen(t.lx);
      t.lx[len] = c;
      t.lx[len + 1] = '\0';
      pos++;
      c = stream[pos];
    }
    t.tp = INT;
    t.ln = lineNum;
    strcpy(t.fl, file_name_global);
    return t;
  }

  for (int i = 0; i < 19; i++)
  {
    if (c == symbols[i][0])
    {
      strcpy(t.lx, symbols[i]);
      t.tp = SYMBOL;
      t.ln = lineNum;
      strcpy(t.fl, file_name_global);
      pos++;
      c = stream[pos];
      return t;
    }
  }
  // if c hasn't been recognised so far, it is an illegal symbol
  pos++;
  t.tp = ERR;
  strcpy(t.lx, "Error: illegal symbol in source file");
  t.ec = 3;
  t.ln = lineNum;
  strcpy(t.fl, file_name_global);
  return t;
}

// peek (look) at the next token in the source file without removing it from the stream
Token PeekNextToken()
{
  Token t;
  t.tp = ERR;
  int init_pos = pos;
  int init_line_num = lineNum;
  t = GetNextToken();
  pos = init_pos;
  lineNum = init_line_num;
  return t;
}

// clean out at end, e.g. close files, free memory, ... etc
int StopLexer()
{
  free(stream);
  lineNum = 0;
  pos = 0;
  stream = NULL;
  char *file_name_global = NULL;
  return 0;
}

// do not remove the next line
#ifndef TEST
int main()
{
  // implement your main function here
  // NOTE: the autograder will not use your main function
  if (InitLexer("Ball.jack") != 0)
  {
    Token t;
    while (t.tp != EOFile && t.tp != ERR)
    {
      t = GetNextToken();
      if (strlen(t.lx) > 0)
      {
        printf("< %s, ", t.fl);
        printf("%d, ", t.ln);
        printf("%s, ", t.lx);
        printf("%d >\n", t.tp);
      }
    }
  }

  StopLexer();
  return 0;
}
// do not remove the next line
#endif
