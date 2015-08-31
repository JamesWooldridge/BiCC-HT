CC = gcc -O0

bic: bic.o stack.o graph.o
	$(CC) -o bic bic.c stack.o graph.o

bic.o: bic.c stack.h graph.o
	$(CC) -c bic.c

stack.o: stack.c stack.h
	$(CC) -c stack.c

graph.o: graph.c graph.h
	$(CC) -c graph.c

graphgen: graphgen.o graph.o
	$(CC) -o graphgen graphgen.c graph.o

graphgen.o: graphgen.c
	$(CC) -c graphgen.c