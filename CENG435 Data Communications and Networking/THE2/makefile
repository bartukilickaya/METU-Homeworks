CC=gcc -g -std=c99
INC_DIR = ./
CFLAGS=-c -Wall -I$(INC_DIR)


all: main

main:
	gcc -g -Wall -o client client.c
	gcc -g -Wall -o server server.c

#client.o: client.c
#	$(CC) $(CFLAGS) client.c

#server.o:  server.c
#	$(CC) $(CFLAGS) server.c


clean:
	    rm -rf *o all
