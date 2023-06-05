#include "HEPTA_SENSOR.h"
/*HEPTA_SENSOR::HEPTA_SENSOR(PinName tx,PinName rx,PinName sw1,PinName sw2,
        PinName sda,PinName scl,int aaddr,int agaddr,
        PinName adcon): HeptaCamera_GPS(tx,rx,sw1,sw2),Hepta9axis(sda,scl,aaddr,agaddr),HeptaTemp(adcon){
}*/
HEPTA_SENSOR::HEPTA_SENSOR(PinName adcon,
        PinName sda,PinName scl,int aaddr): HeptaTemp(adcon),Hepta6axis(sda,scl,aaddr){
}