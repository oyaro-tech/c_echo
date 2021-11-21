CC=gcc
FLAGS=-Wall -Werror -O3

all: build

build:
	$(CC) $(FLAGS) server.c -o server	
	$(CC) $(FLAGS) client.c -o client

clean: 
	rm -rf server server.o client client.o
