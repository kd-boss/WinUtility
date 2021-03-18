#include <cstdint>

enum class mpu6050Reg : std::int8_t {
    SELF_TEST_X = 0x0D,
    SELF_TEST_Y,
    SELF_TEST_Z,
    SELF_TEST_A,
    SMPLRT_DIV = 0x19,
    CONFIG = 0x1A,
    GYRO_CONFIG,
    ACCEL_CONFIG,
    FIFO_EN = 0x23,
    I2C_MST_CTRL,
    I2C_SLV0_ADDR,
    I2C_SLV0_REG,
    I2C_SLV0_CTRL,
    I2C_SLV1_ADDR,
    I2C_SLV1_REG,
    I2C_SLV1_CTRL,
    I2C_SLV2_ADDR,
    I2C_SLV2_REG,
    I2C_SLV2_CTRL,
    I2C_SLV3_ADDR,
    I2C_SLV3_REG,
    I2C_SLV2_CTRL,
    I2C_SLV4_ADDR,
    I2C_SLV4_REG,
    I2C_SLV4_D0,
    I2C_SLV4_CTRL,
    I2C_SLV4_DI,
    I2C_MST_STATUS,
    INT_PIN_CFG,
    INT_ENABLE,
    INT_STATUS = 0X3A,
    ACCEL_XOUT_H,
    ACCEL_XOUT_L,
    ACCEL_YOUT_H,
    ACCEL_YOUT_L,
    ACCEL_ZOUT_H,
    ACCEL_ZOUT_L,
    TEMP_OUT_H,
    TEMP_OUT_L,
    GYRO_XOUT_H,
    GYRO_XOUT_L,
    GYRO_YOUT_H,
    GYRO_YOUT_L,
    GYRO_ZOUT_H,
    GYRO_ZOUT_L,
    EXT_SENSE_DATA_00,
    EXT_SENSE_DATA_01,
    EXT_SENSE_DATA_02,
    EXT_SENSE_DATA_03,
    EXT_SENSE_DATA_04,
    EXT_SENSE_DATA_05,
    EXT_SENSE_DATA_06,
    EXT_SENSE_DATA_07
    EXT_SENSE_DATA_08,
    EXT_SENSE_DATA_09,
    EXT_SENSE_DATA_10,
    EXT_SENSE_DATA_11,
    EXT_SENSE_DATA_12
    EXT_SENSE_DATA_13,
    EXT_SENSE_DATA_14,
    EXT_SENSE_DATA_15,
    EXT_SENSE_DATA_16,
    EXT_SENSE_DATA_17
    EXT_SENSE_DATA_18,
    EXT_SENSE_DATA_19,
    EXT_SENSE_DATA_20,
    EXT_SENSE_DATA_21,
    EXT_SENSE_DATA_22,
    EXT_SENSE_DATA_23,
    I2C_SLV0_DO = 0X63,
    I2C_SLV1_DO,
    I2C_SLV2_DO,
    I2C_SLV3_DO,
    I2C_MST_DELAY_CT,
    SIGNAL_PATH_RES,
    USER_CTRL = 0X6A,
    PWR_MGMT_1,
    PWR_MGMT_2,
    FIFO_COUNTH = 0X72,
    FIFO_COUNTL,
    FIFO_R_W,
    WHO_AM_I
};

std::int8_t 
mpu6050Reset(mpu6050Reg reg) {
    switch(reg)
    {
        case mpu6050Reg::PWR_MGMT_1:
            return 0x40;
        case mpu6050Reg::WHO_AM_I:
            return 0x68;
        default:
            return 0x00;
    }    
}