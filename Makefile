CC=gcc
AR=ar
FLAGS= -Wall -g

all: main server

main:  main.o
	$(CC) $(FLAGS) -o main main.o

server:  server.o
	$(CC) $(FLAGS) -o server server.o

server.o: server.c
	$(CC) $(FLAGS) -c server.c

main.o: main.c
	$(CC) $(FLAGS) -c main.c

.PHONY: clean all
clean:
	rm -f *.o main server
