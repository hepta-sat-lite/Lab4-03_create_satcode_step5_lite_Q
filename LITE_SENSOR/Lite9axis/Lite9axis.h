#ifndef ICM20948_H
#define ICM20948_H

#include "mbed.h"
 
// USER BANK 0
#define ICM20948_WHO_AM_I         0x00 // Should return 0xEA
#define ICM20948_MST_CTRL         0x01 // Should return 0xEA
#define ICM20948_PWR_MGMT_1       0x06  // Device defaults to the SLEEP mode
#define ICM20948_PWR_MGMT_2       0x07
#define INT_PIN_CFG               0x0F
    #define I2C_BYPASS_EN          0x02
//-> added part ---
#define USER_CTRL                 0x03
#define REG_ADD_USER_CTRL       0x03
    #define REG_VAL_BIT_DMP_EN          0x80
    #define REG_VAL_BIT_FIFO_EN         0x40
    #define REG_VAL_BIT_I2C_MST_EN      0x20
    #define REG_VAL_BIT_I2C_IF_DIS      0x10
    #define REG_VAL_BIT_DMP_RST         0x08
    #define REG_VAL_BIT_DIAMOND_DMP_RST 0x04
#define I2C_ADD_ICM20948_AK09916  0x0C  
#define I2C_ADD_ICM20948_AK09916_READ  0x80
#define I2C_ADD_ICM20948_AK09916_WRITE 0x00
#define REG_ADD_MAG_ST2           0x10
#define REG_ADD_MAG_DATA          0x11
#define ICM20948_MAG_XOUT_L       0x11
#define ICM20948_MAG_XOUT_H       0x12
#define ICM20948_MAG_YOUT_L       0x13
#define ICM20948_MAG_YOUT_H       0x14
#define ICM20948_MAG_ZOUT_L       0x15
#define ICM20948_MAG_XOUT_H       0x16
#define REG_ADD_EXT_SENS_DATA_00  0x3B
//---
#define ICM20948_ACCEL_XOUT_H     0x2D
#define ICM20948_ACCEL_XOUT_L     0x2E
#define ICM20948_ACCEL_YOUT_H     0x2F
#define ICM20948_ACCEL_YOUT_L     0x30
#define ICM20948_ACCEL_ZOUT_H     0x31
#define ICM20948_ACCEL_ZOUT_L     0x32
#define ICM20948_GYRO_XOUT_H      0x33
#define ICM20948_GYRO_XOUT_L      0x34
#define ICM20948_GYRO_YOUT_H      0x35
#define ICM20948_GYRO_YOUT_L      0x36
#define ICM20948_GYRO_ZOUT_H      0x37
#define ICM20948_GYRO_ZOUT_L      0x38
#define ICM20948_TEMP_OUT_H       0x39
#define ICM20948_TEMP_OUT_L       0x3A
 
// USER BANK 2
#define ICM20948_GYRO_SMPLRT_DIV  0x00
#define ICM20948_GYRO_CONFIG_1    0x01
#define ICM20948_ACCEL_SMPLRT_DIV_2 0x11
#define ICM20948_ACCEL_CONFIG     0x14

// USER BANK 3
#define I2C_MST_CTRL            0x01
#define REG_ADD_I2C_SLV0_ADDR   0x03
#define REG_ADD_I2C_SLV0_REG    0x04
#define REG_ADD_I2C_SLV0_CTRL   0x05
    #define REG_VAL_BIT_SLV0_EN     0x80
    #define REG_VAL_BIT_MASK_LEN    0x07
#define REG_ADD_I2C_SLV0_DO     0x06
#define REG_ADD_I2C_SLV1_ADDR   0x07
#define REG_ADD_I2C_SLV1_REG    0x08
#define REG_ADD_I2C_SLV1_CTRL   0x09
#define REG_ADD_I2C_SLV1_DO     0x0A
 
// COMMON
#define ICM20948_REG_BANK_SEL     0x7F
#define REG_ADD_REG_BANK_SEL      0x7F
 
// OTHER
#define USER_BANK_0               0b00000000
#define USER_BANK_1               0b00010000
#define USER_BANK_2               0b00100000
#define USER_BANK_3               0b00110000
 

#define GYRO_RATE_250             0x00
#define GYRO_RATE_500             0x02
#define GYRO_RATE_1000            0x04
#define GYRO_RATE_2000            0x06
#define GYRO_LPF_OFF              0x00
#define GYRO_LPF_230Hz            0x01
#define GYRO_LPF_17Hz             0x29
#define GYRO_SMPLRT_100Hz         0x0A
 
// 加速度のレンジは2:1bitの値に相当
// 加速度のモジュール内ローパスフィルタは5:3,0bitに相当
#define ACC_RATE_2g               0x00
#define ACC_RATE_4g               0x02
#define ACC_RATE_8g               0x04
#define ACC_RATE_16g              0x06
#define ACC_LPF_OFF               0x00
#define ACC_LPF_136HZ             0x11
#define ACC_SMPLRT_100Hz          0x0A

/* define ICM-20948 Device I2C address*/
#define I2C_ADD_ICM20948            0xD0
#define I2C_ADD_ICM20948_AK09916    0x0C
#define I2C_ADD_ICM20948_AK09916_READ  0x80
#define I2C_ADD_ICM20948_AK09916_WRITE 0x00
/* define ICM-20948 Register */
/* user bank 0 register */
#define REG_ADD_WIA             0x00
    #define REG_VAL_WIA             0xEA
#define REG_ADD_USER_CTRL       0x03
    #define REG_VAL_BIT_DMP_EN          0x80
    #define REG_VAL_BIT_FIFO_EN         0x40
    #define REG_VAL_BIT_I2C_MST_EN      0x20
    #define REG_VAL_BIT_I2C_IF_DIS      0x10
    #define REG_VAL_BIT_DMP_RST         0x08
    #define REG_VAL_BIT_DIAMOND_DMP_RST 0x04
#define REG_ADD_PWR_MIGMT_1     0x06
    #define REG_VAL_ALL_RGE_RESET   0x80
    #define REG_VAL_RUN_MODE        0x01    //Non low-power mode
#define REG_ADD_LP_CONFIG       0x05
#define REG_ADD_PWR_MGMT_1      0x06
#define REG_ADD_PWR_MGMT_2      0x07
#define REG_ADD_ACCEL_XOUT_H    0x2D
#define REG_ADD_ACCEL_XOUT_L    0x2E
#define REG_ADD_ACCEL_YOUT_H    0x2F
#define REG_ADD_ACCEL_YOUT_L    0x30
#define REG_ADD_ACCEL_ZOUT_H    0x31
#define REG_ADD_ACCEL_ZOUT_L    0x32
#define REG_ADD_GYRO_XOUT_H     0x33
#define REG_ADD_GYRO_XOUT_L     0x34
#define REG_ADD_GYRO_YOUT_H     0x35
#define REG_ADD_GYRO_YOUT_L     0x36
#define REG_ADD_GYRO_ZOUT_H     0x37
#define REG_ADD_GYRO_ZOUT_L     0x38
#define REG_ADD_EXT_SENS_DATA_00 0x3B
#define REG_ADD_REG_BANK_SEL    0x7F
    #define REG_VAL_REG_BANK_0  0x00
    #define REG_VAL_REG_BANK_1  0x10
    #define REG_VAL_REG_BANK_2  0x20
    #define REG_VAL_REG_BANK_3  0x30

/* user bank 1 register */
/* user bank 2 register */
#define REG_ADD_GYRO_SMPLRT_DIV 0x00
#define REG_ADD_GYRO_CONFIG_1   0x01
    #define REG_VAL_BIT_GYRO_DLPCFG_2   0x10 /* bit[5:3] */
    #define REG_VAL_BIT_GYRO_DLPCFG_4   0x20 /* bit[5:3] */
    #define REG_VAL_BIT_GYRO_DLPCFG_6   0x30 /* bit[5:3] */
    #define REG_VAL_BIT_GYRO_FS_250DPS  0x00 /* bit[2:1] */
    #define REG_VAL_BIT_GYRO_FS_500DPS  0x02 /* bit[2:1] */
    #define REG_VAL_BIT_GYRO_FS_1000DPS 0x04 /* bit[2:1] */
    #define REG_VAL_BIT_GYRO_FS_2000DPS 0x06 /* bit[2:1] */    
    #define REG_VAL_BIT_GYRO_DLPF       0x01 /* bit[0]   */
#define REG_ADD_ACCEL_SMPLRT_DIV_2  0x11
#define REG_ADD_ACCEL_CONFIG        0x14
    #define REG_VAL_BIT_ACCEL_DLPCFG_2  0x10 /* bit[5:3] */
    #define REG_VAL_BIT_ACCEL_DLPCFG_4  0x20 /* bit[5:3] */
    #define REG_VAL_BIT_ACCEL_DLPCFG_6  0x30 /* bit[5:3] */
    #define REG_VAL_BIT_ACCEL_FS_2g     0x00 /* bit[2:1] */
    #define REG_VAL_BIT_ACCEL_FS_4g     0x02 /* bit[2:1] */
    #define REG_VAL_BIT_ACCEL_FS_8g     0x04 /* bit[2:1] */
    #define REG_VAL_BIT_ACCEL_FS_16g    0x06 /* bit[2:1] */    
    #define REG_VAL_BIT_ACCEL_DLPF      0x01 /* bit[0]   */

/* user bank 3 register */
#define REG_ADD_I2C_SLV0_ADDR   0x03
#define REG_ADD_I2C_SLV0_REG    0x04
#define REG_ADD_I2C_SLV0_CTRL   0x05
    #define REG_VAL_BIT_SLV0_EN     0x80
    #define REG_VAL_BIT_MASK_LEN    0x07
#define REG_ADD_I2C_SLV0_DO     0x06
#define REG_ADD_I2C_SLV1_ADDR   0x07
#define REG_ADD_I2C_SLV1_REG    0x08
#define REG_ADD_I2C_SLV1_CTRL   0x09
#define REG_ADD_I2C_SLV1_DO     0x0A

/* define ICM-20948 Register  end */
/* define ICM-20948 MAG Register  */
#define REG_ADD_MAG_WIA1    0x00
    #define REG_VAL_MAG_WIA1    0x48
#define REG_ADD_MAG_WIA2    0x01
    #define REG_VAL_MAG_WIA2    0x09
#define REG_ADD_MAG_ST2     0x10
#define REG_ADD_MAG_DATA    0x11
#define REG_ADD_MAG_CNTL2   0x31
    #define REG_VAL_MAG_MODE_PD     0x00
    #define REG_VAL_MAG_MODE_SM     0x01
    #define REG_VAL_MAG_MODE_10HZ   0x02
    #define REG_VAL_MAG_MODE_20HZ   0x04
    #define REG_VAL_MAG_MODE_50HZ   0x05
    #define REG_VAL_MAG_MODE_100HZ  0x08
    #define REG_VAL_MAG_MODE_ST     0x10
/* define ICM-20948 MAG Register  end */

#define MAG_DATA_LEN              6
#define REG_ADD_MAG_WIA1          0x00
#define REG_VAL_MAG_WIA1          0x48
#define REG_VAL_MAG_WIA2          0x09
// Using the GY-521 breakout board, I set ADO to 0 by grounding through a 4k7 resistor
// Seven-bit device addrss is 110100 for ADO = 0 and 110101 for ADO = 1
#define ADO 0
#if ADO
#define ICM20948_slave_addr 0x69<<1  // Device address when ADO = 1
#else
#define ICM20948_slave_addr 0x68<<1  // Device address when ADO = 0
#endif
#define IMU_ONE_G 9.80665

#define ACCEL_DIV       1
#define ACCEL_LP        0
#define ACCEL_LP_EN     1
#define ACCEL_FS        1
#define GYRO_DIV        1
#define GYRO_LP         0
#define GYRO_LP_EN      1
#define GYRO_FS         1
// #define accel_lp        0
// #define accel_lp_en     1
// #define accel_fs        1
// #define gyro_div        1
// #define gyro_lp         0
// #define gyro_lp_en      1
// #define gyro_fs         1

#define ACCEL_CONF  (ACCEL_LP_EN | (ACCEL_FS << 1) | (ACCEL_LP << 3))
#define GYRO_CONF   (GYRO_LP_EN | (GYRO_FS << 1) | (GYRO_LP << 3))

 
class Lite9axis {
    public:
        Lite9axis(PinName sda, PinName sck);
        Lite9axis(I2C &i2c);  
        void ICM_WriteByte(char ICM20948_reg, char ICM20948_data);
        char ICM_ReadByte(char ICM20948_reg);
        char ICM_ReadByte_(char slave_addr ,char ICM20948_reg);
        int  whoAmI();
        void reset();
        void powerOn();
        // void init(char accel_conf, char accel_div, char gyro_conf, char gyro_div);
        // void init();
        void set_up();
        void gyroCalib();
        // void getAccGyro();
        void sen_acc(float *ax,float *ay,float *az);
        void sen_gyro(float *gx,float *gy,float *gz);
        void sen_mag(float *mx,float *my,float *mz);
        void test_getMag();
        void icm20948ReadSecondary(uint8_t u8I2CAddr, uint8_t u8RegAddr, uint8_t u8Len, uint8_t *pu8data);
        void icm20948WriteSecondary(uint8_t u8I2CAddr, uint8_t u8RegAddr, uint8_t u8data);
        bool icm20948MagCheck();
        void icm20948MagRead(float *mx,float *my,float *mz);
        // void getMag();
        int  getIMUTemp();
        // float getAX();
        // float getAY();
        // float getAZ();
        // float getGX();
        // float getGY();
        // float getGZ();
        // float getMX();
        // float getMY();
        // float getMZ();
    private:
        I2C _i2c;
        float aRes, gRes, mRes;
        float gyroBias[3], acc[3], gyro[3], mag[3];
        // float ax, ay, az, gx, gy, gz, mx, my, mz;
        float mx, my, mz;
};

typedef struct icm20948_st_avg_data_tag
{
  uint8_t u8Index;
  int16_t s16AvgBuffer[8];
}ICM20948_ST_AVG_DATA;

#endif
            