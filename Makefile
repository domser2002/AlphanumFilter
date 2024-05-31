CC=gcc
CFLAGS= -std=gnu99 -Wall
server:
	${CC} ${CFLAGS} -o server.o server.c
client:
	${CC} ${CFLAGS} -o client.o client.c
all: server client