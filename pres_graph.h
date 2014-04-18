#ifndef PRES_GRAPH_H_
#define PRES_GRAPH_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

//Graph updates exactly once per second

//public
int flash_pres_animation(double x[], double y[], int n, int update_delay);

#endif
