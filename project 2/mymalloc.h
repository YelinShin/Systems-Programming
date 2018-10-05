#ifndef MYMALLOC_H
#define MYMALLOC_H
#define malloc( x ) mymalloc( x, __FILE__, __LINE__ )
#define free( x ) myfree( x, __FILE__, __LINE__ )
#include <stdio.h>
#include <stddef.h>
#include <errno.h>



static char myblock [5000];

typedef struct Meta{
	size_t size;
	int usage;
} meta;

void initialize();
void *mymalloc(size_t numByte, char* file_name, int line_num);
void myfree (void * ptr, char* file_name, int line_num);
void merge();




#endif