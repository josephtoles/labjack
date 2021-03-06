CC=gcc
CFLAGS=-std=gnu99

ANALYSIS_SRC=analysis.c
ANALYSIS_OBJ=$(ANALYSIS_SRC:.c=.o)

AQUISITION_MAIN_SRC=main.c main.h u3.c
AQUISITION_MAIN_OBJ=$(AQUISITION_MAIN_SRC:.c=.o)

RECORD_DATA_SRC=record_data.c record_data.h
RECORD_DATA_OBJ=$(RECORD_DATA_SRC:.c=.o)

TEMP_GRAPH_SRC=temp_graph.c temp_graph.h
TEMP_GRAPH_OBJ=$(TEMP_GRAPH_SRC:.c=.o)

PRES_GRAPH_SRC=pres_graph.c pres_graph.h
PRES_GRAPH_OBJ=$(PRES_GRAPH_SRC:.c=.o)

CALCULATIONS_SRC=calculations.c calculations.h
CALCULATIONS_OBJ=$(CALCULATIONS_SRC:.c=.o)

SRCS=$(wildcard *.c)
HDRS=$(wildcard *.h)

CFLAGS +=-Wall -g
LIBS=-lm -llabjackusb

all: main plot

plot: $(ANALYSIS_OBJ) $(CALCULATIONS_OBJ) calculations.h
	$(CC) -o plot $(CFLAGS) $(ANALYSIS_OBJ) $(CALCULATIONS_OBJ) $(LDFLAGS) $(LIBS)

#The "rm -f main" command is used because of file permission issues
#accessing the labjack
main: $(AQUISITION_MAIN_OBJ) $(RECORD_DATA_OBJ) $(TEMP_GRAPH_OBJ) $(PRES_GRAPH_OBJ) $(CALCULATIONS_OBJ) $(HDRS)
	rm -f main
	$(CC) -o main $(CFLAGS) $(AQUISITION_MAIN_OBJ) $(RECORD_DATA_OBJ) $(TEMP_GRAPH_OBJ) $(PRES_GRAPH_OBJ) $(CALCULATIONS_OBJ) $(LDFLAGS) $(LIBS)

record_data: $(RECORD_DATA_OBJ) $(HDRS)
	$(CC) -c $(CFLAGS) $(RECORD_DATA_OBJ) $(LDFLAGS)

temp_graph: $(TEMP_GRAPH_OBJ) $(HDRS)
	$(CC) -c $(CFLAGS) $(TEMP_GRAPH_OBJ) $(LDFLAGS)

pres_graph: $(PRES_GRAPH_OBJ) $(HDRS)
	$(CC) -c $(CFLAGS) $(PRES_GRAPH_OBJ) $(LDFLAGS)

calculations: $(CALCULATIONS_OBJ) $(HDRS)
	$(CC) -c $(CFLAGS) $(CALCULATIONS_OBJ) $(LDFLAGS)

clean:
	rm -f *.o main plot *~
