#ifndef __HARDWARE_H
#define __HARDWARE_H

#include "main.h"
#include "tim.h"
#include "analog.h"
#include "encoder.h"
#include "imu.h"
#include "gps.h"
#include "stm32f429i_discovery.h"

#define USB_MAX_BUFF 2048
#define RED_LED LED4
#define GREEN_LED LED3

void hardware_init(void);
void led_init(void);
void user_button_init(void);
void time_base_init(void);
void led_test(void);
void system_clock_config(void);

uint32_t timer_get_current_tick(void);
void timer_update_callback(void);

void usb_callback(uint8_t *buf, uint32_t len);

void error_handler(void);

#endif /* __HARDWARE_H */