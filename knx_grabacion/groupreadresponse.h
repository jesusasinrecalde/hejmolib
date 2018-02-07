// ==============================================================================================================
// Hejmo (c) 2016 
//
// ==============================================================================================================

#ifndef GROUPREADRESPONSE_H_
#define GROUPREADRESPONSE_H_

/** Lectura de un parametro usando protocolo KNX
	*@param KNXAddGrp direccion del parametro de lectura ( pe "1/12")
	*@param buffer buffer de recepcion del parametro ya tiene que estar reservado la memorida del mismo 
	*@return -1 si se ha producido un error en caso contrario la longitud del parametro recibido ( 0 significa que no se ha podido leer parametros)
*/	 
int LeerParametroKNX( char * KNXAddrGrp, char * buffer );

#endif