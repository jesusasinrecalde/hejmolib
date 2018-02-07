#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <algorithm>

#include "groupreadresponse.h"
#include "common.h"


#define ID_DEVICE "2"

typedef
union
 {
  // float16 v;
  struct
   {
    // type determines alignment!
    uint16_t m:10;
    uint16_t e:5;
    uint16_t s:1;
   } bits;
} float16_s;

typedef
union
 {
  float v;
  struct
   {
    uint32_t m:23;
    uint32_t e:8;
    uint32_t s:1;
   } bits;
 } float32_s;

typedef struct Datos
{
	char Nombre[255];
	char Valor[255];
} Dato;
 




float float16to32(float16_s f16);
float ImprimeTemp( uchar * buf);
void ImprimeParametroTemp(char * Knxgroup, char * hejmoGroup,int idParam);
float ConvertKNXToFloat32(uint16_t  in );
int ProcesaLinea(char * bufferIn, char * ParametroOut, char * ParaDataOut);
int DarDatoParametro(Dato * tablaParam,int NumParam ,char * parametro);

int GrabaParametroKNX(char * knxaddr, char * parametro);
#define F16_MANTISA_MAX   2047
#define F16_MANTISA_MIN  -2048
uint16_t ConvertFloat32ToKNX ( float valorin );
int GrabaParametroTempKNX(char * SocketURL, char * KNXaddr, char * data)

 int uint16readBlock (uchar * buf,  uint16_t data);


int
main (int ac, char *ag[])
{
  int len;
  uint16_t valor;
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
 
  uchar buf[200];
  
 
  uint16_t valor16;

  if (ac != 4)
    die ("usage: %s Id_device Id_hejmo filename", ag[0]);

  
  strcpy(FileName,ag[3]);
  
  fp=fopen(FileName,"r");
  if(!fp )
	  die("fallo apertura fichero %s \n",FileName);
  
  // carga el fichero en un a tabla de objetos  nombre-valor 
  while(!feof(fp))
  {
	  fgets(bufferIn,511,fp); // lectura de linea 
	   
	  if(ProcesaLinea(bufferIn,tablaParam[IndiceTabla].Nombre,tablaParam[IndiceTabla].Valor))
	  {
		  if(tablaParam[IndiceTabla].Nombre[0]==ag[2][0]) // si La linea pertenece al IdHejmo Correspondiente , se incluye  en la tabla  
		  {
			IndiceTabla++;
		  
		  }
		  
	  }
	  
  }// fin procesamiento del fichero 
  fclose(fp);
  // en IndiceTabla se encuentra el numero de parametros  cargados en la tabla 
  

  // la relacion de parametros es 
  /*
		dat1   ACS_ON_OFF             1/0
		dat2   ACS_ON_OFF_RESISTENCIA 1/1
		dat3   ACS_TEMPERATURA        1/2
		dat4   CLIMA_ON_OFF           1/3
		dat5   CLIMA_MODO_FUNC        1/4
		dat6   CLIMA_TEMP_CONSIGNA    1/5
		
  */

  int pos=0;
  sprintf(parametro,"%s_dat1",ag[2]);
  pos=DarDatoParametro(tablaParam,IndiceTabla,parametro);
  if(pos!=-1)
  {
	
    if(GrabaParametroKNX("1/0", tablaParam[pos].Valor))
	{
		printf("parametro grabado %s\n",parametro);
	}
	else
	{
	  printf("fallo parametro %s\n", parametro);
	}
  }
  
  sprintf(parametro,"%s_dat2",ag[2]);
  pos=DarDatoParametro(tablaParam,IndiceTabla,parametro);
  if(pos!=-1)
  {
	
    if(GrabaParametroKNX("1/1", tablaParam[pos].Valor))
	{
		printf("parametro grabado %s\n",parametro);
	}
	else
	{
	  printf("fallo parametro %s\n", parametro);
	}
  }
  
  sprintf(parametro,"%s_dat3",ag[2]);
  pos=DarDatoParametro(tablaParam,IndiceTabla,parametro);
  if(pos!=-1)
  {
	
	if( GrabaParametroTempKNX("ip:localhost", "1/2", tablaParam[pos].Valor)) 
	{
		printf("parametro grabado %s\n",parametro);
	}
	else
	{
	  printf("fallo parametro %s\n", parametro);
	}
  }
  
  
  return 0;
}


float float16to32(float16_s f16)
 {
  // back to 32
  float32_s f32;
  f32.bits.s=f16.bits.s;
  f32.bits.e=(f16.bits.e-15)+127; // safe in this direction
  f32.bits.m=((uint32_t)f16.bits.m) << 13;
 
  return f32.v;
 }

 float ImprimeTemp( uchar * buf)
{
  uint16_t valor;
  float salida;
  char * ptr;

  float16_s * Valorfloat = (float16_s *)&valor;

  
  ptr = (char*)&valor;
  
  ptr[0]=buf[1];
  ptr[1]=buf[0];
  salida= float16to32(*Valorfloat);
  return salida*100000;
}

void ImprimeParametroTemp(char * Knxgroup, char * hejmoGroup,int idParam)

{
  int len;
  
 
  uchar buf[200];

  len =  LeerParametroKNX( Knxgroup, buf ); 
  if(len >=2 )
  {
    uint16_t valor;
    uchar * ptr = (uchar* )&valor;
    ptr[0]=buf[1];
    ptr[1]=buf[0]; 
    printf(",\"%s_dat%d\":\"",hejmoGroup,idParam);
    //printf("%.1f\"", ImprimeTemp(buf));
    printf("%.1f\"",ConvertKNXToFloat32(valor));
  }
}





float ConvertKNXToFloat32(uint16_t  in )
{

        double   resultado = pow((double)2,(double)((in&0x7800)>>11))*((double)(in & 0x7FF)*0.01);
        uint16_t signo = in & 0x8000;
        if(signo == 0x8000 )
                resultado = resultado * -1 ;
        return (float ) resultado;

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
int GrabaParametroKNX(char * knxaddr, char * parametro)
{
  int retorno=0;
  int len;
  EIBConnection *con;
  eibaddr_t dest;
  uchar buf[3] = { 0, 0x80 };

  
  con = EIBSocketURL ("ip:localhost");
  if (con)
  {
	  
	dest = readgaddr (knxaddr);
	buf[1] |= readHex (parametro) & 0x3f;

	if (EIBOpenT_Group (con, dest, 1) != -1)
	{
	
		printf("%02X %02X\n",buf[0],buf[1]);
		len = EIBSendAPDU (con, 2, buf);
		if (len != -1)
			retorno = 1;
		
	}
	EIBClose (con);
  }
  return retorno;

}



void float16(uint16_t* __restrict out, const float in) {
        uint32_t inu = *((uint32_t*)&in);
        uint32_t t1;
        uint32_t t2;
        uint32_t t3;

        t1 = inu & 0x7fffffff;                 // Non-sign bits
        t2 = inu & 0x80000000;                 // Sign bit
        t3 = inu & 0x7f800000;                 // Exponent
        
        t1 >>= 13;                             // Align mantissa on MSB
        t2 >>= 16;                             // Shift sign bit into position

        t1 -= 0x1c000;                         // Adjust bias

        t1 = (t3 > 0x38800000) ? 0 : t1;       // Flush-to-zero
        t1 = (t3 < 0x8e000000) ? 0x7bff : t1;  // Clamp-to-max
        t1 = (t3 == 0 ? 0 : t1);               // Denormals-as-zero

        t1 |= t2;                              // Re-insert sign bit

        *((uint16_t*)out) = t1;
    };

void float32to16(float x,float16_s * f16)
 {
  float32_s f32={x}; // c99
 
  // to 16
  f16->bits.s=f32.bits.s;
  f16->bits.e=std::max(-15,std::min(16,(int)(f32.bits.e-127))) +15;
  f16->bits.m=f32.bits.m >> 13;
 }

 int uint16readBlock (uchar * buf,  uint16_t data)
{
  uchar * ptr = (uchar * )& data;
  buf[0]=ptr[1];
  buf[1]=ptr[0];
  
  /*int i = 0;
  while (size - i > 0 && ac > 0)
    {
      buf[i] = readHex (ag[i]);
      i++;
      ac--;
    }
*/
  return 2;
}
