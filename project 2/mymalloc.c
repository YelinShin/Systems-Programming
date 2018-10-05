#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "mymalloc.h"
#define malloc( x ) mymalloc( x, __FILE__, __LINE__ )
#define free( x ) myfree( x, __FILE__, __LINE__ )

//initialize head
static char myblock [5000];
meta* head = (void*) myblock; 

void initialize(){
  head->size = 5000 - sizeof(meta);
  head->usage = 0;
}

void *mymalloc(size_t numByte, char* file_name, int line_num){
  //need to initialize the size and usage of the top metadata
  if(!(head->size)){
    initialize();
    printf("Memory Inititalized\n");
    printf("\n");
  }
  
	meta *ptr = head;
  meta *tmp = head;
  int usagesize = 0;
  void * result =0;
  char * addressptr = NULL;
  int tmpsize=0;

  do{ //looking for a spot to malloc
    if((tmp->usage == 0) && (tmp->size >= numByte)){ //checking for a spot that we can malloc
      if(tmp->size == numByte){// Spot was found for the exact size asked,change usage to 1, and return the void* ptr
        tmp ->usage =1;
        addressptr = (char*)tmp;
        addressptr += sizeof(meta);
        result = (void*) addressptr;
        printf("Spot was found for the exact size malloced, Size allocated:%d\n", tmp->size);
        printf("\n");
        return result;
      }else if(tmp->size >= numByte + sizeof(meta) + 1){// spot was found and there is enough to create a new metadata plus 1
        //change size of tmp, usage of tmp
        //make new meta with new size and usage[0]
        //return void* ptr
        tmpsize = tmp->size; 
        tmp ->usage =1;
        tmp ->size = numByte;
        addressptr = (char*)tmp;
        addressptr += (sizeof(meta) + tmp->size);
        ptr = (meta*) addressptr; 
        ptr ->usage = 0;
        ptr->size = tmpsize - tmp->size - sizeof(meta); 

        addressptr = (char*)tmp;
        addressptr += sizeof(meta);
        result = (void*) addressptr;
        printf("Malloc was succcessful and new metadata was created, Size allocated: %d\n", tmp->size);
        printf("\n");
        return result;
      }else { //enough size to malloc but not enough for another metadata plus 1
        //give the remaining tmp size
        //return void *ptr
        tmp ->usage = 1;
        addressptr = (char*)tmp;
        addressptr += sizeof(meta);
        result = (void*) addressptr;
        printf("Malloc successful but not enough for another metadata, Size allocated: %d\n", tmp->size);
        printf("\n");
        return result; 
      }
    }
    usagesize += tmp->size + sizeof(meta);
    addressptr = (char*) tmp;
    addressptr +=(sizeof(meta) + tmp->size);
    tmp = (meta*) addressptr;
    
  }while (usagesize <5000);

  //no spot were found
  result = NULL; 
  fprintf(stderr, "Error at File: %s, Line: %d\n", __FILE__,__LINE__);
  printf("Error Description: Failed to malloc\n");
  printf("\n");
  return result;
}


void myfree (void * ptr, char* file_name, int line_num){
  meta *tmp = (meta*) ptr;
  char *addressptr = NULL;
  
  //check the void pointer address was actually malloced /exists 
  if (((void*)myblock<=ptr) && (ptr <=(void*)(myblock+5000))){
    addressptr = (char*) tmp; 
    addressptr -= sizeof(meta);
    tmp = (meta*) addressptr; 
 
     if((tmp -> usage) == 1){ // correct address so we free and check if we need to merge
       tmp->usage = 0;
       printf("Free was successful for: %x\n", ptr);
       printf("\n");
       merge();
       return;
     }else if(tmp->size ==0) {// print error when user gives address that wasn't malloced
       fprintf(stderr, "Error at File: %s, Line: %d\n", __FILE__,__LINE__);
       printf("Error Description: Please provide a valid pointer allocated by malloc\n");
       printf("\n");
       
       return;
     }else{ // print error when user free twice of same variable
       fprintf(stderr, "Error at File: %s, Line: %d\n", __FILE__,__LINE__);
       printf("Error Description: Pointer was already freed\n");
       printf("\n");
       return;
     }
  }else{ // print error when user free variable that is not malloced or not a pointer
    fprintf(stderr, "Error at File: %s, Line: %d\n", __FILE__,__LINE__);
    printf("Error Description: Please provide a valid pointer allocated by malloc\n");
    printf("\n");
    return;
  }
  return; 
}


void merge(){
  meta *tmp = head;
  meta *next = head;
  int usagesize = 0;
  char * addressptr = NULL;
  
  //checking through myblock at the head to find usages = 0 so we can check we should merge with the next metadta
  //only merge when next metadta's usage is also 0
  do{ 
    if( tmp -> usage == 0 ){
      addressptr = (char*) tmp ;
      addressptr += sizeof(meta) + tmp->size;
      next = (meta *) addressptr;
      if( next >= (myblock+5000)){
        printf("Nothing to merge \n");
        printf("\n");
        return;
      }else{
        while( next->usage ==0 ){ // we have to merge since we found the next consecutive metadata is also free
          if( next >= (myblock+5000)){ //next is out of the myblock 
            printf("Nothing to merge \n");
            printf("\n");
            return;
          }
          tmp -> size = (tmp -> size) + (next->size) + sizeof(meta);
          printf("Merge was successful\n");
          printf("\n");
          addressptr = (char *) tmp;
          addressptr += (sizeof(meta) + tmp->size) ;
          next = (meta*) addressptr;
          
          if(next->size == 0){ // there are no next metadata to check
            return;
          }
        } 
        usagesize = usagesize + sizeof(meta) + (tmp->size);
        tmp = next; 
        addressptr = (char*)next;
        addressptr += (sizeof(meta) + next->size );
        next = (meta*) addressptr;  
      }
    }else { // if usage is not freed
      usagesize = usagesize+sizeof(meta)+ (tmp->size);
      tmp = next;
      addressptr = (char*)next;
      addressptr += (sizeof(meta) + next->size );
      next = (meta*) addressptr;  
    }
  }while ( usagesize < 5000);


}