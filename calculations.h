#ifndef CALCULATIONS_H_
#define CALCULATIONS_H_

//extern const int NUM_CHANNELS;

double resistance(double voltage, int channel);
double temperature(double voltage, int channel);
double pressure(double voltage);
void load_calibration_data();

#endif
