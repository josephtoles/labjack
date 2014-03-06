#
# Makefile for U3 examples
#

CC=gcc
CFLAGS=-std=gnu99

PT_CONTROL_SRC=pt_control.c u3.c
PT_CONTROL_OBJ=$(PT_CONTROL_SRC:.c=.o)

SRCS=$(wildcard *.c)
HDRS=$(wildcard *.h)

CFLAGS +=-Wall -g
LIBS=-lm -llabjackusb

all: pt_control

pt_control: $(PT_CONTROL_OBJ) $(HDRS)
	$(CC) -o run $(CFLAGS) $(PT_CONTROL_OBJ) $(LDFLAGS) $(LIBS)

clean:
	rm -f *.o run *~ pt_control
