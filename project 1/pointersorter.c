#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node {
  char *word;
  struct node *next;
} node;

node *head = NULL;


void bubble_sort (struct node *front, int x){  //inputs are the front of the link list, x is the number of words
  int i , swapped, comp, innerforloop, outterforloop = 0;
  node *ptr;
  node *prev = NULL;
  node *end = NULL;
  
  if(front == NULL){
    printf("error empty linked list");
    return;
  }
  
  //in bubble sort we loop through the link list and compare ptr and ptr->next
  for (outterforloop =0; outterforloop< x; outterforloop++){ 
    ptr=head;
      do
      {
        swapped = 0;
        if(i ==0){ //only should be for the first time
          ptr = front;
          i++;
        }
        
        while(ptr ->next != NULL){
        
          if (ptr ->next ->word == NULL){
            
            for (innerforloop =0; innerforloop< x; innerforloop++){
            /* because for example, zZ zZz zZZZ,
                sawp will do ... fist compare zZZZ and zZz -> right order 
                second zZz and zZ -> wrong order -> swap
                and then we meet ptr->next->word==null,
                however, this does not compare zZ and zZZZ.
                therefore, we need to make a ptr = head and check everything again.
                so, reason for useing for loop x time (number of words) is
                for example, zZ zZz zZZZ zZZZZZZ,
                we need to go if statement (ptr->next->word == null) one more time
                so for the worst case, we might go to if statement x times...
                therefore, i make for loop to check x times that every words are right order
                (to check it we need to refresh ptr, ptr=head)
            */
            ptr = head;
            
             do
              {
                swapped = 0;
                if(i ==0){ //only should be for the first time
                  ptr = front;
                  i++;
                }
                while(ptr ->next != NULL){ //while its not the end of the link list
                
                  if (ptr ->next ->word == NULL){
                    break;
                  }
                  //comparing word, if ptr->next is great it should be swapped
                  comp = strcmp(ptr->word, ptr ->next ->word);
                  if(comp >0){
                    node *temp = NULL;
                    temp  = ptr;
                    ptr = ptr->next;
                    temp->next = ptr->next; //connecting the link list together so 
                    ptr->next = temp;
                    if(ptr->next == head){
                      head = ptr;
                    }
                    else{
                      prev->next = ptr;  
                    }
                    
                    swapped =1;
                  }
                  prev = ptr;
                  ptr = ptr->next;
                }
                
              }while(swapped);
              
             }//forloop
            break;
          } // first if statment end 
          
          comp = strcmp(ptr->word, ptr ->next ->word);
          if(comp >0){
            node *temp = NULL;
            temp  = ptr;
            ptr = ptr->next;
            temp->next = ptr->next;
            ptr->next = temp;
            if(ptr->next == head){
              head = ptr;
            }
            else{
              prev->next = ptr;  
            }
            
            swapped =1;
          }
          prev = ptr;
          ptr = ptr->next;
        }
        end = ptr;
        
      }while(swapped);
  }
  
  return;
}



/* creatNode : get a word from main and make a linked list node for it
  since, we don't know how many character will inside the one word, we need to contiguous memory
  (#of character in single word) => calloc it, not it
  get memory that data(our word) sitting in, and put it in ptr->word
  and then make linked list by linking with head
*/
void createNode (char * data){ 
  struct node* ptr = (struct node*) malloc (sizeof(struct node)); //pointer for making a node
  ptr->word = (char *) calloc(strlen(data), sizeof(char));
  memcpy(ptr -> word, data, strlen(data)); 
  ptr -> next = head;
  head = ptr;
}


int main (int argc, char **argv){
  if (argc == 1){ // if there is non input parameter 
    printf("no input error\n"); 
    return 0;
  }
  
  if (argc > 2){ // input - wrong parameter (ex. ./pointersorter "adfsadf" 12323)
    printf("wrong input parameter\n"); 
    return 0;
  }
  
  int numberofchar = strlen(argv[1]); // get how many character in argv[1]
  char * string = (char *) calloc(strlen(argv[1]), sizeof(char));
  memcpy(string, argv[1], numberofchar); 
  //string [numberofchar] = '\0'; // assign null character 
  char *ptrString = string;
  
  
  int firstindex = 0;
  int lastindex = 0;
  int index = 0;
  int i = 0; // count the number of character
  int x = -1; // count the number of word
  int asciicode=0;
  
  while(i != numberofchar){
      asciicode = string[i]; // get a ascii code for single character
      
      if (isalpha(asciicode)){
        firstindex = i; 
        i++;
        asciicode = string[i];
        
        while(isalpha(asciicode)){
          i++;
          asciicode = string[i];
       }
        lastindex = i-1;    
        index = lastindex-firstindex +1; // index = #of character in single word
        char * word = (char *) calloc(index, sizeof(int));
        strncpy(word, ptrString +firstindex, index); // we extract alphabet character word in argv[1] and put them in to word pointer
        createNode(word);
        free(word);
        x++;
      } else {
        i++;
      }
  }
  
  free(string);
    
    if (x == -1){ // there's no word inside the input
      printf ("error! there is no word\n");
      return 0;
    }
   bubble_sort(head, x+1);
    
    // traverse through linked list
    struct node *ptr = head;
    while(ptr  != NULL){
       printf("%s\n", ptr->word);
       free(ptr->word);
       ptr = ptr->next;
    }
    
    free(ptr);

  return 0; 
}