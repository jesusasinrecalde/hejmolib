/*
    EIB Demo program - reads group telegrams
    Copyright (C) 2005-2011 Martin Koegler <mkoegler@auto.tuwien.ac.at>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "groupreadresponse.h"

typedef unsigned char bool;
#define TRUE  1
#define FALSE 0

/** Obtener el dato de lectura solicitado
	*@return -1 si se ha producio error, 0 si no llegado el dato, en cualquier otro valor es la longitud de los datos  almacenados en el buffer
	*@param con estructura de  conexion
	*@buf buffer donde se almacena  el resultado tiene que estar allocado
	*@src direccion destino
*/ 
int ObtenerDatoKNX(EIBConnection * con, uchar * buf,eibaddr_t * src);

EIBConnection * RealizarPeticionKNX(char * KNXAddrGrp);

int LeerParametroKNX( char * KNXAddrGrp, char * buffer );

int ObtenerDatoKNX(EIBConnection * con, uchar * buf,eibaddr_t * src)
{
	uchar buf_temp[200];
	int retorno = -1;
	int len = EIBGetAPDU_Src (con, sizeof (buf_temp), buf_temp, src);
	if(len >= 2 )
	{
   
		if(buf_temp[1] & 0XC0 )
		{
       
			 if (len == 2)
			 {
        
			 	retorno=1;
			 	buf[0]=buf_temp[1]&0x3F;

			 }
	      else
		    {
            
    			memcpy((void *)buf,(void *)buf_temp+2,len-2);
				retorno=len-2;
			 }

		}
		
	}
	else
		retorno = 0;
	
	return retorno;
}


EIBConnection * RealizarPeticionKNX(char * KNXAddrGrp)
{
   EIBConnection * con= EIBSocketURL ("ip:localhost");
   eibaddr_t dest;
   int len;
   uchar req_buf[2] = { 0, 0 };
   if (con)
   {
  	dest = readgaddr (KNXAddrGrp);
    
    if (EIBOpenT_Group (con, dest, 0) != -1)
    {
      
      len = EIBSendAPDU (con, 2, req_buf);
      
  	  if (len == -1)
  	  {
        
  	  	EIBClose (con);
  	  	con=NULL;
  	  }
  	}
  	else
  	{
      
  		EIBClose (con);
  		con=NULL;
    }

  }
  
  return con;
  
}

int LeerParametroKNX( char * KNXAddrGrp, char * buffer )
{
  int retorno = -1;
  int len;
  EIBConnection *con;
  fd_set read;
  eibaddr_t dest;
  
  eibaddr_t src;
  uchar req_buf[2] = { 0, 0 };
  uchar buf[200];
  struct timeval tv;
  

  con = RealizarPeticionKNX(KNXAddrGrp);
  if(con==NULL)
    return -1 ;
  

  bool  flgFinBucle=FALSE;
  bool  flgResetTimeout=TRUE;
  while (!flgFinBucle)
    {
      if(flgResetTimeout)
      {
        tv.tv_usec=0;
        tv.tv_sec=10; // 10 segundos de timeout de espera de parametros 
        flgResetTimeout=FALSE;
        
      }//  if(flgResetTimeout)

    
     
      FD_ZERO (&read);
      FD_SET (EIB_Poll_FD (con), &read);

      if (select (EIB_Poll_FD (con) + 1, &read, 0, 0, &tv) == -1)
      {
       flgFinBucle=TRUE;
       printf("3\n"); 
      }
     else
     {
        
        len = EIB_Poll_Complete (con);
  
        if (len == -1)// se ha producido un error
        {
         
         flgFinBucle=TRUE;
        }
        else if (len == 0)
       {
        
        if(tv.tv_sec == 0 && tv.tv_usec==0)
          flgFinBucle=TRUE;
        }
      else
        flgResetTimeout=TRUE;

      
      len = ObtenerDatoKNX( con,  buf, &src);
  
      if( len >0 )
      {
        
        memcpy(buffer,buf,len);
        retorno=len;
       //printf("EL VALOR ES  : ");
       // printHex (len , buf );
       //printf("\n");
       flgFinBucle=TRUE;
      }
    } // else if (select (EIB_Poll_FD (con) + 1, &read, 0, 0, &tv) == -1)
  } // while (!flgFinBucle)
  
  
   

  EIBClose (con);
  return retorno; 


}// LeerParametroKNX


