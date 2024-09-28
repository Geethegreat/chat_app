#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <winsock2.h>
#include <unistd.h> // read(), write(), close()
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)  
#pragma comment(lib, "ws2_32.lib")
#include <pthread.h>
