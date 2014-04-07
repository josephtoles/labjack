#ifndef MAIN_H_
#define MAIN_H_

#include "record_data.h"
#include "temp_graph.h"
#include "u3.h"

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

void close_connection();
double kelvin(double v);
int main(int argc, char **argv);
double resistance(double voltage, int channel);
double temperature(double voltage, int channel);
double pressure(double voltage);

#endif
