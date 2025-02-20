CC=gcc
CFLAGS=-Wall

.INTERMEDIATE: test.o ../lexer.o ../parser.o

.PHONY: test

test: test.o ../lexer.o ../parser.o
	$(CC) $(CFLAGS) -o test test.o ../lexer.o ../parser.o
	./test

clean:
	rm -rf *.o test
