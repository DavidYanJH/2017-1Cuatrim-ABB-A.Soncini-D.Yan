CC = gcc
CFLAGS = -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror
TDA = abb
PRUEBAS = pruebas_alumno
EXE = pruebas


all: tda testing.o main.o pruebas.o pila.o
	$(CC) $(CFLAGS) -o $(EXE) $(PRUEBAS).o $(TDA).o testing.o main.o pila.o

tda:
	$(CC) $(CFLAGS) $(TDA).c -c

testing.o:
	$(CC) $(CFLAGS) testing.c -c

main.o:
	$(CC) $(CFLAGS) main.c -c

pruebas.o:
	$(CC) $(CFLAGS) $(PRUEBAS).c -c

pila.o:
	$(CC) $(CFLAGS) pila.c -c

clean:
	rm $(EXE) *.o

run: all
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./$(EXE)
