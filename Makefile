CC=gcc
CFLAGS=-Wall

.INTERMEDIATE: main.o lexer.o parser.o

all: main

main: main.o lexer.o parser.o 
	$(CC) $(CLFAGS) -o main main.o lexer.o parser.o

test:
	make -C ./tests test

clean:
	rm -rf *.o main
	make -C ./tests clean
