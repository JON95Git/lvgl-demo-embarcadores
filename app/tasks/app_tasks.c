#include "app_tasks.h"

/* Mutexes */
osMutexDef (lvgl_mutex);
osMutexId  (lvgl_mutex_id);
osMutexDef (gps_mutex);
osMutexId  (gps_mutex_id);

/* Tasks handlers */
osThreadId imuTaskHandle;
osThreadId lvglTaskHandle;
osThreadId gpsTaskHandle;
osThreadId analogTaskHandle;

void app_create_tasks(void)
{
    osThreadDef(gpsTask, gps_task, osPriorityNormal, 0, 1024);
    gpsTaskHandle = osThreadCreate(osThread(gpsTask), NULL);

    osThreadDef(lvglTask, lvgl_task, osPriorityHigh, 0, 2048);
    lvglTaskHandle = osThreadCreate(osThread(lvglTask), NULL);
    
    osThreadDef(imuTask, imu_task, osPriorityNormal, 0, 512);
    imuTaskHandle = osThreadCreate(osThread(imuTask), NULL);

    osThreadDef(analogTask, analog_task, osPriorityNormal, 0, 512);
    analogTaskHandle = osThreadCreate(osThread(analogTask), NULL);
}

void lvgl_task(void const * argument)
{
    lvgl_mutex_id = osMutexCreate(osMutex(lvgl_mutex));
    for(;;) {
        osDelay(5);
        osMutexWait(lvgl_mutex_id, osWaitForever);
        lv_task_handler();
        osMutexRelease(lvgl_mutex_id);
    }
}

void imu_task(void const * argument)
{
    osEvent evt;
    for (;;) {
        evt = osSignalWait(IMU_TASK_SIGNAL, osWaitForever);
        if (evt.status == osEventSignal)  {
            imu_read();
		}
    }
}

void gps_task(void const * argument)
{
    osEvent evt;
    gps_mutex_id = osMutexCreate(osMutex(gps_mutex));
    gps_init();
    for (;;) {
        evt = osSignalWait(GPS_TASK_SIGNAL, osWaitForever);
        if (evt.status == osEventSignal)  {
            osMutexWait(gps_mutex_id, osWaitForever);
            gps_minmea_process();
            osMutexRelease(gps_mutex_id);
        }
    }
}

void analog_task(void const * argument)
{
    osEvent evt;
    analog_init();
    analog_start();
    for (;;) {
        evt = osSignalWait(ANALOG_TASK_SIGNAL, osWaitForever);
        if (evt.status == osEventSignal)  {
            analog_process();
        }
    }
}