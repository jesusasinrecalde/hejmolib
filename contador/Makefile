CC = gcc
CFLAGS = -DHAVE_CONFIG_H -g -O2
INCLUDE = -I. 
SRC= contador_servidor.c
OBJ= contador_servidor.o servidorSocket.o structDatos.o hilocontador.o
LD = gcc
LDFLAGS = -g -O2
LIBS = -lpthread

contador_servidor: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

contador_servidor.o : contador_servidor.c
	$(CC) $(CLFAGS) $(INCLUDE)  -c $< -o $@

servidorSocket.o : servidorSocket.c
	$(CC) $(CLFAGS) $(INCLUDE)  -c $< -o $@

structDatos.o : structDatos.c
	$(CC) $(CLFAGS) $(INCLUDE)  -c $< -o $@

hilocontador.o : hilocontador.c
	$(CC) $(CLFAGS) $(INCLUDE)  -c $< -o $@


clean :
	rm contador_servidor *.o
