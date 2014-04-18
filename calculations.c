#include "main.h"

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
    double t0 = 23.0+CELCIUS_TO_KELVIN; //room temperature
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

