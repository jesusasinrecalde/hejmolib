
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>     
#include "command.h"   
/* netbd.h es necesitada por la estructura hostent ;-) */

#define PORT 8000
/* El Puerto Abierto del nodo remoto */

#define MAXDATASIZE 100   
/* El número máximo de datos en bytes */

int main(int argc, char *argv[])
{
	
   stRespuesta respuesta;
   int fd, numbytes;       
   int idHejmo=0;
   /* ficheros descriptores */

   char buf[MAXDATASIZE];  
   /* en donde es almacenará el texto recibido */

   struct hostent *he;         
   /* estructura que recibirá información sobre el nodo remoto */

   struct sockaddr_in server;  
   /* información sobre la dirección del servidor */

  if (argc !=2) { 
      /* esto es porque nuestro programa sólo necesitará un
      argumento, (la IP) */
      printf("Uso: %s <id_hejmo>\n",argv[0]);
      exit(-1);
   }
   idHejmo=atoi(argv[1]);
   
   printf("\"ID_%d\":\"5\"",idHejmo);

   if ((he=gethostbyname("127.0.0.1"))==NULL){       
      /* llamada a gethostbyname() */
      //printf("gethostbyname() error\n");
      printf(",\"%d_dat1\":\"-100\"",idHejmo);
      exit(-1);
   }

   if ((fd=socket(AF_INET, SOCK_STREAM, 0))==-1){  
      /* llamada a socket() */
      //printf("socket() error\n");
       printf(",\"%d_dat1\":\"-101\"",idHejmo);
      exit(-1);
   }

   server.sin_family = AF_INET;
   server.sin_port = htons(PORT); 
   /* htons() es necesaria nuevamente ;-o */
   server.sin_addr = *((struct in_addr *)he->h_addr);  
   /*he->h_addr pasa la información de ``*he'' a "h_addr" */
   bzero(&(server.sin_zero),8);

   if(connect(fd, (struct sockaddr *)&server,
      sizeof(struct sockaddr))==-1){ 
      /* llamada a connect() */
      //printf("connect() error\n");
       printf(",\"%d_dat1\":\"-102\"",idHejmo);
      exit(-1);
   }

   //if ((numbytes=recv(fd,buf,MAXDATASIZE,0)) == -1){  
   //   /* llamada a recv() */
   //   printf("Error en recv() \n");
   //   exit(-1);
   //}
   
   //stComand comando;
   //comando.Comando=CMD_SETVALORFINAL;
   //comando.ValorFinal=10;
   //if(send(fd,&comando,sizeof(stComand),0)==-1)
   //{
	//	printf("error en send\n");
	//	exit(-1);
	//}
	
	stComand comando;
	comando.Comando=CMD_GETDATA;
	if(send(fd,&comando,sizeof(stComand),0)==-1)
	{
		//printf("error en el send \n");
		 printf(",\"%d_dat1\":\"-103\"",idHejmo);
		exit(-1);
	}
	
	
	if (recv(fd, &respuesta, sizeof(stRespuesta), 0) == -1)
	{
		//printf("error al recv \n");
		 printf(",\"%d_dat1\":\"-104\"",idHejmo);
		 exit(-1);
	}
	
	printf(",\"%d_dat1\":\"%d\"",idHejmo,respuesta.estado);
	printf(",\"%d_dat2\":\"%d\"",idHejmo,respuesta.Contador);
	printf(",\"%d_dat3\":\"%d\"",idHejmo,respuesta.ValorFinal);
	
	exit(0);
	//printf(" %d %d %d \n",respuesta.estado,respuesta.Contador,respuesta.ValorFinal);

   //buf[numbytes]='\0';

   //printf("Mensaje del Servidor: %s\n",buf); 
   /* muestra el mensaje de bienvenida del servidor =) */

   close(fd);   /* cerramos fd =) */

}
