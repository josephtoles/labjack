/**
 * @author Joseph Toles
 * @phone (425) 269-7168
 * @email jbtoles@gmail.com
 * @title LabJack pressure and temperature monitor
 * @date April 6, 2014
 */

#include "main.h"

//CONSTANTS
const int UPDATES_PER_SECOND = 10;
const int NUM_CHANNELS = 8;
const double CELCIUS_TO_KELVIN = 272.15;
const int DISC_SAVE_DELAY = 2; //seconds (Use 2 for testing, 60 for production)
const int GRAPH_REFRESH_DELAY = 60; //seconds (Use 10 for testing, 60 for production. Do not lower much below 7.)
const int GRAPH_POINT_SAVE_DELAY = 60; //seconds
const time_t PRESSURE_REFRESH_EPSILON = 50; //clock_t
const int NUM_RTDS = 4;

//STATIC VARIABLES
static HANDLE hDevice;
static u3CalibrationInfo caliInfo;
static int localID = -1;
static long error = 0;
static long DAC1Enable;
static bool text_mode = false; //don't display graphs

//DYNAMIC MEMORY
double* ar_temp[4]; //temperature //4 is equal to NUM_RTDs. Figure out how to variable-ize this number.
double* ar_pres; //pressure
double* ar_time; //time
int ar_len = 10; //dynamic memory array length
int points_in_graph = 0;

//FUNCTION BODIES
int main(int argc, char **argv)
{
    //flags
    int c;
    extern char *optarg;
    extern int optind, optopt, opterr;
    while ((c = getopt(argc, argv, ":t")) != -1) {
        switch(c) {
        case 't':
            printf("Text mode engaged. No graphs will be displayed.\n");
            text_mode = true;
            break;
        case '?':
            printf("unknown arg %c\n", optopt);
        break;
        }
    }

    ar_time = (double*)malloc(ar_len*sizeof(double));
    ar_pres = (double*)malloc(ar_len*sizeof(double));
    for(int i=0; i<NUM_RTDS; ++i)
        ar_temp[i] = (double*)malloc(ar_len*sizeof(double));

	create_record();

	//open connection
	if( (hDevice = openUSBConnection(localID)) == NULL)
	{
		fprintf(stderr, "error: could not open USB connection.\n");
		return 1;
	}
	if( getCalibrationInfo(hDevice, &caliInfo) < 0)
	{
		fprintf(stderr, "error: could not get calibration info.\n");
		return 1;
	}

	//read voltages
	int configIO = 1;
	double voltages[NUM_CHANNELS];
	printf("             ");
	for(int i=0; i<NUM_CHANNELS; ++i)
		printf("     Ch%d", i);
    printf("\n");
	printf("              ");
    for(int i=0; i<NUM_RTDS; ++i)
        printf("    RTD%d", i);
    printf("                            Pressure");
	printf("\n");
	time_t last_disc_save_time = 0;

    //graphing
    clock_t last_graph_update = -GRAPH_REFRESH_DELAY*CLOCKS_PER_SEC; //arbitrary small negative number

    //misc
    int num_samples = 0;
    int samples_saved = 0;

	while(true)
	{
        //Update standard output
		for(int channel = 0; channel < NUM_CHANNELS; ++channel)
			if( (error = eAIN(hDevice, &caliInfo, configIO, &DAC1Enable, channel, 31, &voltages[channel], 0, 0, 0, 0, 0, 0)) != 0 )
				goto close;
		printf("Voltage	    (V)");
		for(int channel = 0; channel < NUM_CHANNELS; ++channel)
			printf("  %6.3f", voltages[channel]); //Maximum Labjack precision
			printf("\n");
		printf("Resistance  (\u03a9)"); //"\u03a9" is a lowercase omega
		for(int channel = 0; channel < 4; ++channel)
			printf("  %6.1f", resistance(voltages[channel], channel));
		printf("\n");
		printf("Temperature (K)");
		for(int channel = 0; channel < 4; ++channel)
			printf("  %6.1f", temperature(voltages[channel], channel));
		printf("\n");
		printf("Temperature (C)");
		for(int channel = 0; channel < 4; ++channel)
			printf("  %6.1f", temperature(voltages[channel], channel)-CELCIUS_TO_KELVIN);
		printf("\n");
		printf("Pressure  (PSI)                                                          %5.1f\n", pressure(voltages[7]));
        //printf("Sample #%d. Total %d samples saved to disk. Total %d points on graph.\n", ++num_samples, samples_saved, points_in_graph);

        //Calculate temperatures
		double temperatures[4];
		for(int i=0; i<4; ++i)
			temperatures[i] = temperature(voltages[i], i);

        //Save to disc
        if(clock() > last_disc_save_time + DISC_SAVE_DELAY*CLOCKS_PER_SEC)
		{
			last_disc_save_time = clock();
			save_datum(temperatures, pressure(voltages[7]), voltages);
            ++samples_saved;
		}

        if(!text_mode)
        {
            //Arrays, graph, dynamic memory
            if(points_in_graph == ar_len) //arrays are full
            {
                double* n_ar_time = (double*)malloc(2*ar_len*sizeof(double));
                for(int i=0; i<ar_len; ++i)
                    n_ar_time[i] = ar_time[i];
                free(ar_time);
                ar_time = n_ar_time;
        
                double* n_ar_pres = (double*)malloc(2*ar_len*sizeof(double));
                for(int i=0; i<ar_len; ++i)
                    n_ar_pres[i] = ar_pres[i];
                free(ar_pres);
                ar_pres = n_ar_pres;
                
                for(int i=0; i<NUM_RTDS; ++i)
                {
                    double* n_ar_temp = (double*)malloc(2*ar_len*sizeof(double));
                    for(int j=0; j<ar_len; ++j)
                        n_ar_temp[j] = ar_temp[i][j];
                    free(ar_temp[i]);
                    ar_temp[i] = n_ar_temp;
                }
        
                ar_len *= 2;
            }
            static clock_t last_graph_point_save = 0;
            if(clock() > last_graph_point_save+GRAPH_POINT_SAVE_DELAY*CLOCKS_PER_SEC)
            {
                for(int i=0; i<NUM_RTDS; ++i)
                    ar_temp[i][points_in_graph] = temperatures[i];
                ar_pres[points_in_graph] = pressure(voltages[7]);
                ar_time[points_in_graph] = points_in_graph;
                ++points_in_graph;
                last_graph_point_save = clock();
            }
    
            //Update graph
            if(clock() >= CLOCKS_PER_SEC*GRAPH_REFRESH_DELAY + last_graph_update + PRESSURE_REFRESH_EPSILON)
            {
                flash_temp_animation(ar_time, ar_temp, points_in_graph, GRAPH_REFRESH_DELAY);
                flash_pres_animation(ar_time, ar_pres, points_in_graph, GRAPH_REFRESH_DELAY);
                last_graph_update = clock();
            }
        }

        //Pause before updating screen standard output
		clock_t goal = CLOCKS_PER_SEC/UPDATES_PER_SECOND + clock();
		while (goal > clock());
		for(int i=0; i<5; ++i) fputs("\033[A\033[2K",stdout); //clear lines
		configIO = 0;
	}

//close connection
close:
	if( error > 0 )
		printf("Received an error code of %ld\n", error);
	closeUSBConnection(hDevice);
	return 0;
}

double resistance(double voltage, int channel)
{
	//are these the voltages including or not including the cable?
	double v0,v1,r0,r1;
	switch(channel)
	{
	/* Each board resistance sensor is calibrated from a linear fit of the voltage
	 * measured of two resistors*/
	case 0:;
		v0 = 0.085;
		r0 = 20.0;
		v1 = 1.882;
		r1 = 98.6;
		return (voltage-v0)*(r1-r0)/(v1-v0)+r0;
	case 1:;
		v0 = 0.091;
		r0 = 21.1;
		v1 = 1.879;
		r1 = 98.6;
		return (voltage-v0)*(r1-r0)/(v1-v0)+r0;
	case 2:;
		v0 = 0.116;
		r0 = 21.1;
		v1 = 1.977;
		r1 = 98.6;
		return (voltage-v0)*(r1-r0)/(v1-v0)+r0;
	case 3:;
		v0 = 0.117;
		r0 = 21.0;
		v1 = 1.977;
		r1 = 98.6;
		return (voltage-v0)*(r1-r0)/(v1-v0)+r0;
	default:
		break;
	}
	return (voltage-0.005)*100.0/2.400+18.0;
}

double temperature(double voltage, int channel)
{
	double t0 = 8.6+CELCIUS_TO_KELVIN; //room temperature
    double t1 = 77.0; //liquid nitrogen
	double v0,v1;
	switch(channel)
	{
	case 0:;
		v0 = 2.011;
		v1 = 0.080;
		return (voltage-v0)*(t1-t0)/(v1-v0)+t0;
	case 1:;
		v0 = 2.011;
		v1 = 0.100;
		return (voltage-v0)*(t1-t0)/(v1-v0)+t0;
	case 2:;
		v0 = 2.125;
		v1 = 0.115;
		return (voltage-v0)*(t1-t0)/(v1-v0)+t0;
	case 3:;
		v0 = 2.122;
		v1 = 0.120;
		return (voltage-v0)*(t1-t0)/(v1-v0)+t0;
	default:
		break;
	}
	return 0;
}

//Both pressure and voltage start at zero here, after counting the offset
const double PRESSURE_VOLTAGE_OFFSET = 0.010;
const double VOLTAGE_RANGE = 2.4;
const double PRESSURE_RANGE = 200.0; //(PSI)
double pressure(double voltage)
{
	return PRESSURE_RANGE*((voltage-PRESSURE_VOLTAGE_OFFSET)/VOLTAGE_RANGE);
}
