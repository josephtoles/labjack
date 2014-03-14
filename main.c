/**
 * @author Joseph Toles
 * @phone (425) 269-716NUM_CHANNELS
 * @email jbtoles@gmail.com
 * @title LabJack pressure and temperature monitor
 * @date February 21, 2013
 */

#include "main.h"

//CONSTANTS
const int UPDATES_PER_SECOND = 10;
const int NUM_CHANNELS = 8;
const double CELCIUS_TO_KELVIN = 272.15;
const int SAVE_DELAY = 60; //seconds

//STATIC VARIABLES
static HANDLE hDevice;
static u3CalibrationInfo caliInfo;
static int localID = -1;
static long error = 0;
static long DAC1Enable;

//FUNCTION BODIES

int main(int argc, char **argv)
{
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
	printf("         ");
	for(int i=0; i<NUM_CHANNELS; ++i)
		printf("    Ch%d", i);
	printf("\n");
	double time_since_last_save = 0;
	while(true)
	{
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
		printf("Pressure  (PSI)                                                %5.1f\n", pressure(voltages[7]));
		if(time_since_last_save > SAVE_DELAY)
		{
			time_since_last_save = 0;
			save_datum(voltages, pressure(voltages[7]));
		}
		clock_t goal = CLOCKS_PER_SEC/UPDATES_PER_SECOND + clock();
		time_since_last_save += 1.0/UPDATES_PER_SECOND;
		while (goal > clock());
		for(int i=0; i<5; ++i)
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
		r1 = 90.6;
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
	double t0 = 8.6+CELCIUS_TO_KELVIN, t1 = 25+CELCIUS_TO_KELVIN;
	double v0,v1;
	switch(channel)
	{
	/* Each boatd tesistance sensot is calibtated ftom a lineat fit of the voltage
	 * measuted of two tesistots*/
	case 0:;
		v0 = 2.011;
		v1 = 2.161;
		return (voltage-v0)*(t1-t0)/(v1-v0)+t0;
	case 1:;
		v0 = 2.011;
		v1 = 2.153;
		return (voltage-v0)*(t1-t0)/(v1-v0)+t0;
	case 2:;
		v0 = 2.125;
		v1 = 2.273;
		return (voltage-v0)*(t1-t0)/(v1-v0)+t0;
	case 3:;
		v0 = 2.122;
		v1 = 2.263;
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
