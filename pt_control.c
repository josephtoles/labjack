/**
 * @author Joseph Toles
 * @phone (425) 269-716NUM_CHANNELS
 * @email jbtoles@gmail.com
 * @title LabJack pressure and temperature monitor
 * @date February 21, 2013
 */

#include "u3.h"
#include <unistd.h>
#include <stdio.h>
#include <time.h> 

//HEADERS
void close_connection();
double kelvin(double v);
int main(int argc, char **argv);
double resistance(double voltage, int channel);
double temperature(double resistance);
double pressure(double voltage);

//CONSTANTS
static const int UPDATES_PER_SECOND = 10;
static const int NUM_CHANNELS = 8;

//STATIC VARIABLES
static HANDLE hDevice;
static u3CalibrationInfo caliInfo;
static int localID = -1;
static long error = 0;
static long DAC1Enable;

//FUNCTION BODIES

int main(int argc, char **argv)
{
	//open connection
	if( (hDevice = openUSBConnection(localID)) == NULL)
	{
		fprintf(stderr, "error: could not open USB connection.\n");
		return 1;
	}
	if( getCalibrationInfo(hDevice, &caliInfo) < 0)
	{
		fprintf(stderr, "error: could not get calibration info.\n");
		return 1;	//	goto close;
	}

	//read voltages
	int configIO = 1;
	double voltages[NUM_CHANNELS];
	printf("	     ");
	for(int i=0; i<NUM_CHANNELS; ++i)
		printf("    Ch%d", i);
	printf("\n");
	while(true)
	{
		for(int channel = 0; channel < NUM_CHANNELS; ++channel)
			if( (error = eAIN(hDevice, &caliInfo, configIO, &DAC1Enable, channel, 31, &voltages[channel], 0, 0, 0, 0, 0, 0)) != 0 )
				goto close;
		printf("Voltage     (V)");
		for(int channel = 0; channel < NUM_CHANNELS; ++channel)
			printf("  %5.3f", voltages[channel]); //Maximum Labjack precision
    		printf("\n");
		printf("Resistance  (\u03a9)");
		for(int channel = 0; channel < 4; ++channel)
			printf("  %5.1f", resistance(voltages[channel], channel));
		printf("\n");
		printf("Temperature (K)");
		for(int channel = 0; channel < 4; ++channel)
			printf("  %5.1f", temperature(resistance(voltages[channel], channel)));
		printf("\n");
		printf("Pressure  (PSI)                                                   %5.1f\n", pressure(voltages[7]));
		clock_t goal = CLOCKS_PER_SEC/UPDATES_PER_SECOND + clock();
		while (goal > clock());
		for(int i=0; i<4; ++i)
	    		fputs("\033[A\033[2K",stdout);
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
	double v0,v1,r0,r1;
	switch(channel)
	{
	case 0:;
		v0 = 0.087;
		r0 = 20.8;
		v1 = 1.694;
		r1 = 90.3;
		return (voltage-v0)*(r1-r0)/(v1-v0)+r0;
	case 1:;
		v0 = 0.091;
		r0 = 20.8;
		v1 = 1.692;
		r1 = 90.3;
		return (voltage-v0)*(r1-r0)/(v1-v0)+r0;
	case 2:; //NOT CALIBRATED
		v0 = 0.091;
		r0 = 20.8;
		v1 = 1.692;
		r1 = 90.3;
		return (voltage-v0)*(r1-r0)/(v1-v0)+r0;
	case 3:; //NOT CALIBRATED
		v0 = 0.091;
		r0 = 20.8;
		v1 = 1.692;
		r1 = 90.3;
		return (voltage-v0)*(r1-r0)/(v1-v0)+r0;
	default:
		break;
	}
	return (voltage-0.005)*100/2.400+18;
}

double temperature(double resistance)
{
	//approximate, still needs fine tuning
	//return 72.0+(320.0-72.0)*(resistance-18.0)/100.0 ;
	double r0,r1,t0,t1;
	r0 = 17.11;
	r1 = 118.19;
	t0 = 70;
	t1 = 320.0;
	return t0+resistance*(r1-r0)/(t1-t0);
}

double pressure(double voltage)
{
	return 200.0*(voltage-1.0)/4.0;
}
