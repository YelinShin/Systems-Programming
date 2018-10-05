#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h> 

pthread_mutex_t lock;

typedef struct fd
{
  char* filepath ;
  int mode;
  int fd;
  int valid;
  
  
}fd;


fd listFD[100];
int newsocketFD[1024];


void error (char *message)
{
  perror(message);
  exit(1);
}

void *clientservice (void *sockptr)
{
  pthread_mutex_lock(&lock);
  int *tmp = (int*) sockptr;
  int newsocketfd = *tmp; 
  char * token = (char *)calloc(256, sizeof(char));
  char delim[2] = ",";
  char * filepath;
  char * mode;
  int i,j,tempfd,x,a = 0;
  char buffer[256];

   bzero(buffer, 256);
   x = read(newsocketfd, buffer, 255); 
    if(x < 0)
    {
      error("Error reading from socket");
    }
    printf("Received from client: %s\n", buffer);
     

    char* write_buff = (char*) calloc(256, sizeof(char));
    sprintf(write_buff, "%s", buffer);
    
    token = strtok(buffer, delim);
    a = atoi(token);
    
   switch(a)
   {
      case 1 :
        //open()
        
        filepath = (char*) calloc(256, sizeof(char));
        mode = (char*) calloc(10, sizeof(char));
        printf("open\n");
        
        
        filepath = strtok(NULL,delim);
        mode = strtok(NULL,delim);
   
        int p =0;
        int nummode = atoi(mode);
        if(nummode == 0)
        {
          tempfd = open(filepath, O_RDONLY);
        }else if(nummode == 1)
        {
          tempfd = open(filepath, O_WRONLY);
        }else if(nummode == 2)
        {
          tempfd = open(filepath, O_RDWR);
        }
        
        if(tempfd == -1)
        {
          bzero(buffer, 256); 
          if(errno == EACCES)
          {
            strcat(buffer, "-1,13");
            x = write(newsocketfd, buffer, 255);
            printf("Sent to client: %s\n", buffer);
            if (x < 0)
            {
              error("Error writing to socket");
            }
             close (newsocketfd); 
             pthread_mutex_unlock(&lock); 
             return;  
            }else if(errno == EINTR)
            {
              strcat(buffer, "-1,4");
              x = write(newsocketfd, buffer, 255);
              printf("Sent to client: %s\n", buffer);
              if (x < 0)
              {
                error("Error writing to socket");
              }
               close (newsocketfd); 
               pthread_mutex_unlock(&lock); 
               return;  
            }else if(errno == EISDIR)
            {
              strcat(buffer, "-1,21");
              x = write(newsocketfd, buffer, 255);
              printf("Sent to client: %s\n", buffer);
              if (x < 0)
              {
                error("Error writing to socket");
              }
               close (newsocketfd); 
               pthread_mutex_unlock(&lock); 
               return;  
            }else if(errno == ENOENT)
            {
              strcat(buffer, "-1,2");
              x = write(newsocketfd, buffer, 255);
              printf("Sent to client: %s\n", buffer);
              if (x < 0)
              {
                error("Error writing to socket");
              }
               close (newsocketfd); 
               pthread_mutex_unlock(&lock); 
               return; 
            
            }else if(errno == EROFS)
            {
              strcat(buffer, "-1,30");
              x = write(newsocketfd, buffer, 255);
              printf("Sent to client: %s\n", buffer);
              if (x < 0)
              {
                error("Error writing to socket");
              }
               close (newsocketfd); 
               pthread_mutex_unlock(&lock); 
               return;  
            }else
            {
              strcat(buffer, "-1");
              x = write(newsocketfd, buffer, 255);
              printf("Sent to client: %s\n", buffer);
              if (x < 0)
              {
                error("Error writing to socket");
              }
              close (newsocketfd); 
              pthread_mutex_unlock(&lock); 
              return; 
            
            }
            
          }
        
        printf("tempfd: %d\n", tempfd); 
        //return neg tempfd to client
    
        for(p;p < 100; p++)
        {
          if(listFD[p].valid == 0)
          {
            
            listFD[p].valid = calloc(1,sizeof(int));
            listFD[p].valid = 1;
            
            listFD[p].filepath = (char*)calloc(256, sizeof(char));
            listFD[p].filepath = filepath;
            
            listFD[p].mode = calloc(10, sizeof(int));
            listFD[p].mode = nummode;
            
            listFD[p].fd = calloc(10, sizeof(int));
            listFD[p].fd = tempfd;
            
            break;
          }
      
        }
        
        bzero(buffer,256); 
        sprintf(buffer, "%d", (tempfd * -1));

 
        break;
      case 2:
        //read()
        printf("read\n"); 
        char * fd = (char*) calloc(10, sizeof(char));
        char *bytesize = (char*) calloc(10, sizeof(char));
        int size,j = 0; 
        char * tempbuff[256];
        int result = 0;
        
        
        fd = strtok(NULL,delim);
        tempfd = atoi(fd);
        tempfd = (tempfd *-1);
  
        
        bytesize = strtok(NULL,delim);
        size = atoi(bytesize);
        
   
        bzero(tempbuff, 256); 
        for(j =0; j< 100; j++)
        {
          if(listFD[j].valid == 1)
          {
            if(tempfd == listFD[j].fd)
            {
              if(listFD[j].mode == 1)
              {
                errno = EBADF;
                result = -1; 
                bzero(buffer, 256);
                sprintf(buffer, "%d", result);
                break;
              }else 
              {
              result = read (tempfd, tempbuff, size); 
              
              
              if(result == -1)
              {
                bzero(buffer, 256); 
                if(errno == ETIMEDOUT)
                {
                  strcat(buffer, "-1,110");
                  x = write(newsocketfd, buffer, 255);
                  printf("Sent to client: %s\n", buffer);
                  if (x < 0)
                  {
                    error("Error writing to socket");
                  }
                   close (newsocketfd); 
                   pthread_mutex_unlock(&lock); 
                   return;  
                }else if(errno == EBADF)
                {
                  strcat(buffer, "-1,9");
                  x = write(newsocketfd, buffer, 255);
                  printf("Sent to client: %s\n", buffer);
                  if (x < 0)
                  {
                    error("Error writing to socket");
                  }
                   close (newsocketfd); 
                   pthread_mutex_unlock(&lock); 
                   return;  
                }else if(errno == ECONNRESET)
                {
                  strcat(buffer, "-1,104");
                  x = write(newsocketfd, buffer, 255);
                  printf("Sent to client: %s\n", buffer);
                  if (x < 0)
                  {
                     error("Error writing to socket");
                  }
                  close (newsocketfd); 
                  pthread_mutex_unlock(&lock); 
                  return;  
                }
                strcat(buffer, "-1");
                x = write(newsocketfd, buffer, 255);
                printf("Sent to client: %s\n", buffer);
                if (x < 0)
                {
                     error("Error writing to socket");
                }
                close (newsocketfd); 
                pthread_mutex_unlock(&lock); 
                return; 
                
              }
              
              char * tempresult = (char*) calloc(100, sizeof(char));
              bzero(buffer,256);
              sprintf(tempresult, "%d", result);
              
              strcat(buffer,",");
              strcat(buffer, tempresult);
              strcat(buffer,",");
              strcat(buffer, tempbuff);
              break;
              }
            }
          }
        }
        if(j>= 100)
        {
          errno = EBADF;
          result = -1; 
          bzero(buffer, 256);
          sprintf(buffer, "%d", result);
          break;
       }
       //set perrno
        break;
        
       case 3:
        //write()
        printf("write\n"); 
        char * write_fd = (char*) calloc(10, sizeof(char));
        char *write_bytesize = (char*) calloc(10, sizeof(char));
        int write_size,m = 0; 
        char * write_tempbuff[256];
        int write_result = 0;
        char * write_data = (char*)calloc(256,sizeof(char));
        
        
        write_fd = strtok(NULL,delim);
        int  tmpsize = 0;
        tmpsize = strlen(write_fd);
        tempfd = atoi(write_fd);
        tempfd = (tempfd *-1);
  
        
        write_bytesize = strtok(NULL,delim);
        write_size = atoi(write_bytesize);
        tmpsize += strlen(write_bytesize);
        tmpsize += 4;
      
        sprintf(write_data,"%s",write_buff+(tmpsize));
  

       
        for(m =0; m< 100; m++)
        {
          if(listFD[m].valid == 1)
          {
            if(tempfd == listFD[m].fd)
            {
              if(listFD[m].mode == 0)
              {
                //not valid fd
                errno = EBADF;
                write_result = -1; 
                bzero(buffer, 256);
                sprintf(buffer, "%d", write_result);
                break;
              }else 
              {
              write_result = write (tempfd, write_data, write_size); 
              if(write_result == -1)
              {
                bzero(buffer, 256); 
                if(errno == ETIMEDOUT)
                {
                  strcat(buffer, "-1,110");
                  x = write(newsocketfd, buffer, 255);
                  printf("Sent to client: %s\n", buffer);
                  if (x < 0)
                  {
                    error("Error writing to socket");
                  }
                   close (newsocketfd); 
                   pthread_mutex_unlock(&lock); 
                   return;  
                }else if(errno == EBADF)
                {
                  strcat(buffer, "-1,9");
                  x = write(newsocketfd, buffer, 255);
                  printf("Sent to client: %s\n", buffer);
                  if (x < 0)
                  {
                    error("Error writing to socket");
                  }
                   close (newsocketfd); 
                   pthread_mutex_unlock(&lock); 
                   return;  
                }else if(errno == ECONNRESET)
                {
                  strcat(buffer, "-1,104");
                  x = write(newsocketfd, buffer, 255);
                  printf("Sent to client: %s\n", buffer);
                  if (x < 0)
                  {
                     error("Error writing to socket");
                  }
                  close (newsocketfd); 
                  pthread_mutex_unlock(&lock); 
                  return;  
                }
                strcat(buffer, "-1");
                x = write(newsocketfd, buffer, 255);
                printf("Sent to client: %s\n", buffer);
                if (x < 0)
                {
                     error("Error writing to socket");
                }
                close (newsocketfd); 
                pthread_mutex_unlock(&lock); 
                return; 
                
              }
              
              bzero(buffer,256);
              sprintf(buffer, "%d", write_result);
              break;
              }
            }
          }
        }
        if(m>= 100)
        {
          //file has not been open, invalid fd
          errno = EBADF;
          write_result = -1; 
          bzero(buffer, 256);
          sprintf(buffer, "%d", write_result);
          break;
       }
       break;
      case 4:
        //close()
        printf("close\n"); 
        char * close_fd = (char*) calloc(10, sizeof(char));
        int n = 0; 
        char * close_tempbuff[256];
        int close_result = 0;
        
        
        close_fd = strtok(NULL,delim);
        tempfd = atoi(close_fd);
        tempfd = (tempfd *-1);
  
        
   
        bzero(tempbuff, 256); 
        for(n =0; n< 100; n++)
        {
          if(listFD[n].valid == 1)
          {
            if(tempfd == listFD[n].fd)
            {
              close_result = close(tempfd);
              if(close_result == 0)
              {

                listFD[n].valid =0;
                listFD[n].filepath = NULL;
                listFD[n].mode =0;;   
                listFD[n].fd =0;
                bzero(buffer, 256);
                sprintf(buffer, "%d", close_result);
                break;
              }else 
              {
              errno = EBADF;
              close_result = -1; 
              bzero(buffer, 256);
              sprintf(buffer, "%d", close_result);
              break;

              }
            }
          }
        }
        if(n>= 100)
        {
          errno = EBADF;
          close_result = -1; 
          bzero(buffer, 256);
          sprintf(buffer, "%d", close_result);
          break;
       }
        break;
      default :
        break;
    }    
   
   
    x = write(newsocketfd, buffer, 255);
    printf("Sent to client: %s\n", buffer);
    if (x < 0)
    {
      error("Error writing to socket");
    }
   close (newsocketfd); 
   pthread_mutex_unlock(&lock); 
   return; 
}




int main (int argc, char *argv[])
{
  int socketfd, newsocketfd, clilen = 0;
  char buffer[256];
  struct sockaddr_in servAddr, cliAddr;
  int x = 0;
 
  pthread_t thread;
  pthread_mutex_init(&lock, NULL);
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  
  if(socketfd < 0)
  {
    error("Error opening socket");
  }
  bzero((char*) &servAddr, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = INADDR_ANY;
  servAddr.sin_port = htons(49628);
  
  if(bind(socketfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
  {
    error("Error on binding");
  }
   
  listen (socketfd, 10);
  
  
  while(1)
  {  
    clilen = sizeof(cliAddr); 
    newsocketfd = accept(socketfd, (struct sockaddr *)&cliAddr, &clilen);
    if(newsocketfd < 0)
    {
      error("Error on accept");
    }

     pthread_create(&thread, NULL, clientservice, &newsocketfd);
     int errnum = pthread_detach (thread);
     if(errnum!= 0)
     {
        error("Failed to join thread");
     }
     
  } 
  pthread_exit(&thread);
  close(socketfd);
  return 0; 

}