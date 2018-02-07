
/*
    EIB Demo program - send A_GroupValue_Write
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
#include "common.h"


#define F16_MANTISA_MAX   2047
#define F16_MANTISA_MIN  -2048
uint16_t ConvertFloat32ToKNX ( float valorin );
int GrabaParametroTempKNX(char * SocketURL, char * KNXaddr, char * data);

int
main (int ac, char *ag[])
{
  int len;
  EIBConnection *con;
  eibaddr_t dest;
  uchar buf[255] = { 0, 0x80 };

  if (ac < 4)
    die ("usage: %s url eibaddr val val ...", ag[0]);
  

  
  if(!GrabaParametroTempKNX(ag[1],ag[2],ag[3]))
  {
	die ("Request failed");
  }
 
  return 0;
}

#define F16_MANTISA_MAX   2047
#define F16_MANTISA_MIN  -2048
uint16_t ConvertFloat32ToKNX ( float valorin )
{

        float valorf32=valorin;

        uint16_t f16exponente=0;
        uint16_t f16mantisa=0;
        uint16_t fltInt16;

        uint16_t f16signo=0;

        if(valorf32<0)
        {
                f16signo=0x8000;
                valorf32*=-1;
        }
        int f16valor=valorf32*100;
        if(f16valor<=F16_MANTISA_MAX)
        {
                f16mantisa=f16valor;
                fltInt16= f16signo& f16mantisa;
        }
        else
        {

        unsigned int  mult=1;
                unsigned int  cociente;
                do
                {
                        f16exponente++;
                        mult*=2;
                        cociente=(unsigned int)f16valor/mult;
                }
                while(cociente>F16_MANTISA_MAX);
                fltInt16  = f16signo;
                fltInt16 |=  f16exponente<<11;
                fltInt16 |=  (uint16_t)cociente;
      }


        return fltInt16;

}


int GrabaParametroTempKNX(char * SocketURL, char * KNXaddr, char * data)
{
	int retorno = 0;
	EIBConnection *con;
    eibaddr_t dest;
    uchar buf[255] = { 0, 0x80 };
    con = EIBSocketURL (SocketURL);
    if (con)
	{
      dest = readgaddr (KNXaddr);
 	  uint16_t resultado=ConvertFloat32ToKNX(atof(data));
      uchar * ptr=(uchar *)&resultado;
      buf[2] =ptr[1];
      buf[3] =ptr[0];
      int len=2; 
      if (EIBOpenT_Group (con, dest, 1) != -1)
	  {
		len = EIBSendAPDU (con, 2 + len, buf);
		if (len != -1)
		   retorno=1;
		EIBClose (con);
	  }
	}
    return retorno;
}

