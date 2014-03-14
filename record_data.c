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
	fprintf(ofp, "Timestamp| RTD1 | RTD2 | RTD3 | RTD4 |Pressure\n");
	fprintf(ofp, "hh:mm:ss | (K)  | (K)  | (K)  | (K)  | (PSI)  \n");
	fprintf(ofp, "---------+------+------+------+------+--------\n");
	fclose(ofp);
}

void save_datum(const double voltages[4], const double pressure_k)
{
	char newline[200];

	time_t rawtime;
	time(&rawtime);
	struct tm* timeinfo;
	timeinfo = localtime(&rawtime);
	strftime (newline,80,"%H:%M:%S ",timeinfo);

	FILE* ofp = fopen(file_name, "a");
	strcat(newline, "| %5.1f| %5.1f| %5.1f| %5.1f|   %5.1f\n");
	fprintf(ofp, newline,
		voltages[0], voltages[1], voltages[2], voltages[3],
		pressure_k);
	fclose(ofp);
}
