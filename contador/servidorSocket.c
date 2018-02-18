

   
#include "servidorSocket.h"



void ServidorSocket( sck_desc  * fd , eProcessSocket (*fncprt)(  int sock_fd, u_int8_t * buffer))
{
	eProcessSocket process=SCK_CONTINUE;
	 int temp_sock_descriptor;
	 u_int8_t buf[1048*10];
	 int address_size;
	 while(process==SCK_CONTINUE)
	 {
			temp_sock_descriptor = accept(fd->sock_descriptor, (struct sockaddr *)&(fd->pin), &address_size);
			if (temp_sock_descriptor == -1) {
				perror("call to accept");
				exit(1);
			}


	
			if (recv(temp_sock_descriptor, buf, 16384, 0) == -1) {
					perror("call to recv");
					exit(1);
			}		
			process=fncprt(temp_sock_descriptor,buf);
	}
	close(temp_sock_descriptor);
    // for this server example, we just convert the
    // characters to upper case:

    //len = strlen(buf);
    //for (i=0; i<len; i++) buf[i] = toupper(buf[i]);
    
    //if (send(temp_sock_descriptor, buf, len, 0) == -1) {
    //  perror("call to send");
    //  exit(1);
    //}

    close(temp_sock_descriptor);

 
}


int ConexionSocket(  sck_desc  * fd , int port )
{

  struct sockaddr_in sin;
  struct sockaddr_in pin;
  int sock_descriptor;
  int temp_sock_descriptor;
  int address_size;
  
  int i, len;

  fd->sock_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (fd->sock_descriptor == -1)
   {
    perror("call to socket");
	sock_descriptor=-1;
  }
  else
  {

	bzero(&(fd->sin), sizeof(fd->sin));
	fd->sin.sin_family = AF_INET;
	//sin.sin_addr.s_addr = INADDR_ANY;
	fd->sin.sin_port = inet_addr("127.0.0.1"); // solo se dejara conectar de manera local
	fd->sin.sin_port = htons(port);

	if (bind(fd->sock_descriptor, (struct sockaddr *)&(fd->sin), sizeof(fd->sin)) == -1) 
	{
		perror("call to bind");
		fd->sock_descriptor=-1;
	}
    else
    {
		  if (listen(fd->sock_descriptor, 20) == -1) 
		  {
			perror("call to listen");
			fd->sock_descriptor=-1;
		  }

	}
   }

   return fd->sock_descriptor;
	
}
