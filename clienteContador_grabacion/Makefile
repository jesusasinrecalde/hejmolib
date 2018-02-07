CC = g++
CFLAGS = -DHAVE_CONFIG_H  -O2 -MD -MP -fpermissive
INCLUDE =  -I.  -I../contador
SRC = main.c
OBJ =  main.o
LIBS = 
LD = gcc
LDFLAGS =  -O2

clieneteContador_grabacion : $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS) 

main.o : main.c
	$(CC) $(CFLAGS) $(INCLUDE) -MT $@  -c $< -o $@ 

clean : 
	rm clienteContador_grabacion *.o


