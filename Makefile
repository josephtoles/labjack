#
# Makefile for U3 examples
#

CC=gcc
CFLAGS=-std=gnu99

MAIN_SRC=main.c main.h u3.c
MAIN_OBJ=$(MAIN_SRC:.c=.o)

RECORD_DATA_SRC=record_data.c record_data.h
RECORD_DATA_OBJ=$(RECORD_DATA_SRC:.c=.o)

GRAPH_SRC=graph.c graph.h
GRAPH_OBJ=$(GRAPH_SRC:.v=.o)

SRCS=$(wildcard *.c)
HDRS=$(wildcard *.h)

CFLAGS +=-Wall -g
LIBS=-lm -llabjackusb

all: main

main: $(MAIN_OBJ) $(RECORD_DATA_OBJ) $(HDRS)
	rm -f main
	$(CC) -o main $(CFLAGS) $(MAIN_OBJ) $(RECORD_DATA_OBJ) $(GRAPH_OBJ) $(LDFLAGS) $(LIBS)

record_data: $(RECORD_DATA_OBJ) $(HDRS)
	$(CC) -c $(CFLAGS) $(RECORD_DATA_OBJ) $(LDFLAGS)

graph: $(GRAPH_OBJ) $(HDRS)
	$(CC) -c $(CFLAGS) $(GRAPH_OBJ) $(LDFLAGS)

clean:
	rm -f *.o main *~ pt_control
