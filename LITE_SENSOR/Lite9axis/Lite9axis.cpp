#include "mbed.h"
#include "Lite9axis.h"
 
Lite9axis::Lite9axis(PinName sda, PinName sck) : _i2c(sda, sck)
{
    _i2c.frequency(400000);
    gyroBias[0] = 0;
    gyroBias[1] = 0;
    gyroBias[2] = 0;
}
 
void Lite9axis::ICM_WriteByte(char ICM20948_reg, char ICM20948_data)
{
    char data_out[2];
    data_out[0]=ICM20948_reg;
    data_out[1]=ICM20948_data;
    _i2c.write(ICM20948_slave_addr, data_out, 2, 0);
}
 
char Lite9axis::ICM_ReadByte(char ICM20948_reg)
{
    char data_out[1], data_in[1];
    data_out[0] = ICM20948_reg;
    _i2c.write(ICM20948_slave_addr, data_out, 1, 1);
    _i2c.read(ICM20948_slave_addr, data_in, 1, 0);
    return (data_in[0]);
}

// Communication test: WHO_AM_I register reading
int Lite9axis::whoAmI()
{
    char whoAmI = ICM_ReadByte(ICM20948_WHO_AM_I);   // Should return 0x68
    if(whoAmI==0xea) {
        return 68;
    } else {
        return -1;
    }
}

void Lite9axis::reset()
{
    // reset ICM20948
    ICM_WriteByte(REG_ADD_REG_BANK_SEL, REG_VAL_REG_BANK_0);
    ICM_WriteByte(REG_ADD_PWR_MIGMT_1,  REG_VAL_ALL_RGE_RESET);
    wait_ms(10);
}

void Lite9axis::powerOn()
{
    // USER_BANK_0
    ICM_WriteByte(ICM20948_REG_BANK_SEL, USER_BANK_0);
    ICM_WriteByte(ICM20948_PWR_MGMT_1, 0x01);
    //ICM_WriteOneByte(ICM20948_PWR_MGMT_2, (0x38 | 0x07));
    ICM_WriteByte(ICM20948_PWR_MGMT_2, (0b00111000 | 0b00000111));
    wait_ms(10);//ThisThread::sleep_for(10ms);
    //ICM_WriteOneByte(ICM20948_PWR_MGMT_2, (0x00 | 0x00));
    ICM_WriteByte(ICM20948_PWR_MGMT_2, (0x00 | 0x00));
}
 
void Lite9axis::gyroCalib(){
    int16_t gxtmp, gytmp, gztmp;
    float gx, gy, gz;
    gx = 0.0; gy = 0.0; gz = 0.0;
    uint8_t LoByte, HiByte;
    for(int i = 0; i < 10; i++){
        LoByte = ICM_ReadByte(ICM20948_GYRO_XOUT_L); // read Gyrometer X_Low  value
        HiByte = ICM_ReadByte(ICM20948_GYRO_XOUT_H); // read Gyrometer X_High value
        gxtmp = ((HiByte<<8) | LoByte);
        gx += (float)(gxtmp) * gRes;
        
        LoByte = ICM_ReadByte(ICM20948_GYRO_YOUT_L);
        HiByte = ICM_ReadByte(ICM20948_GYRO_YOUT_H);
        gytmp = ((HiByte<<8) | LoByte);
        gy += (float)(gytmp) * gRes;
        
        LoByte = ICM_ReadByte(ICM20948_GYRO_ZOUT_L);
        HiByte = ICM_ReadByte(ICM20948_GYRO_ZOUT_H);
        gztmp = ((HiByte<<8) | LoByte);
        gz += (float)(gztmp) * gRes;
        wait_ms(50);//ThisThread::sleep_for(50ms);
    }
    
    gyroBias[0] = gx / 100.0;
    gyroBias[1] = gy / 100.0;
    gyroBias[2] = gz / 100.0;

}

void Lite9axis::set_up()
{
    wait_ms(10);
    reset();
    powerOn();
    switch((ACCEL_CONF >> 1) & 0x03)
    {
        case 0:
            aRes = (2.0*9.81)/32768.0;
            break;
        case 1:
            aRes = (4.0*9.81)/32768.0;
            break;
        case 2:
            aRes = (8.0*9.81)/32768.0;
            break;
        case 3:
            aRes = (16.0*9.81)/32768.0;
            break;
    }
    switch((GYRO_CONF >> 1) & 0x03)
    {
        case 0:
            gRes = 250.0/32768.0;
            break;
        case 1:
            gRes = 500.0/32768.0;
            break;
        case 2:
            gRes = 1000.0/32768.0;
            break;
        case 3:
            gRes = 2000.0/32768.0;
            break;
    }
    // USER_BANK_2 to access data
    ICM_WriteByte(ICM20948_REG_BANK_SEL, USER_BANK_2);
    ICM_WriteByte(ICM20948_GYRO_CONFIG_1, GYRO_CONF);
    ICM_WriteByte(ICM20948_GYRO_SMPLRT_DIV, GYRO_DIV);
    ICM_WriteByte(ICM20948_ACCEL_CONFIG, ACCEL_CONF);
    ICM_WriteByte(ICM20948_ACCEL_SMPLRT_DIV_2, ACCEL_DIV);
 
    // USER_BANK_0 to access data
    ICM_WriteByte(ICM20948_REG_BANK_SEL, USER_BANK_0);
    wait_ms(20);

    gyroCalib();
}

void Lite9axis::sen_acc(float *ax,float *ay,float *az){
    int16_t _ax, _ay, _az;
    uint8_t LoByte, HiByte;
    
    LoByte = ICM_ReadByte(ICM20948_ACCEL_XOUT_L); // read Accelerometer X_Low  value
    HiByte = ICM_ReadByte(ICM20948_ACCEL_XOUT_H); // read Accelerometer X_High value
    _ax = (HiByte<<8) | LoByte;
    *ax = ((float)(_ax)) * aRes;
    
    LoByte = ICM_ReadByte(ICM20948_ACCEL_YOUT_L);
    HiByte = ICM_ReadByte(ICM20948_ACCEL_YOUT_H);
    _ay = (HiByte<<8) | LoByte;
    *ay = ((float)(_ay)) * aRes;
    
    LoByte = ICM_ReadByte(ICM20948_ACCEL_ZOUT_L);
    HiByte = ICM_ReadByte(ICM20948_ACCEL_ZOUT_H);
    _az = (HiByte<<8) | LoByte;
    *az = ((float)(_az)) * aRes;
}

void Lite9axis::sen_gyro(float *gx,float *gy,float *gz){
    int16_t _gx, _gy, _gz;
    uint8_t LoByte, HiByte;

    LoByte = ICM_ReadByte(ICM20948_GYRO_XOUT_L); // read Gyrometer X_Low  value
    HiByte = ICM_ReadByte(ICM20948_GYRO_XOUT_H); // read Gyrometer X_High value
    _gx = (HiByte<<8) | LoByte;
    *gx = ((float)(_gx)) * gRes - gyroBias[0];
    
    LoByte = ICM_ReadByte(ICM20948_GYRO_YOUT_L);
    HiByte = ICM_ReadByte(ICM20948_GYRO_YOUT_H);
    _gy = (HiByte<<8) | LoByte;
    *gy = ((float)(_gy)) * gRes - gyroBias[1];
    
    LoByte = ICM_ReadByte(ICM20948_GYRO_ZOUT_L);
    HiByte = ICM_ReadByte(ICM20948_GYRO_ZOUT_H);
    _gz = (HiByte<<8) | LoByte;
    *gz = ((float)(_gz)) * gRes - gyroBias[2];
}

// void Lite9axis::getAccGyro(){
//     int16_t _ax, _ay, _az, _gx, _gy, _gz;
//     uint8_t LoByte, HiByte;
    
//     LoByte = ICM_ReadByte(ICM20948_ACCEL_XOUT_L); // read Accelerometer X_Low  value
//     HiByte = ICM_ReadByte(ICM20948_ACCEL_XOUT_H); // read Accelerometer X_High value
//     _ax = (HiByte<<8) | LoByte;
//     ax = ((float)(_ax)) * aRes;
    
//     LoByte = ICM_ReadByte(ICM20948_ACCEL_YOUT_L);
//     HiByte = ICM_ReadByte(ICM20948_ACCEL_YOUT_H);
//     _ay = (HiByte<<8) | LoByte;
//     ay = ((float)(_ay)) * aRes;
    
//     LoByte = ICM_ReadByte(ICM20948_ACCEL_ZOUT_L);
//     HiByte = ICM_ReadByte(ICM20948_ACCEL_ZOUT_H);
//     _az = (HiByte<<8) | LoByte;
//     az = ((float)(_az)) * aRes;
    
//     LoByte = ICM_ReadByte(ICM20948_GYRO_XOUT_L); // read Gyrometer X_Low  value
//     HiByte = ICM_ReadByte(ICM20948_GYRO_XOUT_H); // read Gyrometer X_High value
//     _gx = (HiByte<<8) | LoByte;
//     gx = ((float)(_gx)) * gRes - gyroBias[0];
    
//     LoByte = ICM_ReadByte(ICM20948_GYRO_YOUT_L);
//     HiByte = ICM_ReadByte(ICM20948_GYRO_YOUT_H);
//     _gy = (HiByte<<8) | LoByte;
//     gy = ((float)(_gy)) * gRes - gyroBias[1];
    
//     LoByte = ICM_ReadByte(ICM20948_GYRO_ZOUT_L);
//     HiByte = ICM_ReadByte(ICM20948_GYRO_ZOUT_H);
//     _gz = (HiByte<<8) | LoByte;
//     gz = ((float)(_gz)) * gRes - gyroBias[2];
    
// }

#define ICM20948_I2C_ADDRESS 0x68 << 1 // <<1 for Mbed OS I2C 7bit addressing
#define AK09916_CNTL2 0x31
#define AK09916_CNTL3 0x32
#define AK09916_HXL 0x11

// Magnetometer mode
#define AK09916_MODE_POWER_DOWN 0x00
#define AK09916_MODE_SINGLE_MEASURE 0x01
#define AK09916_MODE_CONTINUOUS_100HZ 0x06
bool Lite9axis::icm20948MagCheck(){
    bool bRet = false;
    uint8_t u8Ret[2];
    
    icm20948ReadSecondary(I2C_ADD_ICM20948_AK09916|I2C_ADD_ICM20948_AK09916_READ,
                                REG_ADD_MAG_WIA1, 2,u8Ret);
    if( (u8Ret[0] == REG_VAL_MAG_WIA1) && ( u8Ret[1] == REG_VAL_MAG_WIA2) )
    {
        bRet = true;
    }
    icm20948WriteSecondary(I2C_ADD_ICM20948_AK09916|I2C_ADD_ICM20948_AK09916_WRITE,
                               REG_ADD_MAG_CNTL2, REG_VAL_MAG_MODE_20HZ);  
    return bRet;
}

void Lite9axis::sen_mag(float *mx,float *my,float *mz)
{
    icm20948MagCheck();
    uint8_t counter = 20;
    uint8_t u8Data[MAG_DATA_LEN];
    int16_t s16Buf[3] = {0}; 
    uint8_t i;
    int32_t s32OutBuf[3] = {0};
    static ICM20948_ST_AVG_DATA sstAvgBuf[3];
    while( counter>0 )
    {
        HAL_Delay(10);
        icm20948ReadSecondary( I2C_ADD_ICM20948_AK09916|I2C_ADD_ICM20948_AK09916_READ, 
                                    REG_ADD_MAG_ST2, 1, u8Data);
        
        if ((u8Data[0] & 0x01) != 0)
            break;
        
        counter--;
    }
    
    if(counter != 0)
    {
        icm20948ReadSecondary( I2C_ADD_ICM20948_AK09916|I2C_ADD_ICM20948_AK09916_READ, 
                                    REG_ADD_MAG_DATA, 
                                    MAG_DATA_LEN,
                                    u8Data);
        s16Buf[0] = ((int16_t)u8Data[1]<<8) | u8Data[0];
        s16Buf[1] = ((int16_t)u8Data[3]<<8) | u8Data[2];
        s16Buf[2] = ((int16_t)u8Data[5]<<8) | u8Data[4];   
        *mx = s16Buf[0]*0.15;
        *my = s16Buf[1]*0.15;
        *mz = s16Buf[2]*0.15;
    }    
    return;
}

// void Lite9axis::getMag(){
//     icm20948MagCheck();
//     icm20948MagRead();
// }

void Lite9axis::test_getMag(){
    char temp;
    printf("test getmag start\r\n");

    ICM_WriteByte(ICM20948_REG_BANK_SEL, USER_BANK_0); //bank0に移動
    // temp = ICM_ReadByte(REG_ADD_USER_CTRL);
    printf("REG_ADD_USER_CTRL: 0x%x\r\n",temp);
    temp |= REG_VAL_BIT_I2C_MST_EN;
    ICM_WriteByte(REG_ADD_USER_CTRL, temp); 
    temp = ICM_ReadByte(REG_ADD_USER_CTRL);
    printf("REG_ADD_USER_CTRL: 0x%x\r\n",temp);

    // I2Cバイパスモードを有効
    temp = I2C_BYPASS_EN;
    ICM_WriteByte(INT_PIN_CFG, temp); 
    temp = ICM_ReadByte(INT_PIN_CFG);
    printf("INT_PIN_CFG: 0x%x\r\n",temp);

    //I2Cマスターのクロック速度を設定
    ICM_WriteByte(ICM20948_REG_BANK_SEL, USER_BANK_3); //bank3に移動
    ICM_WriteByte(I2C_MST_CTRL, 0x07);
    temp = ICM_ReadByte(I2C_MST_CTRL);
    printf("I2C_MST_CTRL: 0x%x\r\n",temp);

    // printf("REG_ADD_USER_CTRL: 0x%x\r\n",temp);
    // wait_ms(5);
    // temp &= ~REG_VAL_BIT_I2C_MST_EN;
    // ICM_WriteByte(REG_ADD_USER_CTRL, temp); 
    // temp = ICM_ReadByte(REG_ADD_USER_CTRL);
    // printf("REG_ADD_USER_CTRL: 0x%x\r\n\r\n",temp);

    ICM_WriteByte(ICM20948_REG_BANK_SEL, USER_BANK_3);
    ICM_WriteByte(REG_ADD_I2C_SLV0_ADDR, I2C_ADD_ICM20948_AK09916|I2C_ADD_ICM20948_AK09916_READ); //slave(AK09916)のアドレス書き込み
    temp = ICM_ReadByte(REG_ADD_I2C_SLV0_ADDR);
    printf("REG_ADD_I2C_SLV0_ADDR: 0x%x\r\n",temp);


    ICM_WriteByte(REG_ADD_I2C_SLV0_REG,  AK09916_CNTL2); // slave(AK09916)の読み込みたいレジスタアドレス書き込み
    temp = ICM_ReadByte(REG_ADD_I2C_SLV0_REG);
    printf("REG_ADD_I2C_SLV0_REG: 0x%x\r\n",temp);

    ICM_WriteByte(REG_ADD_I2C_SLV0_DO, AK09916_MODE_CONTINUOUS_100HZ); // slave(AK09916)のモード設定
    temp = ICM_ReadByte(REG_ADD_I2C_SLV0_DO);
    printf("REG_ADD_I2C_SLV0_DO: 0x%x\r\n",temp);

    int test_reg_bit = REG_VAL_BIT_SLV0_EN|0x01;
    ICM_WriteByte(REG_ADD_I2C_SLV0_CTRL, test_reg_bit); // slave(AK09916)のモード設定
    temp = ICM_ReadByte(REG_ADD_I2C_SLV0_CTRL);
    printf("REG_ADD_I2C_SLV0_CTRL: 0x%x\r\n",temp);

    
    wait_ms(10);
    temp = ICM_ReadByte(REG_ADD_EXT_SENS_DATA_00);
    printf("REG_ADD_EXT_SENS_DATA_00: 0x%x\r\n",REG_ADD_EXT_SENS_DATA_00);
    temp = ICM_ReadByte(REG_ADD_EXT_SENS_DATA_00+1);
    printf("REG_ADD_EXT_SENS_DATA_00+1: 0x%x\r\n",REG_ADD_EXT_SENS_DATA_00+1);


    // char data[2] = {0};
    
    // // バンク0を選択
    // data[0] = ICM20948_REG_BANK_SEL;
    // data[1] = 0x00;
    // _i2c.write(ICM20948_slave_addr, data, 2);
    
    // // I2Cマスターモードを有効化
    // data[0] = USER_CTRL;
    // data[1] = REG_VAL_BIT_I2C_MST_EN;
    // _i2c.write(ICM20948_slave_addr, data, 2);
    
    // // バンク3を選択
    // data[0] = ICM20948_REG_BANK_SEL;
    // data[1] = 0x03;
    // _i2c.write(ICM20948_slave_addr, data, 2);
    // // I2Cマスタークロック速度を設定
    // data[0] = I2C_MST_CTRL;
    // data[1] = 0x07;
    // _i2c.write(ICM20948_slave_addr, data, 2);


    // // AK09916のCNTL2レジスタ（連続測定モード2）を設定
    // data[0] = REG_ADD_I2C_SLV0_ADDR;
    // data[1] = I2C_ADD_ICM20948_AK09916 | 0x80; // 読み取り操作
    // _i2c.write(ICM20948_slave_addr, data, 2);
    
    // data[0] = REG_ADD_I2C_SLV0_REG;
    // data[1] = 0x10; // 磁気データのレジスタアドレス
    // _i2c.write(ICM20948_slave_addr, data, 2);
    
    // // スレーブ0を読み取りモードで有効化
    // data[0] = REG_ADD_I2C_SLV0_CTRL;
    // data[1] = 0x80 | 0x08; // 8バイト読み取り
    // _i2c.write(ICM20948_slave_addr, data, 2);

    // wait_ms(10);

    // char magData[8] = {0};
    // _i2c.write(ICM20948_slave_addr, (char*)REG_ADD_EXT_SENS_DATA_00, 1, true); // レジスタアドレ
    // _i2c.read(ICM20948_slave_addr, magData, 8); 
    // printf("magData[0] = %d ,magData[1] = %d\r\n", magData[0], magData[1]);
}

void Lite9axis::icm20948ReadSecondary(uint8_t u8I2CAddr, uint8_t u8RegAddr, uint8_t u8Len, uint8_t *pu8data)
{
    uint8_t i;
    char u8Temp;
    
    ICM_WriteByte(ICM20948_REG_BANK_SEL, USER_BANK_3); //swtich bank3
    ICM_WriteByte(REG_ADD_I2C_SLV0_ADDR, u8I2CAddr);
    ICM_WriteByte(REG_ADD_I2C_SLV0_REG,  u8RegAddr);
    ICM_WriteByte(REG_ADD_I2C_SLV0_CTRL, REG_VAL_BIT_SLV0_EN|u8Len);

    ICM_WriteByte(ICM20948_REG_BANK_SEL, USER_BANK_0); //swtich bank0
    u8Temp = ICM_ReadByte(USER_CTRL);
    // printf("u8Temp: %c\r\n", u8Temp); //add
    u8Temp |= REG_VAL_BIT_I2C_MST_EN;
    ICM_WriteByte(REG_ADD_USER_CTRL, u8Temp);
    wait_ms(5);
    u8Temp &= ~REG_VAL_BIT_I2C_MST_EN;
    ICM_WriteByte( REG_ADD_USER_CTRL, u8Temp);
    // add
    u8Temp = ICM_ReadByte(USER_CTRL);
    // printf("u8Temp: %c\r\n", u8Temp);
    
    for(i=0; i<u8Len; i++)
    {
        *(pu8data+i) = ICM_ReadByte(REG_ADD_EXT_SENS_DATA_00+i);
        // printf("pu8data+i = %d\r\n",  *(pu8data+i));
        
    }
    ICM_WriteByte(REG_ADD_REG_BANK_SEL, USER_BANK_3); //swtich bank3
    
    u8Temp = ICM_ReadByte(REG_ADD_I2C_SLV0_CTRL);
    u8Temp &= ~((REG_VAL_BIT_I2C_MST_EN)&(REG_VAL_BIT_MASK_LEN));
    ICM_WriteByte( REG_ADD_I2C_SLV0_CTRL,  u8Temp);
    
    ICM_WriteByte( REG_ADD_REG_BANK_SEL, USER_BANK_0); //swtich bank0

}

void Lite9axis::icm20948WriteSecondary(uint8_t u8I2CAddr, uint8_t u8RegAddr, uint8_t u8data)
{
  uint8_t u8Temp;
  ICM_WriteByte(REG_ADD_REG_BANK_SEL,  REG_VAL_REG_BANK_3); //swtich bank3
  ICM_WriteByte(REG_ADD_I2C_SLV1_ADDR, u8I2CAddr);
  ICM_WriteByte(REG_ADD_I2C_SLV1_REG,  u8RegAddr);
  ICM_WriteByte(REG_ADD_I2C_SLV1_DO,   u8data);
  ICM_WriteByte(REG_ADD_I2C_SLV1_CTRL, REG_VAL_BIT_SLV0_EN|1);

  ICM_WriteByte(REG_ADD_REG_BANK_SEL, REG_VAL_REG_BANK_0); //swtich bank0

  u8Temp = ICM_ReadByte(REG_ADD_USER_CTRL);
  u8Temp |= REG_VAL_BIT_I2C_MST_EN;
  ICM_WriteByte(REG_ADD_USER_CTRL, u8Temp);
  HAL_Delay(5);
  u8Temp &= ~REG_VAL_BIT_I2C_MST_EN;
  ICM_WriteByte(REG_ADD_USER_CTRL, u8Temp);

  ICM_WriteByte(REG_ADD_REG_BANK_SEL, REG_VAL_REG_BANK_3); //swtich bank3

  u8Temp = ICM_ReadByte(REG_ADD_I2C_SLV0_CTRL);
  u8Temp &= ~((REG_VAL_BIT_I2C_MST_EN)&(REG_VAL_BIT_MASK_LEN));
  ICM_WriteByte(REG_ADD_I2C_SLV0_CTRL,  u8Temp);

  ICM_WriteByte(REG_ADD_REG_BANK_SEL, REG_VAL_REG_BANK_0); //swtich bank0
    
    return;
}

// void Lite9axis::icm20948MagRead()
// {
//     uint8_t counter = 20;
//     uint8_t u8Data[MAG_DATA_LEN];
//     int16_t s16Buf[3] = {0}; 
//     uint8_t i;
//     int32_t s32OutBuf[3] = {0};
//     static ICM20948_ST_AVG_DATA sstAvgBuf[3];
//     while( counter>0 )
//     {
//         HAL_Delay(10);
//         icm20948ReadSecondary( I2C_ADD_ICM20948_AK09916|I2C_ADD_ICM20948_AK09916_READ, 
//                                     REG_ADD_MAG_ST2, 1, u8Data);
        
//         if ((u8Data[0] & 0x01) != 0)
//             break;
        
//         counter--;
//     }
    
//     if(counter != 0)
//     {
//         icm20948ReadSecondary( I2C_ADD_ICM20948_AK09916|I2C_ADD_ICM20948_AK09916_READ, 
//                                     REG_ADD_MAG_DATA, 
//                                     MAG_DATA_LEN,
//                                     u8Data);
//         s16Buf[0] = ((int16_t)u8Data[1]<<8) | u8Data[0];
//         s16Buf[1] = ((int16_t)u8Data[3]<<8) | u8Data[2];
//         s16Buf[2] = ((int16_t)u8Data[5]<<8) | u8Data[4];   
//         mx = s16Buf[0]*0.15;
//         my = s16Buf[1]*0.15;
//         mz = s16Buf[2]*0.15;
//     }    
//     return;
// }

// void Lite9axis::getMag(){
//     uint8_t counter = 20;
//     uint8_t u8Data[MAG_DATA_LEN];
//     int16_t s16Buf[3] = {0}; 
//     uint8_t i;
//     int32_t s32OutBuf[3] = {0};
//     static ICM20948_ST_AVG_DATA sstAvgBuf[3];
//     while( counter>0 )
//     {
//         wait_ms(10);
//         icm20948ReadSecondary(I2C_ADD_ICM20948_AK09916|I2C_ADD_ICM20948_AK09916_READ, 
//                                     REG_ADD_MAG_ST2, 1, u8Data);
        
//         if ((u8Data[0] & 0x01) != 0)
//             break;
        
//         counter--;
//     }
//     printf("counter=%d\r\n", counter);
//     if(counter != 0)
//     {
//         icm20948ReadSecondary( I2C_ADD_ICM20948_AK09916|I2C_ADD_ICM20948_AK09916_READ, 
//                                     REG_ADD_MAG_DATA, 
//                                     MAG_DATA_LEN,
//                                     u8Data);
//         printf("u8Data[0]: %d, u8Data[1]: %d, u8Data[2]: %d\r\n", u8Data[0], u8Data[1], u8Data[2]); 
//         s16Buf[0] = ((int16_t)u8Data[1]<<8) | u8Data[0];
//         s16Buf[1] = ((int16_t)u8Data[3]<<8) | u8Data[2];
//         s16Buf[2] = ((int16_t)u8Data[5]<<8) | u8Data[4];      
//         printf("s16Buf[0]: %f, s16Buf[1]: %f, s16Buf[2]: %f\r\n", s16Buf[0]*0.15, s16Buf[1]*0.15, s16Buf[2]*0.15); 
//     }
//     // char val;
//     // ICM_WriteByte(ICM20948_REG_BANK_SEL, USER_BANK_3);
//     // I2C_WriteOneByte(REG_ADD_I2C_SLV0_ADDR, u8I2CAddr);
//     // I2C_WriteOneByte(REG_ADD_I2C_SLV0_REG,  u8RegAddr);
//     // // ICM_WriteByte(AK09916_CNTL3, 0x01); //reset mag
//     // // val = ICM_ReadByte(AK09916_CNTL3);
//     // // printf("CNTL3: 0x%x\r\n", val);

//     // ICM_WriteByte(AK09916_CNTL2, AK09916_MODE_CONTINUOUS_100HZ); //reset mag
//     // val = ICM_ReadByte(AK09916_CNTL2);
//     // printf("CNTL2: 0x%x\r\n", val);
//     // int16_t _mx;
//     // uint8_t LoByte, HiByte;

//     // LoByte = ICM_ReadByte(ICM20948_GYRO_ZOUT_L);
//     // HiByte = ICM_ReadByte(ICM20948_GYRO_ZOUT_H);

//     // _mx = (HiByte<<8) | LoByte;
//     // mx = ((float)(_mx)) * mRes;


//     // char cmd[2], data[7],val;
//     // // Reset magnetometer
//     // cmd[0] = AK09916_CNTL3;
//     // cmd[1] = 0x01; // Reset
//     // _i2c.write(ICM20948_slave_addr, cmd, 2);
//     // val = _i2c.read(ICM20948_I2C_ADDRESS, data, 2);
//     // printf("CNTL3: 0x%x\r\n", val);

//     // // printf("CNTL2: 0x%x\r\n", cmd[1]);
//     // wait(0.1); // Wait for reset to complete
    
//     // Set to continuous measurement mode at 100Hz
//     // cmd[0] = AK09916_CNTL2;
//     // cmd[1] = AK09916_MODE_CONTINUOUS_100HZ;
//     // _i2c.write(ICM20948_slave_addr, cmd, 2);
//     // val = _i2c.read(ICM20948_slave_addr, data, 2);
//     // printf("CNTL2: 0x%x\r\n", val);

//     // char reg = AK09916_HXL;
//     // _i2c.write(ICM20948_slave_addr, &reg, 1, true); // Set the register pointer to HXL
//     // _i2c.read(ICM20948_slave_addr, data, 7); // Read 6 bytes data + 1 status byte
//     // mx = (int16_t)(data[1] << 8 | data[0]);
//     // my = (int16_t)(data[3] << 8 | data[2]);
//     // mz = (int16_t)(data[5] << 8 | data[4]);
//     // printf("mx: %f, my: %f, mz: %f\r\n",mx, my, mz);


//     // char data[7]; // 6 bytes data + 1 byte status
//     // char reg = AK09916_HXL;
//     // i2c.write(ICM20948_I2C_ADDRESS, &reg, 1, true); // Set the register pointer to HXL
//     // i2c.read(ICM20948_I2C_ADDRESS, data, 7); // Read 6 bytes data + 1 status byte
    
//     // x = (int16_t)(data[1] << 8 | data[0]);
//     // y = (int16_t)(data[3] << 8 | data[2]);
//     // z = (int16_t)(data[5] << 8 | data[4]);

//     // int16_t _mx, _my, _mz;
//     // uint8_t LoByte, HiByte, test;
//     // // char cmd[2], data_in[1];
    
//     // char data_o[1], data_i[1];
//     // data_o[0] = 0x31;
//     // _i2c.write(I2C_ADD_ICM20948_AK09916, data_o, 1, 1);
//     // _i2c.read(I2C_ADD_ICM20948_AK09916, data_i, 1, 0);
//     // printf("CNTL2: 0x%x\r\n", data_i[0]);

//     // cmd[0] = 0x31;
//     // _i2c.write(I2C_ADD_ICM20948_AK09916, cmd, 1);
//     // _i2c.read(I2C_ADD_ICM20948_AK09916, data_in, 1);
//     // printf("CNTL2: %d\r\n", cmd[0]);
//     // cmd[0] = 0x31;
//     // cmd[1] = 0x02;
//     // _i2c.write(I2C_ADD_ICM20948_AK09916, cmd, 2);
//     // _i2c.read(I2C_ADD_ICM20948_AK09916, data_in, 1);
//     // printf("CNTL2: %d\r\n", cmd[0]);
//     // LoByte = ICM_ReadByte_(I2C_ADD_ICM20948_AK09916,ICM20948_MAG_XOUT_L); // read Gyrometer X_Low  value
//     // HiByte = ICM_ReadByte_(I2C_ADD_ICM20948_AK09916,ICM20948_MAG_XOUT_H); // read Gyrometer X_High value
//     // _mx = (HiByte<<8) | LoByte;
//     // mRes = 0.15;
//     // mx = ((float)(_mx)) * mRes;

//     // uint8_t counter = 20;
//     // uint8_t u8Data[6];
//     // int16_t s16Buf[3] = {0}; 
//     // uint8_t i;
//     // int32_t s32OutBuf[3] = {0};
//     // static ICM20948_ST_AVG_DATA sstAvgBuf[3];
//     // while( counter>0 )
//     // {
//     //     wait_ms(10);
//     //     char data_out[1], data_in[1];
//     //     data_out[0] = REG_ADD_MAG_ST2;
//     //     _i2c.write(I2C_ADD_ICM20948_AK09916|I2C_ADD_ICM20948_AK09916_READ, data_out, 1, 1);
//     //     _i2c.read(I2C_ADD_ICM20948_AK09916|I2C_ADD_ICM20948_AK09916_READ, data_in, 1, 0);
//     //     return (data_in[0]);

//     //     icm20948ReadSecondary( I2C_ADD_ICM20948_AK09916|I2C_ADD_ICM20948_AK09916_READ, 
//     //                                 REG_ADD_MAG_ST2, 1, u8Data);
        
//     //     if ((u8Data[0] & 0x01) != 0)
//     //         break;
        
//     //     counter--;
//     // }
// }

// float Lite9axis::getAX()
// {
//     return ax;
// }

// float Lite9axis::getAY()
// {
//     return ay;
// }

// float Lite9axis::getAZ()
// {
//     return az;
// }

// float Lite9axis::getGX()
// {
//     return gx;
// }

// float Lite9axis::getGY()
// {
//     return gy;
// }

// float Lite9axis::getGZ()
// {
//     return gz;
// }

// float Lite9axis::getMX()
// {
//     return mx;
// }

// float Lite9axis::getMY()
// {
//     return my;
// }

// float Lite9axis::getMZ()
// {
//     return mz;
// }
// int ICM20948::getIMUTemp()
// {
//     uint8_t LoByte, HiByte;
//     LoByte = ICM_ReadByte(ICM20948_TEMP_OUT_L); // read Accelerometer X_Low  value
//     HiByte = ICM_ReadByte(ICM20948_TEMP_OUT_H); // read Accelerometer X_High value
//     return ((HiByte<<8) | LoByte);
// }
            