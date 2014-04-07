#ifndef GRAPH_H_
#define GRAPH_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

//Graph updates exactly once per second

//public
int flash_temp_animation(double x[], double y[], int n, int update_delay);
int flash_pres_animation(double x[], double y[], int n, int update_delay);

//private
int create_temp_script(double x[], double y[], int n, int update_delay);
int create_pres_script(double x[], double y[], int n, int update_delay);
void INThandler(int);
void cleanup();
int pID;

#endif
