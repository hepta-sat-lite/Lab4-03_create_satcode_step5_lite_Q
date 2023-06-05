#include"HeptaTemp.h"
#include"mbed.h"

HeptaTemp::HeptaTemp(PinName pin) : _pin(pin)
{

}

void HeptaTemp::temp_sense(float* temp)
{
    //temperature coefficient
    *temp = ((_pin.read())*3.3-0.6)*100;
}

void HeptaTemp::temp_sense_u16(char* temp_u16)
{
    unsigned short temp_datas;
    char temp1[8]= {0x00},temp2[8]= {0x00};
    temp_datas=_pin.read_u16()>>4;
    sprintf( temp1, "%02X", (temp_datas >> 8) & 0x0F);
    sprintf( temp2, "%02X", (temp_datas) & 0xFF);
    temp_u16[0]=temp1[0];
    temp_u16[1]=temp1[1];
    temp_u16[2]=temp2[0];
    temp_u16[3]=temp2[1];
    //*dsize = 4;
}

void HeptaTemp::temp_sensing_vol(float* voltage)
{
    *voltage = (_pin.read())*3.3;
}