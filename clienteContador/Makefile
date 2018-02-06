CC = gcc
CFLAGS = -DHAVE_CONFIG_H -g -O2
INCLUDE = -I. -I../contador
SRC= cliente.c
OBJ= cliente.o 
LD = gcc
LDFLAGS = -g -O2
LIBS = 

clienteContador: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

cliente.o : cliente.c
	$(CC) $(CLFAGS) $(INCLUDE)  -c $< -o $@

install : 
	cp clienteContador /home/pi/hejmo/modulos 

clean :
	rm clienteContador *.o
