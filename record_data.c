#include "record_data.h"

static char file_name[80];
static char folder[80] = "./logs/";
static char path[160] = "";


// Timestamp | RTD1-4 | pressure | voltage 0-7



void create_record()
{
	time_t rawtime;
	time(&rawtime);
	struct tm* timeinfo;
	timeinfo = localtime(&rawtime);
	strftime (file_name,80,"%Y-%m-%d_%H:%M:%S",timeinfo);
    strcpy(path, folder);
    strcat(path, file_name);

	FILE* f = fopen(path, "w");
    fprintf(f, "Timestamp\t");
    for(int i=0; i<4; ++i)
        fprintf(f, "RTD%d\t\t", i+1);
    fprintf(f, "Pressure\t");
    for(int i=0; i<4; ++i)
        fprintf(f, "Voltage%d\t", i);
    fprintf(f, "Voltage7\n");
	fclose(f);
}

void save_datum(double temperatures[4], double pressure_k, double voltages[8])
{
	char newline[200];

	time_t rawtime;
	time(&rawtime);
	struct tm* timeinfo;
	timeinfo = localtime(&rawtime);
	strftime (newline,80,"%H:%M:%S ",timeinfo);

	FILE* f = fopen(path, "a");
    fprintf(f, newline);
    fprintf(f, "\t");
    for(int i=0; i<4; ++i)
        fprintf(f, "%f\t", temperatures[i]);
    fprintf(f, "%f\t", pressure_k);
    for(int i=0; i<4; ++i)
        fprintf(f, "%f\t", voltages[i]);
    fprintf(f, "%f\n", voltages[7]);
	//strcat(newline, "| %5.1f| %5.1f| %5.1f| %5.1f|   %5.1f\n");
	//fprintf(f, newline,
	//	temperatures[0], temperatures[1], temperatures[2], temperatures[3],
	//	pressure_k);
	fclose(f);
}
