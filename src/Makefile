# -*- Makefile -*-

.PHONY: all clean

all: main.o httpMethods.o utils.o stats.o
	gcc main.o httpMethods.o utils.o stats.o -o htbench

main.o: main.c inputs.h httpMethods.h constants.h
	gcc -c main.c

httpMethods.o: httpMethods.c httpMethods.h inputs.h constants.h utils.h stats.h
	gcc -c httpMethods.c

utils.o: utils.c utils.h
	gcc -c utils.c

stats.o: stats.c stats.h inputs.h
	gcc -c stats.c

clean:
	rm -rf *o htbench