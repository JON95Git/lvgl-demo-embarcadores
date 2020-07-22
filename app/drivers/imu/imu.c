#include "imu.h"

static MPU6050_t MPU6050;
static volatile uint32_t timer = 0;

static osMutexDef (mutex);
static osMutexId  (mutex_id);

uint8_t imu_init(void)
{
    uint8_t ret = 0;

    mutex_id = osMutexCreate(osMutex(mutex));

    /* Initialize gyroscope/accelerometer */
    ret = MPU6050_Init(&IMU_I2C_HANDLE);

    return ret;
}

void imu_get_pitch_and_roll(int16_t *pitch, int16_t *roll)
{
    osMutexWait(mutex_id, osWaitForever);
    MPU6050_Read_All(&IMU_I2C_HANDLE, &MPU6050);
    /* Convert from float to int16_t */
    *pitch = (int16_t)(MPU6050.KalmanAngleX + 0.5);
    *roll = (int16_t)(MPU6050.KalmanAngleY + 0.5);
    osMutexRelease(mutex_id);
}

void imu_read(void)
{
    osMutexWait(mutex_id, osWaitForever);
    MPU6050_Read_All(&IMU_I2C_HANDLE, &MPU6050);
    osMutexRelease(mutex_id);
}

void imu_check_who_i_am(uint8_t *who_i_am)
{
    uint8_t check;
    osMutexWait(mutex_id, osWaitForever);
    MPU6050_who_i_am_check(&IMU_I2C_HANDLE, &check);
    osMutexRelease(mutex_id);
    *who_i_am = (uint8_t)check;
}

void imu_timer_callback(void)
{
    ++timer;
    if (timer >= 10) {
        osSignalSet(imuTaskHandle, IMU_TASK_SIGNAL);
        timer = 0;
    }
}