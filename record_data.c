#include "record_data.h"

static char file_name[80];

void create_record()
{
	time_t rawtime;
	time(&rawtime);
	struct tm* timeinfo;
	timeinfo = localtime(&rawtime);
	strftime (file_name,80,"%Y-%m-%d_%H:%M:%S",timeinfo);
	FILE* ofp = fopen(file_name, "w");
	fprintf(ofp, "Timestamp | RTD1 K | RTD2 K | RTD3 K | RTD4 K | P (PSI)\n");
	fprintf(ofp, "----------+--------+--------+--------+--------+--------\n");
	fclose(ofp);
}

void save_datum(const double voltages[4], const double pressure_k)
{

	FILE* ofp = fopen(file_name, "a");
	char newline[200];
	strcpy(newline, "%s|%5.1f|%5.1f|%5.1f|%5.1f|%5.1f");
	fprintf(ofp, newline,
		"12-12-12",
		voltages[0], voltages[1], voltages[2], voltages[3],
		pressure_k);
	fclose(ofp);
}
