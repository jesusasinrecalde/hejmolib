#ifndef _SERVIDOR_SOCKET_H_
#define _SERVIDOR_SOCKET_H_

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>   

typedef enum 
{
	SCK_CONTINUE,
	SCK_FIN
}eProcessSocket;


// descriptor de socket 
typedef struct
{
	struct sockaddr_in sin;
    struct sockaddr_in pin;
	int sock_descriptor;
	int port;
}sck_desc; 


int ConexionSocket(  sck_desc  * fd , int port );
void ServidorSocket( sck_desc  * fd , eProcessSocket (*fncprt)(  int sock_fd, u_int8_t * buffer));


#endif
