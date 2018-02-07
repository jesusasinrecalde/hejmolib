CC = g++
CFLAGS = -DHAVE_CONFIG_H -g -O2 -MD -MP -fpermissive
INCLUDE =  -I. -I/home/pi/domotica/EIB/bcusdk-0.0.5/common -I/home/pi/domotica/EIB/bcusdk-0.0.5/eibd/include 
SRC = common.c groupreadresponse.c main.c
OBJ = common.o groupreadresponse.o main.o
LIBS = ./libs/libeibclient.so -lm
LD = gcc
LDFLAGS = -g -O2

KNX_KLIC_DA_LT_grab : $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS) 

groupreadresponse.o : groupreadresponse.c
	$(CC) $(CFLAGS) $(INCLUDE) -MT $@  -c $< -o $@ 

common.o : common.c
	$(CC) $(CFLAGS) $(INCLUDE) -MT $@  -c $< -o $@ 

main.o : main.c
	$(CC) $(CFLAGS) $(INCLUDE) -MT $@  -c $< -o $@ 

clean : 
	rm KNX_KLIC_DA_LT_grab *.o


