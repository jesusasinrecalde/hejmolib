#include "structDatos.h"
#include <stdio.h>

void InicializaDatos(stDatos * Datos)
{
	Datos->estado=ST_NOACTIVO;
	Datos->Contador=0;
	Datos->valor_final=0;
	Datos->ContadorMinuto=0;
	pthread_mutex_init(&(Datos->mutex),NULL);
	
}

void BloqueaDatos(stDatos * Datos)
{
	pthread_mutex_lock(&(Datos->mutex));
}

void DesbloqueaDatos(stDatos * Datos )
{
	pthread_mutex_unlock(&(Datos->mutex));
}


void GrabaDatosAFichero(stDatos * Datos )
{
		FILE * fp;
		fp = fopen("/home/pi/hejmo/conf/contador.dat" ,"w");
		fwrite(Datos,sizeof(stDatos),1,fp);
		fclose(fp);
}

int LeeDatosDeFichero(stDatos * Datos )
{
		int retorno = 0;
		FILE * fp;
		fp = fopen("/home/pi/hejmo/conf/contador.dat" ,"r");
		if(fp != NULL )
		{
			printf("Lee Datos  \n");
			fread(Datos,sizeof(stDatos),1,fp);
			fclose(fp);
			retorno =1 ;
		}
		return retorno;
}
