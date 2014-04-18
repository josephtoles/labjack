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
