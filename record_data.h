#ifndef RECORD_DATA_H_
#define RECORD_DATA_H_

#include <stdio.h>
#include <string.h>
#include <time.h>

void save_datum(const double voltage[4], const double pressure_k);
void create_record();

#endif
