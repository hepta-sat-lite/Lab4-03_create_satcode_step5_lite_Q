#ifndef MBED_HEPTA2TEMP_H
#define MBED_HEPTA2TEMP_H
#include"mbed.h"

class HeptaTemp
{
public:
    HeptaTemp(PinName pin);
    void temp_sense(float* temp);
    void temp_sense_u16(char* temp_u16);
    void temp_sensing_vol(float* voltage);

private:
    AnalogIn _pin;
};

#endif
