CC = gcc
CFLAGS = -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror
TDA = abb
PRUEBAS = pruebas_catedra
EXE = pruebas


all: tda testing.o lista.o main.o
	$(CC) $(CFLAGS) -o $(EXE) $(PRUEBAS).c $(TDA).o testing.o main.o

tda:
	$(CC) $(CFLAGS) $(TDA).c -c

testing.o:
	$(CC) $(CFLAGS) testing.c -c

main.o:
	$(CC) $(CFLAGS) main.c -c

clean:
	rm $(EXE) *.o

run: all
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./$(EXE)
