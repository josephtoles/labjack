ifdef ICC_DEFINED
CC=icpc
else
CC=gcc
endif

ROOT_CFLAGS = `$(ROOTSYS)/bin/root-config --cflags`
ROOT_LIBS = `$(ROOTSYS)/bin/root-config --libs` -lThread -lGui

CFLAGS = $(ROOT_CFLAGS) -g -fPIC

all: Main

Main : main.o
	$(CC) $(CFLAGS) -o $@ main.o $(ROOT_LIBS)

main.o: main.cpp
	$(CC) -c -o $@ $< $(ROOT_FLAGS) $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o $(INC_DIR)/*~ *~ Main





