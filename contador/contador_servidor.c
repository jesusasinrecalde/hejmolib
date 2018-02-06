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
stDatos Datos;

eProcessSocket Funcion(int sock_fd , u_int8_t * buffer );

void HndSennal( int sennal );

void main() {
  
  sck_desc fd_descriptor;
  int port = 8000;
   
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

	switch(comando->Comando)
	{
		case CMD_SETVALORFINAL :
			BloqueaDatos(&Datos);
				Datos.valor_final=comando->ValorFinal;
				//printf("Cambio valor final %d \n",Datos.valor_final);
				printf("Datos %d %d %d %d \n",Datos.estado ,Datos.ContadorMinuto ,Datos.valor_final ,Datos.Contador);
			DesbloqueaDatos(&Datos);
			break;
		case CMD_GETDATA : // Devuelve los datos 
			BloqueaDatos(&Datos);
				printf("GetData \n");
				Respuesta.estado=Datos.estado;
				Respuesta.Contador=Datos.Contador;
				Respuesta.ValorFinal=Datos.valor_final;
				send(sock_fd,&Respuesta,sizeof(stRespuesta),0);
			DesbloqueaDatos(&Datos);
			break;
	}
	printf(">>>>> %d \n",comando->Comando);
	return SCK_CONTINUE;
}


void HndSennal ( int Sennal )
{
	printf("hola soy una sennal \n");
	GrabaDatosAFichero(&Datos);
	exit(0);
}
