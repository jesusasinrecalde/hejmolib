 #ifndef _COMANDO_H_
 #define _COMANDO_H_
 
 #include "structDatos.h" 
 
 typedef enum
 {
	CMD_SETVALORFINAL,
	CMD_GETDATA ,
	CMD_SETESTADO,
	CMD_SETCONTADOR,
	NUM_COMMAND
 }eCommand;
 
 
 typedef struct
 {
	eCommand Comando;
	eEstado estado;
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
 
 #define PORT 8000
 #endif
