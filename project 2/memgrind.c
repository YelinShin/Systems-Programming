#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include "mymalloc.h"


//1000 separate mallocs()s of 1 byte, then free() the 1000 1 byte pointers one by one
long testA(){
  int i;
  char * test[5000];
  struct timeval start;
  struct timeval finish;
  long avg = 0;
  
  gettimeofday(&start,0); 
  for(i =0; i < 1000; i++){
    printf("File is %s, Line: %d\n",__FILE__,__LINE__);
    test[i] = malloc(1);  
  }
  
  for(i =0; i < 1000; i++){
    printf("File is %s, Line: %d\n",__FILE__,__LINE__);
    free(test[i]);
  }
   gettimeofday(&finish,0);
   avg += ((finish.tv_sec - start.tv_sec) * 1000000 + finish.tv_usec-start.tv_usec);
   return avg; 
}

//first malloc() 1 byte and immediately free it
long testB(){
  int i = 0;
  char * test[5000];
  struct timeval start;
  struct timeval finish;
  long avg = 0;
  
   gettimeofday(&start,0); 
  for(i =0; i < 1000; i++){
    printf("File is %s, Line: %d\n",__FILE__,__LINE__);
    test[i] = malloc(1);  
    free(test[i]);
  }
    gettimeofday(&finish,0);
   avg += ((finish.tv_sec - start.tv_sec) * 1000000 + finish.tv_usec-start.tv_usec);
    
   return avg; 

}

//randomly choose between a 1 byte malloc() or free()ing a 1 byte pointer
long testC(){
  long avg = 0;
  int i = 0;
  struct timeval start;
  struct timeval finish;
  int array[1000] = {0};
  void * ptr[1000] = {};
  srand(time(NULL));
  int countMalloc = 0;
  int countFree= 0;
  
  gettimeofday(&start,0); 
  //make an array to hold the randomly generated numbers, either 0 or 1
  for(i = 0; i < 1000; i++){
    int r = rand();
    int value = r %2;
    array[i] = value; 
  
  }
  //if 1 we should malloc, if 0 we should free
  for(i = 0; i < 1000; i ++){
    if(array[i] == 1){
      countMalloc ++;
      ptr[countMalloc-1] = malloc(1);
    
    }else {
      countFree++;
      free(ptr[countFree-1]);
      array[countFree-1] = 0;
    }
  }
  //free the remaining bytes
  for(i = 0; i <1000;i ++){
    free(ptr[i]); 
  }
  

  printf("Free Count: %d Malloc Count: %d \n", countFree, countMalloc);
  //following commented code is to make sure we actually freed all of the pointers 
  //printf("File is %s, Line: %d\n",__FILE__,__LINE__);
  //char * x = malloc(4984);
  //free(x); 
 
  
   
  gettimeofday(&finish ,0);
  avg += ((finish.tv_sec - start.tv_sec) * 1000000 + finish.tv_usec-start.tv_usec);
  
  return avg;

}

long testD (){
  long avg = 0;
  int i = 0;
  struct timeval start;
  struct timeval finish;
  int arrayMF[1000] = {0};
  //int size[1000] = {0};
  char * ptr[1000] = {};
  srand(time(NULL));
  int countMalloc = 0;
  int countFree= 0;
  int totalBytes =0;
  
  gettimeofday(&start,0); 
  //if 1 we should malloc, if 0 we should free
  //while loop to make sure that we malloc 1000 times
 
  while(countMalloc < 1000){
    int r = rand() %2;
    int byte = (rand() %64) +1 ;
    if(r == 1){
      totalBytes += (byte +16);
      if(totalBytes > 5000){
        printf("Memory is full\n"); 
      }
      ptr[countMalloc] = malloc(byte);
      countMalloc++;
    
    }else {
      //freeing any pointer
      free(ptr[countMalloc-1]); 
      countFree ++;
    }
  }
  
  
  //free the remaining bytes
  for(i = 0; i <1000;i ++){
     free(ptr[i]); 
  }
  

  printf("Free Count: %d Malloc Count: %d \n", countFree, countMalloc);
  //following commented code is to make sure we actually freed all of the pointers 
 //printf("File is %s, Line: %d\n",__FILE__,__LINE__);
 // char * x = malloc(4984);
  // free(x); 
 
  
   
  gettimeofday(&finish ,0);
  avg += ((finish.tv_sec - start.tv_sec) * 1000000 + finish.tv_usec-start.tv_usec);
  
  return avg;



}


long testE (){
  long avg = 0;
  struct timeval start;
  struct timeval finish;
  
  gettimeofday(&start,0);
  printf("File is %s, Line: %d\n",__FILE__,__LINE__);
  char * a = malloc (200);
  int * b = malloc(3000);
  int * x = malloc(1);
  char * y = malloc(1);
  free(b);
  free (x); 
  char * c = malloc(3017); 
  free(a);
  free(c);
  free(y); 
  gettimeofday(&finish ,0);
  avg += ((finish.tv_sec - start.tv_sec) * 1000000 + finish.tv_usec-start.tv_usec);
  return avg; 
 
}

long testF(){
  long avg = 0;
  struct timeval start;
  struct timeval finish;
  char * test[5000];
  int i = 0;
  
  gettimeofday(&start,0);
  printf("File is %s, Line: %d\n",__FILE__,__LINE__);
  
  char * x = malloc(200);
  int * y = malloc(300);
  char * z = malloc(400); 
  char * xyz = malloc(1);
  
  
  free(x); //no merging
  free(z);//no merging
  free(y);// merging twice 
  
  int * a = malloc(932); //exact spot found 
  
  free(xyz);
  free(a);
  
  gettimeofday(&finish ,0);
  avg += ((finish.tv_sec - start.tv_sec) * 1000000 + finish.tv_usec-start.tv_usec);
  return avg;
}


int main (int argc, char **argv){
  int i =0;
  long avgA = 0;
  long avgB = 0;
  long avgC = 0;
  long avgD = 0;
  long avgE = 0;
  long avgF = 0;
  
  
  for(i = 0; i < 100; i++){
    //will print which tes case count we are in 
    printf( "Test %d/%d\n", i+1, 100);
    avgA += testA();
    avgB += testB();
    avgC += testC();
    avgD+= testD(); 
    avgE += testE();
    avgF += testF();
    
  }
  printf ("Average time for Test A: %ld microseconds\n", avgA/ 100);
  printf ("Average time for Test B: %ld microseconds\n", avgB/ 100);
  printf ("Average time for Test C: %ld microseconds\n", avgC/ 100);
  printf ("Average time for Test D: %ld microseconds\n", avgD/ 100);
  printf ("Average time for Test E: %ld microseconds\n", avgE/ 100);
  printf ("Average time for Test F: %ld microseconds\n", avgF/ 100);
  return 0; 
}
