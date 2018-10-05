#ifndef _LIBNETFILES_H_
#define _LIBNETFILES_H_



#include <stdlib.h>


extern int netserverinit (char *hostname);
extern char* request_socket(char *message);
extern int netopen (const char *pathname, int flags); 




#endif 