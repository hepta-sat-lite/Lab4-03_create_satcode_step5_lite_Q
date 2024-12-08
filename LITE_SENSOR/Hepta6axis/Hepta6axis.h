#ifndef MBED_HEPTA6AXIS_H
#define MBED_HEPTA6AXIS_H
#include "mbed.h"

class Hepta6axis
{
public:
    I2C n_axis;
    int addr_accel_gyro;
    Hepta6axis(
        PinName sda,
        PinName scl,
        int aaddr
    );
    void setup();
    void sen_acc(float *ax,float *ay,float *az);
    void sen_gyro(float *gx,float *gy,float *gz);
    //void sen_gyro_u16(char* gx_u16,char* gy_u16,char* gz_u16);
    //void sen_acc_u16(char* ax_u16,char* ay_u16,char* az_u16);

private:
    char cmd[2];
    char adata[1];
    char g1[8],g2[8];
    char a1[8],a2[8];
    char xl[1],xh[1],yl[1],yh[1],zl[1],zh[1];
    char gxl[1],gxh[1],gyl[1],gyh[1],gzl[1],gzh[1];
    //char xl[1],xh[1],yl[1],yh[1],zl[1],zh[1];
    short int st2;
    //short int xl,xh,yl,yh,zl,zh;
    //short int gxl,gxh,gyl,gyh,gzl,gzh;
};

#endif