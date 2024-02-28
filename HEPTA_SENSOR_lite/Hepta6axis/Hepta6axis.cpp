#include"Hepta6axis.h"
#include"mbed.h"

Hepta6axis::Hepta6axis(PinName sda, PinName scl, int aaddr) : n_axis(sda,scl),addr_accel_gyro(aaddr)
{
    /*n_axis.frequency(100000);
    cmd[0]=0x6B;
    cmd[1]=0x00;
    n_axis.write(addr_accel_gyro,cmd,2);
    cmd[0]=0x6C;
    cmd[1]=0x00;
    n_axis.write(addr_accel_gyro,cmd,2);
    cmd[0] = 0x37;
    cmd[1] = 0x02;
    n_axis.write(addr_accel_gyro,cmd,2);
    n_axis.stop();*/
}

void Hepta6axis::setup()
{
    n_axis.frequency(100000);
    cmd[0]=0x6B;
    cmd[1]=0x00;
    n_axis.write(addr_accel_gyro,cmd,2);
    cmd[0] = 0x37;
    cmd[1] = 0x02;
    n_axis.write(addr_accel_gyro,cmd,2);
    n_axis.stop();
}

void Hepta6axis::sen_acc(float *ax,float *ay,float *az)
{
    setup();
    setup();
//x-axis accel
    adata[0] = 0x3B;
    n_axis.write(addr_accel_gyro,adata,1);
    n_axis.read(addr_accel_gyro|0x01,xh,1);
    adata[0] = 0x3C;
    n_axis.write(addr_accel_gyro,adata,1);
    n_axis.read(addr_accel_gyro|0x01,xl,1);
    double  acc_ax = short((xh[0]<<8) | (xl[0]));
    *ax = (acc_ax)*2/32764*9.81;

//y-axis
    adata[0] = 0x3D;
    n_axis.write(addr_accel_gyro,adata,1);
    n_axis.read(addr_accel_gyro|0x01,yh,1);
    adata[0] = 0x3E;
    n_axis.write(addr_accel_gyro,adata,1);
    n_axis.read(addr_accel_gyro|0x01,yl,1);
    double  acc_ay = short((yh[0]<<8) | (yl[0]));
    *ay = (acc_ay)*2/32764*9.81;

//z-axis
    adata[0] = 0x3F;
    n_axis.write(addr_accel_gyro,adata,1);
    n_axis.read(addr_accel_gyro|0x01,zh,1);
    adata[0] = 0x40;
    n_axis.write(addr_accel_gyro,adata,1);
    n_axis.read(addr_accel_gyro|0x01,zl,1);
    double  acc_az = short((zh[0]<<8) | (zl[0]));
    *az = (acc_az)*2/32764*9.81;
}

void Hepta6axis::sen_gyro(float *gx,float *gy,float *gz)
{
    setup();
    setup();
//x-axis gyro
    adata[0] = 0x43;
    n_axis.write(addr_accel_gyro,adata,1);
    n_axis.read(addr_accel_gyro|0x01,gxh,1);
    adata[0] = 0x44;
    n_axis.write(addr_accel_gyro,adata,1);
    n_axis.read(addr_accel_gyro|0x01,gxl,1);
    double  gyro_ax = short((gxh[0]<<8) | (gxl[0]));
    *gx = (gyro_ax)*0.01526;
    
//y-axis gyro
    adata[0] = 0x45;
    n_axis.write(addr_accel_gyro,adata,1);
    n_axis.read(addr_accel_gyro|0x01,gyh,1);
    adata[0] = 0x46;
    n_axis.write(addr_accel_gyro,adata,1);
    n_axis.read(addr_accel_gyro|0x01,gyl,1);
    double  gyro_ay = short((gyh[0]<<8) | (gyl[0]));
    *gy = (gyro_ay)*0.01526;
    
//z-axis gyro
    adata[0] = 0x47;
    n_axis.write(addr_accel_gyro,adata,1);
    n_axis.read(addr_accel_gyro|0x01,gzh,1);
    adata[0] = 0x48;
    n_axis.write(addr_accel_gyro,adata,1);
    n_axis.read(addr_accel_gyro|0x01,gzl,1);
    double  gyro_az = short((gzh[0]<<8) | (gzl[0]));
    *gz = (gyro_az)*0.01526;
}

//////////////For HEX/////////////////////////////////////////
/*void Hepta6axis::sen_gyro_u16(char* gx_u16,char* gy_u16,char* gz_u16)
{
    //x
    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x43);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    gxh = n_axis.read(0);
    n_axis.stop();

    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x44);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    gxl = n_axis.read(0);
    n_axis.stop();

    sprintf( g1, "%02X", ((gxh)) & 0xFF);
    sprintf( g2, "%02X", ((gxl)) & 0xFF);
    gx_u16[0]=g1[0];
    gx_u16[1]=g1[1];
    gx_u16[2]=g2[0];
    gx_u16[3]=g2[1];

//y
    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x45);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    gyh = n_axis.read(0);
    n_axis.stop();

    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x46);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    gyl = n_axis.read(0);
    n_axis.stop();
    sprintf( g1, "%02X", (gyh) & 0xFF);
    sprintf( g2, "%02X", (gyl) & 0xFF);
    gy_u16[0]=g1[0];
    gy_u16[1]=g1[1];
    gy_u16[2]=g2[0];
    gy_u16[3]=g2[1];

//z
    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x47);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    gzh = n_axis.read(0);
    n_axis.stop();

    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x48);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    gzl = n_axis.read(0);
    n_axis.stop();

    sprintf( g1, "%02X", ((gzh)) & 0xFF);
    sprintf( g2, "%02X", ((gzl)) & 0xFF);
    gz_u16[0]=g1[0];
    gz_u16[1]=g1[1];
    gz_u16[2]=g2[0];
    gz_u16[3]=g2[1];
    //*dsize = 4;
}

void Hepta6axis::sen_acc_u16(char* ax_u16,char* ay_u16,char* az_u16)
{
    //x
    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x3B);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    xh = n_axis.read(0);
    n_axis.stop();

    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x3C);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    xl = n_axis.read(0);
    n_axis.stop();

    sprintf( a1, "%02X", ((xh)) & 0xFF);
    sprintf( a2, "%02X", ((xl)) & 0xFF);
    ax_u16[0]=a1[0];
    ax_u16[1]=a1[1];
    ax_u16[2]=a2[0];
    ax_u16[3]=a2[1];

//y
    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x3D);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    yh = n_axis.read(0);

    n_axis.stop();
    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x3E);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    yl = n_axis.read(0);
    n_axis.stop();

    sprintf( a1, "%02X", ((yh)) & 0xFF);
    sprintf( a2, "%02X", ((yl)) & 0xFF);
    ay_u16[0]=a1[0];
    ay_u16[1]=a1[1];
    ay_u16[2]=a2[0];
    ay_u16[3]=a2[1];
//z
    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x3F);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    zh = n_axis.read(0);
    n_axis.stop();

    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x40);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    zl = n_axis.read(0);
    n_axis.stop();

    sprintf( a1, "%02X", ((zh)) & 0xFF);
    sprintf( a2, "%02X", ((zl)) & 0xFF);
    az_u16[0]=a1[0];
    az_u16[1]=a1[1];
    az_u16[2]=a2[0];
    az_u16[3]=a2[1];
    //*dsize = 4;
}*/