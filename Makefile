CC=gcc
CFLAGS=-Wall

all: main

main: main.o parser.o lexer.o
	$(CC) $(CLFAGS) -o main main.o parser.o lexer.o

clean:
	rm -rf *.o main

run: main
	./main test.json
