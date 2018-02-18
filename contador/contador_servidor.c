/* server.c
 * Copyright Mark Watson 1999. Open Source Software License.
 */

#include <stdlib.h>
#include <stdio.h>
#include "servidorSocket.h"
#include "command.h"
#include "structDatos.h"
#include "hiloContador.h"
#include <signal.h>
#include <errno.h>
#include <unistd.h>


stDatos Datos;

eProcessSocket Funcion(int sock_fd , u_int8_t * buffer );

void HndSennal( int sennal );

void main(int arg , char ** argv) {
  
  sck_desc fd_descriptor;
  int port = 8000;
   FILE * fp;
   pid_t pid , sid;
   
   if(arg != 2 )
   {
	   perror(" fallo en los argumentos ( -s -k )\n");
	   exit(EXIT_FAILURE);
   }
   if(!strcmp(argv[1],"-k"))
   {
	   fp = fopen("/tmp/hejmo/contador.pid","r");
	   if(fp)
	    {
			system("kill `cat /tmp/hejmo/contador.pid`");
			exit(EXIT_SUCCESS);
		}
		else
		{
			perror(" programa no lanzado \n");
			exit(EXIT_FAILURE);
		}
   }
   
   if(strcmp(argv[1],"-s"))
   {
	   perror(" parametro incorrecto \n");
	   exit(EXIT_FAILURE);
    }
   
   pid = fork();
   if(pid<0 )
   {
	perror("fork");
	exit(EXIT_FAILURE);
	}	
   if(pid>0) // llamada padre 
   {
	   exit(EXIT_SUCCESS);
    }
   
   fp = fopen("/tmp/hejmo/contador.pid","r");
   if( fp )
   {
	   perror(" proceso ya en funcionamiento");
	   exit(EXIT_FAILURE);
   }
   
    
   // se esta en la llamada hijo , se situa el directorio de ejecucion el actual  
   if((sid = setsid()) <0 )
   {
	   perror("setsid");
	   exit(EXIT_FAILURE);
	}
	
	fp= fopen("/tmp/hejmo/contador.pid","w");
   fprintf(fp,"%d",sid);
   fclose(fp);
    
   if(chdir("/") <0 )
   {
	   perror("chdir");
	   exit(EXIT_FAILURE);
    }
   
   umask(0); // se restablece el modo archivo
   
  // close(STDIN_FILENO );
  // close (STDOUT_FILENO );
  // close(STDERR_FILENO);
   
   
   
  if( signal(SIGTERM,HndSennal) == SIG_ERR )
  {
		printf("Error al asignar sennal  \n");
  }
  if(!LeeDatosDeFichero(&Datos))
	InicializaDatos(&Datos);
  
  LanzaHiloContador(); 
  ConexionSocket( &fd_descriptor , port );
  ServidorSocket( &fd_descriptor, Funcion);		

} 


eProcessSocket Funcion(int sock_fd , u_int8_t * buffer )
{
	stComand * comando = (stComand * )buffer;
	stRespuesta Respuesta;

    BloqueaDatos(&Datos);
	switch(comando->Comando)
	{
		case CMD_SETVALORFINAL :
			
				Datos.valor_final=comando->ValorFinal;
				//printf("Cambio valor final %d \n",Datos.valor_final);
				printf("Datos %d %d %d %d \n",Datos.estado ,Datos.ContadorMinuto ,Datos.valor_final ,Datos.Contador);
			
			break;
		case CMD_GETDATA : // Devuelve los datos 
				printf("GetData \n");
				Respuesta.estado=Datos.estado;
				Respuesta.Contador=Datos.Contador;
				Respuesta.ValorFinal=Datos.valor_final;
				send(sock_fd,&Respuesta,sizeof(stRespuesta),0);
				break;
		case CMD_SETESTADO :
			Datos.estado=comando->estado;
			printf("SET Estado %d \n",Datos.estado);
			break;
		case CMD_SETCONTADOR :
			Datos.Contador=comando->Contador;
			if(Datos.Contador==0) // implica un reseteo del contador de horas , tambien hay que resetear el de minuto
				Datos.ContadorMinuto=0;
			printf ("SET Contador %d \n",Datos.Contador);
			break;
	}
	DesbloqueaDatos(&Datos);
	return SCK_CONTINUE;
}


void HndSennal ( int Sennal )
{
	GrabaDatosAFichero(&Datos);
	system (" rm /tmp/hejmo/contador.pid");
	exit(0);
}
