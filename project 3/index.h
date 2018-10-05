#ifndef INDEX_H
#define INDEX_H
#include <stdio.h>
#include <stddef.h>

typedef struct token 
{
  char *word;
  char *filename;
  int count;
  struct token *next; 
} token; 

void tokenizer (char* file_path, char* file_name);
void createToken (char * word, char* file_name);
void sortedInsert (token *newToken);
void indexer (char * file_path);
void createFile(char * file_path);
void sorted_count();

#endif 