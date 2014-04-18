#ifndef TEMP_GRAPH_H_
#define TEMP_GRAPH_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

//Graph updates exactly once per second

//public
int flash_temp_animation(double x[], double* y[4], int n, int update_delay);

#endif
