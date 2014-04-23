#include "main.h"
#include "calculations.h"

static double* calibration_temperatures;
static double* calibration_resistances;
static int calibration_n = 0;
static bool calibration_data_is_loaded = false;

static const char* CALIBRATION_DATA_FILE_NAME = "calibration_data";

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
    //testing
    printf("calibration_n is %d\n", calibration_n);
    printf("#lines is %d\n", lines);
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


//Output should be in kelvin. Always do everythingin kelvin.
double temperature(double voltage, int channel)
{
    if(!calibration_data_is_loaded)
    {
        load_calibration_data();
    }
    //resistance = 100 ohm at 173.15 K. Changes by 0.385 ohms per degree.
    /*
     * t is temperature
     * r is resistance
     */
    double r = resistance(voltage, channel);
    double delta_r = r - 100;
    double delta_t = delta_r / 0.385;
    double t = 273.15 + delta_t;
    return t;
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
