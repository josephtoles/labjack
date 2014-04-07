#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

//Graph updates exactly once per second

const int UPDATE_DELAY = 3; //seconds
const char* ROOT_FILE_NAME = "graph.C";

//public
int flash_animation(double x[], double y[], int n);

//private
int create_script(double x[], double y[], int n);
void INThandler(int);
static pid_t pID;
void cleanup();


