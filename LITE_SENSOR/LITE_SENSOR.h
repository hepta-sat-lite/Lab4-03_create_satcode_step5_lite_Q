#ifndef MBED_LITESENSOR_H
#define MBED_LITESENSOR_H
//#include "HeptaCamera_GPS.h"
#include "Lite9axis.h"
#include "HeptaTemp.h"

class LITE_SENSOR : public Lite9axis, public HeptaTemp
{
public:
    LITE_SENSOR(
        /*PinName tx,PinName rx,PinName sw1,PinName sw2,
        PinName sda,PinName scl,int aaddr,int agaddr,
        PinName adcon*/
        PinName adcon,
        PinName sda,PinName scl
        //PinName tx,PinName rx,PinName sw1,PinName sw2
    );

private:
};

#endif