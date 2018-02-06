#ifndef _STRUCT_DATOS_H_
#define _STRUCT_DATOS_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef enum { ST_ACTIVO , ST_NOACTIVO , ST_PAUSE,ST_END } eEstado;

typedef struct
{
	eEstado estado;
	int Contador ;
	int ContadorMinuto;
	int valor_final;
	pthread_mutex_t mutex;
	pthread_t hilo;
	
}stDatos;



void InicializaDatos(stDatos *);

void BloqueaDatos(stDatos * Datos);
void DesbloqueaDatos(stDatos * Datos );
void GrabaDatosAFichero(stDatos * Datos );
int LeeDatosDeFichero(stDatos * Datos );
#endif
