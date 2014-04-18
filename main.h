#ifndef MAIN_H_
#define MAIN_H_

#include "record_data.h"
#include "temp_graph.h"
#include "pres_graph.h"
#include "calculations.h"
#include "u3.h"

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

void close_connection();
int main(int argc, char **argv);

//CONSTANTS
extern const int NUM_CHANNELS;
const double CELCIUS_TO_KELVIN;

#endif
