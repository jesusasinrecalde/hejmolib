 #ifndef _COMANDO_H_
 #define _COMANDO_H_
 
 #include "structDatos.h" 
 
 typedef enum
 {
	CMD_SETVALORFINAL,
	CMD_GETDATA ,
	NUM_COMMAND
 }eCommand;
 
 
 typedef struct
 {
	eCommand Comando;
	int Contador;
	int ValorFinal;
 }stComand;
 
 // estructura de respuesta 
 typedef struct
 {
	eEstado estado;
	int Contador;
	int ValorFinal;
 }stRespuesta;
 #endif
