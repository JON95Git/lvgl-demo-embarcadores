#ifndef __IMU_H
#define __IMU_H

#include "stdint.h"
#include "stdio.h"
#include "cmsis_os.h"
#include "string.h"
#include "mpu6050.h"
#include "i2c.h"
#include <math.h>

#define	IMU_TASK_SIGNAL 0x0002
#define IMU_I2C_HANDLE hi2c3

extern osThreadId imuTaskHandle;

uint8_t imu_init(void);
void imu_read(void);
void imu_timer_callback(void);
void imu_get_pitch_and_roll(int16_t *pitch, int16_t *roll);
void imu_check_who_i_am(uint8_t *x_int);

#endif /* __IMU_H */