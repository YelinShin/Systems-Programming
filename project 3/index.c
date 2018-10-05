#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h> 
#include <sys/param.h> 
#include <libgen.h>
#include <errno.h>
#include "index.h"


token *head = NULL; 

void tokenizer (char* file_path, char* file_name) // get word from the file 
{
 
  FILE* fp = fopen(file_path, "r");
  
  fseek(fp, 0, SEEK_END);
  if (ftell(fp) == 0){
    printf("empty file! \n");
    return;
  }
  
  fp = fopen (file_path, "r");
  
  char ch = NULL;
  int word_count = 0;
  while((ch = fgetc(fp)) != EOF){
    word_count++;
  }
  fclose(fp);
  fp = fopen(file_path, "r");
  
  char *line = NULL;
  char strTemp[word_count+1] ;
  int linelength= 0;
  int wordlength=0;
  int x =0;
  
 
  while(!feof(fp))
  {
    line = fgets(strTemp, sizeof(strTemp), fp);
    
    if (line == NULL){ // skip blank line
      continue;
    }
    
    linelength = strlen(line);
    
    for(x = 0; x <linelength ; x++)
    {
      if(isalpha(line[x])==0 ){
        if(isdigit(line[x]) !=0){
          continue;
        }
        line[x] = ' ';
      } else if(isdigit(line[x]) ==0){
        if(isalpha(line[x])!=0){
          continue;
        }
        line[x] = ' ';
      }
    }
    
    int i =0;
    int asciicode =0;
    int firstindex =0;
    int lastindex=0;
    int index =0;
    char *ptrString = line;
    
    while (i != linelength){
      asciicode = line[i];
      
      if (isalpha(asciicode)){
        firstindex = i;
        i++;
        asciicode = line[i];
        
        while(isalpha(asciicode) || isdigit(asciicode)){
          i++;
          asciicode = line[i];
        }
        
        lastindex = i-1;
        index = lastindex-firstindex+1;
        char * word = (char*) calloc (index, sizeof(int));
        strncpy(word, ptrString+firstindex, index);
        
        
        createToken(word, file_name);
        free(word);
      } else {
        i++;
      }
    }

  }

}



void createToken (char * word, char* file_name)
{
  struct token *ptr = (struct token*) malloc (sizeof(struct token));
  ptr -> word = (char *) calloc(strlen(word), sizeof(char));
  ptr -> filename = (char *) calloc(strlen(file_name), sizeof(char));
  
  int x = 0;
  for(x = 0; x <strlen(word) ; x++)
    {
      word[x] = (char) tolower(word[x]);
    }
  
  memcpy(ptr ->word, word, strlen(word));
  memcpy(ptr ->filename, file_name, strlen(file_name));
  ptr->count = 1;
  
  sortedInsert(ptr);

  
  return;
}


void sortedInsert (token *newToken) // compare the word and sorted by the word (not count)
{
  token *ptr = head; 
  int comp = 0;
  if(head == NULL)
  {
    head = newToken;
    
    return;
  }
  do
  {
    comp = strcmp (newToken->word , ptr->word); 
    while(comp > 0)
    {
      if(ptr->next != NULL)
      {
         comp = strcmp (newToken->word , ptr->next->word);
         if(comp >0)
         {
           ptr = ptr->next; 
         }else if(comp < 0)
         {
           break; 
         }else if(comp == 0)
         {
           ptr = ptr->next;
           break; 
         }
      }else if (ptr->next == NULL)
        {
          ptr->next = newToken;
          return;  
        }
    }
    if(comp < 0)
    {
      if(strcmp (newToken->word , head->word) < 0)
      {
        newToken ->next = head;
        head = newToken; 
        return; 
      }
      newToken->next = ptr->next;
      ptr->next = newToken; 
      return; 
    }
    if(comp == 0)
    {
      if(strcmp(newToken->filename, ptr->filename) == 0){
        ptr->count +=1; 
        return;
      }else {
        while(ptr->next != NULL && strcmp(newToken->word, ptr->next->word) == 0 ){
          if(strcmp(newToken->filename, ptr->next->filename) == 0){
            ptr->next->count +=1; 
            return;
          }else{
            ptr=ptr->next;
            continue;
          }
        }
        newToken->next = ptr->next;
        ptr->next = newToken; 
        return;
      }
    }
  }while (ptr ->next != NULL);
}

void indexer (char * file_path){ //recursive for looking dir/file 

  struct dirent * file = NULL;
  DIR * dirfd = opendir(file_path);
  
  int check_count = 0;
  
  if (dirfd == NULL){
    printf("empty directory!\n");
    return;
  }
  
  char *orgin_path = (char*) calloc (MAXPATHLEN, sizeof(char));
  char *file_name = NULL;
  
  do{
    file = readdir(dirfd);
    if (file == NULL){
      if (check_count == 2){
        printf("empty directory!\n");
        return;
      }
      return;
    }
    
    if (strcmp (file->d_name, ".") == 0 || strcmp (file->d_name, "..")==0){
      ++check_count;
      continue;
    }
    
    int x =0;
    check_count = 0;
    
    if(file!=NULL) {
      if(file->d_type == DT_REG){
        strncpy(orgin_path, file_path, strlen(file_path));
        strcat(file_path, "/");
        strcat(file_path, file->d_name);
        file_name = file->d_name;
        tokenizer(file_path, file_name);
        *file_path = 0;
        strcpy(file_path, orgin_path);
      }else if (file->d_type == DT_DIR){
        strncpy(orgin_path, file_path, strlen(file_path));
        strcat(file_path, "/");
        strcat(file_path, file->d_name);
        indexer(file_path);
        *file_path = 0;
        strcpy(file_path, orgin_path);
      }else{
        printf("found something else (neither file nor directory\n");
        continue;
      } 
    }
  }while(file!= NULL);
  free(orgin_path);
}


void createFile(char * file_path){ // create file in the path (argv[1]) in xml format with write function
  token *ptr = head; 
  int result = 0; 
  int x =0;
  
  if (head == NULL){
    return;
  }

  int fd = open (file_path, O_RDWR | O_CREAT | O_EXCL, S_IWUSR | S_IRUSR);
  //error checking: file already exists (replace?), can't create 
  
  if (errno == EEXIST){
    close(fd);
    char test = NULL;
    perror("Error output");
   
    printf("Inverted index file with the same name already exist, do you want to overwrite? (y/n)\n");
    scanf ("%c", &test);
    if (test =='y'){
     fd = open (file_path, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
   } else if (test =='n'){
     printf("user stated to not overwrite file. No interveted index file created.\n");
     return;
   } else{
    printf("invaild input.\n"); // when they enter wrong input just not creating file
    return;
    }
  }
  
  write(fd, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n", 40);
  write(fd, "<fileIndex>\n", 13);
  do{
    if(ptr != head){
      ptr = ptr->next; 
    }
    char numCount[10];
    sprintf(numCount, "%d", ptr->count);
    write(fd,"\t<word text=\"", 13);
    write(fd, ptr->word, strlen(ptr->word));
    write(fd,"\">\n",3);
    write(fd,"\t\t<filename=\"", 13); 
    write(fd, ptr->filename, strlen(ptr->filename));
    write(fd,"\">",2);
    write(fd, numCount, strlen(numCount));
    write(fd,"</file>\n", 8);
    
    for(x = 0; x < 10; x++){
      numCount[x] = ' '; // make numCount == NULL
    }
    if(ptr->next == NULL){
      write(fd,"\t</word>\n", 9);
      break;
    }
    
    if (strcmp(ptr->word, ptr->next->word) != 0){
      write(fd,"\t</word>\n", 9);
      ptr = ptr->next;
      sprintf(numCount, "%d", ptr->count);
      write(fd,"\t<word text=\"", 13);
      write(fd, ptr->word, strlen(ptr->word));
      write(fd,"\">\n",3);
      write(fd,"\t\t<filename=\"", 13); 
      write(fd, ptr->filename, strlen(ptr->filename));
      write(fd,"\">",2);
      write(fd, numCount, strlen(numCount));
      write(fd,"</file>\n", 8);
      
      
      if(ptr->next == NULL ){
        break;
      }
      
      while(strcmp(ptr->word, ptr->next->word) == 0){
        ptr= ptr->next;
        sprintf(numCount, "%d", ptr->count);
        write(fd,"\t\t<filename=\"", 13); 
        write(fd, ptr->filename, strlen(ptr->filename));
        write(fd,"\">",2);
        write(fd, numCount, strlen(numCount));
        write(fd,"</file>\n", 8);
        for(x = 0; x < 10; x++){
          numCount[x] = ' ';
        } 
          if(ptr->next == NULL || ptr == NULL){
            break;
          }
      }//end while loop
      write(fd,"\t</word>\n", 9);
      continue;
    }
    
    while(strcmp(ptr->word, ptr->next->word) == 0){
      ptr= ptr->next;
      sprintf(numCount, "%d", ptr->count);
      write(fd,"\t\t<filename=\"", 13); 
      write(fd, ptr->filename, strlen(ptr->filename));
      write(fd,"\">",2);
      write(fd, numCount, strlen(numCount));
      write(fd,"</file>\n", 8);
      for(x = 0; x < 10; x++){
        numCount[x] = ' ';
      } 
      if(ptr->next == NULL || ptr == NULL){
        break;
      }
    }
    write(fd,"\t</word>\n", 9);
    
  }while(ptr-> next != NULL);
  
  
  write(fd,"</fileIndex>\n", 13);
}

void sorted_count()
{
  token *ptr = head;
  token *temp_head = NULL;
  token *temp_last = NULL;
  int swapped = 0;
  
  if (head == NULL){
    return;
  }
  
  if (ptr == NULL){
    printf("no indexed word\n");
    return;
  }
  if (ptr->next == NULL){
      return;
  }
  
  do{
    if (strcmp (ptr->word, ptr->next->word) != 0 ){
      ptr = ptr->next;
      continue;
    }
    
    temp_head = ptr;
    
    do{
      swapped = 0;
      ptr = temp_head;
      
      if(ptr->next == NULL){
        break;
      }
      
      while(strcmp (ptr->word, ptr->next->word) == 0){
        ptr= ptr->next;
        if (ptr->next == NULL){
          break;
        }
      }
      
      temp_last = ptr;
      ptr = temp_head;
      
      while ( ptr -> next != temp_last->next){
        if(ptr->count < ptr->next->count){
          char* temp_filename = (char*) calloc (strlen(ptr->filename), sizeof(char));
          int temp_count = 0;
          
          temp_filename = ptr->filename;
          temp_count = ptr->count;
          
          ptr->filename = ptr->next->filename;
          ptr->next->filename = temp_filename;
          
          ptr->count = ptr->next->count;
          ptr->next->count =temp_count;
          swapped = 1;
        }
        ptr = ptr->next;
        if(ptr->next == NULL && swapped == 0){
          break;
        }
      }// end while
    }while(swapped);
    
    if (ptr->next == NULL){
      break;
    }
    ptr= ptr->next;
    
  }while(ptr->next!=NULL); // big do-while
}



int main (int argc, char *argv[]){
  //argv[1] is output file path 
  //argv[2] is what we need to index 
  
  if (argc != 3){
    printf("wrong number of input argument\n");
    return;
  }
  
  DIR * dirfd_out = opendir(argv[1]);
  
  if(dirfd_out == NULL){
    if (errno == EISDIR){
      perror("Error from argv[1]");
      return;
    }
  }

  DIR * dirfd = opendir(argv[2]);
  
  if (dirfd == NULL){
    //argv[2] is a file path not a directory path
    
    if (errno == ENOENT){
      perror("Error from argv[2]");
      return;
    }   
    
    char * filename = (char*) calloc (strlen(argv[2]), sizeof(char));
    
    filename = basename (argv[2]);
    tokenizer(argv[2],filename);
    sorted_count();
    createFile(argv[1]);
    closedir(dirfd);
    closedir(dirfd_out);
    return 0;
  }
  
  
  char *path = (char*) calloc (MAXPATHLEN, sizeof(char));
  path = argv[2];
  indexer(path);
  closedir(dirfd);
  closedir(dirfd_out);
  sorted_count();
  createFile(argv[1]); 
  
return 0; 
}