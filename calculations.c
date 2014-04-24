#include "main.h"
#include "calculations.h"

static double* calibration_temperatures;
static double* calibration_resistances;
static int calibration_n = 0;
static bool calibration_data_is_loaded = false;

static const char* CALIBRATION_DATA_FILE_NAME = "calibration_data";

//Output should be in kelvin. Always do everythingin kelvin.
double temperature(double voltage, int channel)
{
    if(!calibration_data_is_loaded)
        load_calibration_data();
    /*
     * t is temperature
     * r is resistance
     */
    double r = resistance(voltage, channel);
    if(r <= calibration_resistances[0])
        return 0; //error value
    if(r >= calibration_resistances[calibration_n-2])
        return 1000; //error value

    /*
     * The following search is inefficient, specifically O(n) time
     * But it should only deal with a list of 80 values and I do
     * not want to debug a more complicated search algorithm
     */
    int floor_index = 0;
    while(r > calibration_resistances[floor_index])
        ++floor_index;
    --floor_index;
    double r0 = calibration_resistances[floor_index];
    double r1 = calibration_resistances[floor_index+1];
    double t0 = calibration_temperatures[floor_index];
    double t1 = calibration_temperatures[floor_index+1];

    /*
     * Local linear fit
     */
    double t = t0 + (r-r0)*(t1-t0)/(r1-r0);
    return t;
}

void load_calibration_data()
{
    //count the lines
    FILE* f = fopen(CALIBRATION_DATA_FILE_NAME, "r");
    if(f == NULL)
    {
        printf("Could not open file %s.\n", CALIBRATION_DATA_FILE_NAME);
        exit(-1);
    }
    int lines = 0;
    while(!feof(f))
    {
        char ch = getc(f);
        if (ch=='\n')
            ++lines;
    }
    fclose(f);

    calibration_temperatures = (double*)malloc(sizeof(double)*lines);
    calibration_resistances = (double*)malloc(sizeof(double)*lines);

    f = fopen(CALIBRATION_DATA_FILE_NAME, "r");
 
    while(!feof(f))
    {
        fscanf(f,
            "%lf %lf",
            calibration_temperatures + calibration_n,
            calibration_resistances + calibration_n);
        ++calibration_n;
    }
    calibration_data_is_loaded = true;
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

//Here we will recalculate pressure from actual values
double pressure(double voltage)
{
    double p0 = 0.0;
    double v0 = 0.015;
    double p1 = 15.0;
    double v1 = 0.192;
    return (voltage-v0)*(p1-p0)/(v1-v0)+p0;
}
