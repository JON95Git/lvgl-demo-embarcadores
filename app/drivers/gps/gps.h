#ifndef __GPS_H
#define __GPS_H

#include <string.h>
#include "cmsis_os.h"
#include "usart.h"
#include "minmea.h"
#include "system.h"

typedef enum {
    GPS_PARSE_ERR_SUCESS = 0,
    GPS_PARSE_ERR_FAILED,
} _gps_parse_err_t;

#define GPS_USART_INSTANCE USART1
#define	GPS_USART huart1
#define	GPS_USART_IRQn USART1_IRQn
#define GPS_DMA_IRQn DMA2_Stream2_IRQn
#define	GPS_TASK_SIGNAL 0x0001

// #define GPS_DEBUG

extern osThreadId gpsTaskHandle;

void gps_init(void);
void gps_minmea_process_blocking_mode(void);
void gps_minmea_process(void);
void gps_get_parsed_data(float *latitude, float *longitude, int32_t *altitude, int32_t *tracked_satellites);

void gps_callback(UART_HandleTypeDef *huart);
void gps_callback_error(UART_HandleTypeDef *huart);

#endif /* __GPS_H */
