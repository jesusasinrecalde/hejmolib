#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "command.h"
#define ID_DEVICE "5"


typedef struct Datos
{
	char Nombre[255];
	char Valor[255];
} Dato;
 






int ProcesaLinea(char * bufferIn, char * ParametroOut, char * ParaDataOut);
int DarDatoParametro(Dato * tablaParam,int NumParam ,char * parametro);

int GrabaParam(char * textParam,eCommand comando, Dato* TablaParam, int NumElmTablaParam);
int EnviaComando( stComand  * comando);

int main (int ac, char *ag[])
{
  int len;

  char * ptr;
  char FileName[255];
  char bufferIn[512];
  char Parametro[512];
  char ParamData[512];
  FILE * fp;
  Dato tablaParam[12];
  int IndiceTabla=0;
  int Indice=0;
  char parametro[255];
 
 
  if (ac != 3)
  {
    printf ("usage: %s Id_hejmo filename", ag[0]);
	exit(-1);
  }
  
   strcpy(FileName,ag[2]);
    
  fp=fopen(FileName,"r");
  if(!fp )
  {	
	  printf("fallo apertura fichero %s \n",FileName);
	  exit(-2);
  }
  // carga el fichero en un a tabla de objetos  nombre-valor 
  while(!feof(fp))
  {
	  fgets(bufferIn,511,fp); // lectura de linea 
	
	  if(ProcesaLinea(bufferIn,tablaParam[IndiceTabla].Nombre,tablaParam[IndiceTabla].Valor))
	  {
		  if(tablaParam[IndiceTabla].Nombre[0]==ag[1][0]) // si La linea pertenece al IdHejmo Correspondiente , se incluye  en la tabla  
		  {
			IndiceTabla++;
		  
		  }
		  
	  }
	  
  }// fin procesamiento del fichero 
  printf(" indiceTabla %d \n",IndiceTabla);
  fclose(fp);
  // en IndiceTabla se encuentra el numero de parametros  cargados en la tabla 
  

  // la relacion de parametros es 
  /*
		dat1   ESTADO                 segun los datos del tipo ecommand
		dat2   Contador               (**) no se actualiza el valor y si es numerico si
		dat3   ValorFinal             (**) no se actuaiza el valor y si es numerico si
		
		
  */

  int pos=0;
  
  sprintf(parametro,"%s_dat1",ag[1]);
  GrabaParam(parametro, CMD_SETESTADO,  tablaParam, IndiceTabla);
  
  sprintf(parametro,"%s_dat2",ag[1]);
  GrabaParam(parametro, CMD_SETVALORFINAL,  tablaParam, IndiceTabla);
 
 sprintf(parametro,"%s_dat3",ag[1]);
  GrabaParam(parametro, CMD_SETCONTADOR,  tablaParam, IndiceTabla);
    
  
  return 0;
}



 
 
 int GrabaParam(char * textParam,eCommand comando, Dato* TablaParam, int NumElmTablaParam)
 {
   int retorno = 0;
   int pos;
/*   typedef struct
 {
	eCommand Comando;
	int Contador;
	int ValorFinal;
 }
 */
 stComand comandoAEnviar;
  pos=DarDatoParametro(TablaParam,NumElmTablaParam,textParam);
  
  printf(" pos %d \n",pos);
  if(pos!=-1 &&  strcmp(TablaParam[pos].Valor,"**"))
  {
	printf(" ....> %d \n",comando);
	comandoAEnviar.Comando=comando;
	switch(comando)
	{
		case CMD_SETVALORFINAL :
			printf(" comando SET VALOR FINAL a enviar [%d] \n",atoi(TablaParam[pos].Valor));
			comandoAEnviar.ValorFinal=atoi(TablaParam[pos].Valor);
			break;
		case CMD_SETESTADO :
			printf(" comando a enviar %s %d \n",TablaParam[pos].Valor,atoi(TablaParam[pos].Valor));
			comandoAEnviar.estado=(eEstado)atoi(TablaParam[pos].Valor);
			break;
		case CMD_SETCONTADOR :
			printf(" comando SET contador a enviar [%d] \n",atoi(TablaParam[pos].Valor));
			comandoAEnviar.Contador=atoi(TablaParam[pos].Valor);
			break;
	}
	retorno= EnviaComando( &comandoAEnviar);
	if(retorno)
	{
		printf("Error en el envio del comando %d , (%d)\n",(int)comando,retorno);
	}
	
	
  }
  return retorno;
 }
 
 
 // --------------------------------------------------------------------------------------------------------------
/** Procesa una linea en una estructura JSON 
*@param bufferIn buffer de entrada 
*@param ParametroOut buffer donde se almacenará el nombre del parametro
*@param ParaDataOut  buffer donde se almacenara el dato
*@return 0 si hay error 1 si hay exito 
*/
int ProcesaLinea(char * bufferIn, char * ParametroOut, char * ParaDataOut)
{
	int longitudLinea=strlen(bufferIn);
	int Estado=0;
	int indice=0;
	int retorno = 1;
	int Indx=0;
	for(indice=0;indice<longitudLinea  && (Estado!=4 ||Estado!=-1); indice++)
	{
		switch(Estado)
		{
			case 0 :
				
				if(bufferIn[indice]=='"')
				{
					Indx=0;
					Estado=1;
				}
				break;
			case 1 : // lectura de nombre de parametro
				
				if(bufferIn[indice]=='"')
					Estado=2;
				else
				{
					ParametroOut[Indx]=bufferIn[indice];
					ParametroOut[Indx+1]=0;
					Indx++;
					
				}
				break;
			case 2 :
				
				if(bufferIn[indice]=='"')
				{
					Indx=0;
					Estado=3;
				}
				break;
			case 3 :
				
				if(bufferIn[indice]=='"')
					Estado=4;
				else
				{
					ParaDataOut[Indx]=bufferIn[indice];
					ParaDataOut[Indx+1]=0;
					Indx++;
					
				}
				break;
			case 4 :
				break;
			default :
				
				Estado=-1;
				break;
			
		}
	}
	
	if(Estado != 4)
		retorno=0;
	
	
	return retorno;
}

int  DarDatoParametro(Dato * tablaParam,int NumParam ,char * parametro)
{
	int  retorno=-1;
	int Indice=0;
	
	for(Indice=0;Indice<NumParam ||retorno!=-1;Indice++)
	{
		if(!strcmp(parametro,tablaParam[Indice].Nombre))
		{
			
			retorno=Indice;
			
			break;
		}
	}

	return retorno;
}


int EnviaComando( stComand  * comando)
{
	
   stRespuesta respuesta;
   int fd, numbytes;       
   int idHejmo=0;
   int valor;
   /* ficheros descriptores */


   struct hostent *he;         
   /* estructura que recibirá información sobre el nodo remoto */

   struct sockaddr_in server;  
   /* información sobre la dirección del servidor */

   if ((he=gethostbyname("127.0.0.1"))==NULL){       
	  perror("gethostbyname");
      return -1;
   }

   if ((fd=socket(AF_INET, SOCK_STREAM, 0))==-1){  
	   perror("socket");
       return -2 ;
   }

   bzero(&server,sizeof(server));	
   server.sin_family = AF_INET;
   server.sin_addr.s_addr = htonl (INADDR_ANY);
   server.sin_addr.s_addr = ((struct in_addr* ) (he->h_addr))->s_addr;
   server.sin_port = htons(PORT); 
   /* htons() es necesaria nuevamente ;-o */
   //server.sin_addr = *((struct in_addr *)he->h_addr);  
   /*he->h_addr pasa la información de ``*he'' a "h_addr" */
   //bzero(&(server.sin_zero),8);

   if(connect(fd, (struct sockaddr *) &server, sizeof(server))==-1){ 
     perror("connect");
     return -2 ;
   }

   printf("envia comando \n");
	valor =send(fd,comando,sizeof(stComand),0);
	
	
	//{
	//	perror("send");
	//	return -3;
	//}
	

   close(fd);   /* cerramos fd =) */
   return 0;

}
