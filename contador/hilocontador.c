
#include "hiloContador.h"
#include "structDatos.h"

extern stDatos Datos;
void hiloContador( void );

void LanzaHiloContador(void)
{
	pthread_create(&(Datos.hilo),NULL,(void*)hiloContador,NULL);
}


/** Resumen de estados ....
 * 	ST_NOACTIVO estado inicial sin datos no implica proceso alguno se modificara en caso de paso a pause o tener datos que contar 
 *  ST_ACTIVO estado procesando hay datos que contar y no se ha llegado al igual el dato final con el contador
 *  ST_END los valores dato final y dato contador se han igualado
 *  ST_PAUSE se suspende el proceso de conteo sea cual sea el estado en el que este
 * */ 
void hiloContador( void )
{
		do 
		{
			BloqueaDatos(&Datos);
			
			if(Datos.estado!=ST_PAUSE)
			{	
				printf("%d %d %d %d \n",Datos.estado,Datos.ContadorMinuto,Datos.valor_final,Datos.Contador);
				if(Datos.valor_final==0 && Datos.Contador == 0)
				{
					Datos.estado = ST_NOACTIVO;
				}
				
				
				printf(" %d %d \n",Datos.valor_final,Datos.Contador);
				if(Datos.valor_final>Datos.Contador)
				{
					Datos.estado=ST_ACTIVO;
				}
				else if( Datos.valor_final && Datos.Contador && Datos.valor_final <= Datos.Contador)
					Datos.estado=ST_END;
			
				if(Datos.estado==ST_ACTIVO)
				{
					//Datos.Contador++;
					Datos.ContadorMinuto++;	
					if(Datos.ContadorMinuto >=60) // ha llegado a la hora 
					{
						Datos.Contador++; // Avanzamos la hora 
						Datos.ContadorMinuto=0;
					}
					printf("Datos ...  estado %d , Minuto %d contador %d  valor final %d  \n",Datos.estado,Datos.ContadorMinuto,Datos.Contador,Datos.valor_final);
				}
			}
			printf("Estado  ",Datos.estado);
			
			switch(Datos.estado)
			{
				case ST_NOACTIVO:
					printf("ST_NOACTIVO \n");
					break;
				case ST_ACTIVO:
					printf ("ST_ACTIVO \n");
					break;
				case ST_PAUSE:
					printf("ST_PAUSE  \n");
					break;
				case ST_END:
					printf ("ST_END \n");
					break;
				default :
					printf (" %d \n",Datos.estado);
					break;
					
			}
			DesbloqueaDatos(&Datos);
			sleep(60);// el proceso se activa cada minuto 
		}
		while(1);
}
