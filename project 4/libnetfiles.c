#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netdb.h>
#include "libnetfiles.h"
#include <errno.h>


struct hostent *server; 
extern int netserverinit (char *hostname)
{
  struct sockaddr_in servAddr;
  
  if(hostname == NULL)
  {
    h_errno = HOST_NOT_FOUND;
    return -1;
  }
  
  server = gethostbyname(hostname);
  if (server == NULL)
  {
    h_errno = HOST_NOT_FOUND;
    printf("should be host not found\n");
    perror("Error \n");
    return -1;
  } 
  
  bzero((char*) &servAddr, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = INADDR_ANY;
  //servAddr.sin_addr.s_addr = hostname;
  servAddr.sin_port = htons(49628);
 
  
  return 0;   
//return 0 on success, -1 on error and h_errnor set correctly 
}

extern char* request_socket(char *message)
{
  //make a socket
  int sockfd = 0;
  struct sockaddr_in servAddr; 
  int send_message =0;
  
  char * buffer = (char*) calloc(256,sizeof(char));
  socklen_t socketlen = sizeof(struct sockaddr_in); 
  
  bzero((char*) &servAddr, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = INADDR_ANY;
  //bcopy((char*) server->h_addr, (char*) &servAddr.sin_addr.s_addr, server->h_length);
  servAddr.sin_port = htons(49628);
  
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0)
  {
    perror("Error");
    return -1;
  }
  
  
  if(connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
  {
    error ("Error on connecting");
  }
  
  send_message = sendto(sockfd, message,strlen(message), 0, (struct sockaddr *)&servAddr, sizeof(servAddr));
  printf("%d out of %d bytes were sent\n", send_message, (int) strlen(message));
  
  recvfrom(sockfd, buffer, 256, 0, (struct sockaddr *) &servAddr, &(socketlen));
  printf("Received message: %s\n", buffer);

  close(sockfd);
  return buffer; 

}
extern int netopen (const char *pathname, int flags)
{
  int result = 0;
  char *tmp = (char*)calloc(10, sizeof(char)); 
  char buffer[1024];
  char * tmpbuffer = (char*) calloc(1025, sizeof(char));;
  char getflag[10];
  char delim[2] = ",";
  
  if(flags<0 || flags>2){
    printf("Error Invalid flag\n");
    return -1;
  }

  //1 stands for open 
  sprintf(getflag, "%d", flags);  
  bzero(buffer, 1024);
  strcat(buffer,"1,");
  strcat(buffer, pathname);
  strcat(buffer,",");
  strcat(buffer, getflag);
 
  
  
  bzero(tmpbuffer, 1024); 
  tmpbuffer = (request_socket(buffer));
  tmp = strtok(tmpbuffer,delim);
  result = atoi(tmp); 
  
  if(strlen(tmpbuffer)>2 && result == -1){
    tmp = strtok(NULL, delim);
    errno = atoi(tmp);
    
    perror("error inside netopen\n");
    free(tmpbuffer);
    
    return -1; 
  }else {
    result = atoi(tmpbuffer);
  }
  
  
 
  free(tmpbuffer);
  return result; 

}

ssize_t netread(int fildes, void*buf, size_t nbyte)
{
  int i, result= 0;
  char buffer[1024];
  char fd[5];
  char * size = (char*) calloc( 10, sizeof(char));
  bzero(buffer, 1024); 
  char  *tempbuffer  = (char*) calloc(1024, sizeof(char));
  char *temp2buffer = (char*) calloc(1024, sizeof(char));
  char delim[2] = ",";
  char *tempchar;
  char *tmp =(char*)calloc(10,sizeof(char)); 
  
  
  
  sprintf(fd, "%d", fildes);
  sprintf(size, "%d", nbyte);
  strcat(buffer,"2,");
  strcat(buffer, fd);
  strcat(buffer,",");
  strcat(buffer, size);
  printf("buffer inside netread\n");
  printf("%s\n", buffer);
  
  bzero(tempbuffer, 1024); 
  tempbuffer = (request_socket(buffer));
 
  //return bytes read
  bzero(buffer, 256);
  
  
  bzero (temp2buffer, 1024);
  
  sprintf(temp2buffer, "%s", tempbuffer);
  size = strtok (tempbuffer, delim); 
 // size = strtok(NULL, delim); 
 
  
  result = atoi(size);
  
  
  if(strlen(tempbuffer)>2 && result == -1){
    tmp = strtok(NULL, delim);
    errno = atoi(tmp);
    
   
    free(tempbuffer);
    free(temp2buffer);
    free(tmp);
    return -1; 
  }else if(result == -1 && strlen(tempbuffer) <= 2)
  {
    
    free(tempbuffer);
    free(temp2buffer);
    free(tmp);
    return -1; 
  }
  

  bzero(buf, strlen(buf));
  int sizelen = strlen(size); 
  
  sprintf(buf,"%s",temp2buffer+(sizelen+2));
  
  
  free(tempbuffer);
  free(temp2buffer);
  free(tmp);
  return result; 
}



ssize_t netwrite(int fildes, void*buf, size_t nbyte)
{
  int i, result= 0;
  char buffer[1024];
  char fd[5];
  char * size = (char*) calloc( 10, sizeof(char));
  bzero(buffer, 1024); 
  char  *tempbuffer  = (char*) calloc(1024, sizeof(char));
  char *temp2buffer = (char*) calloc(1024, sizeof(char));
  char delim[2] = ",";
  char *tempchar;
  char *tmp = (char*) calloc(10,sizeof(char));;
  
  bzero(tempbuffer, 1024); 
  sprintf(fd, "%d", fildes);
  sprintf(size, "%d", nbyte);
  sprintf(tempbuffer, "%s",buf);
  strcat(buffer,"3,");
  strcat(buffer, fd);
  strcat(buffer,",");
  strcat(buffer, size);
  strcat(buffer,",");
  strcat(buffer, tempbuffer);
  printf("buffer inside netwrite\n");
  printf("%s\n", buffer);
  
  bzero(temp2buffer, 1024); 
  temp2buffer = (request_socket(buffer));
 
  //return bytes write
  bzero(buffer, 256);
  bzero (tempbuffer, 1024);
  
  sprintf(tempbuffer, "%s", temp2buffer);
  size = strtok (tempbuffer, delim); 
  
  result = atoi(size);
  
  if(strlen(temp2buffer)>2 && result == -1){
    tmp = strtok(NULL, delim);
    errno = atoi(tmp);
    
    
    free(tempbuffer);
    free(temp2buffer);
    free(tmp);
    return -1; 
  }else if(result == -1 && strlen(tempbuffer) <= 2)
  {
    
    free(tempbuffer);
    free(temp2buffer);
    free(tmp);
    return -1; 
  }
  
  
  free(tempbuffer);
  free(temp2buffer);
  free(tmp);
  
  return result; 
}


int netclose(int fd)
{
	int result =0;
	char buffer[256];
  char *tmpbuffer = (char*) calloc(256, sizeof(char));
  
  
	bzero(buffer, 256);
	bzero(tmpbuffer, 256);
	sprintf(tmpbuffer, "%d", fd);
 
  strcat(buffer,"4,");
  strcat(buffer, tmpbuffer);
  
  bzero(tmpbuffer, 256);
	tmpbuffer = (request_socket(buffer));
	result = atoi(tmpbuffer);
   if(result == -1)
   {
     errno = EBADF;
     free(tmpbuffer);
     return -1; 
   }
  
  free(tmpbuffer);
	return result; 

	
}


