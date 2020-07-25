#ifndef __ENCODER_H
#define __ENCODER_H

#include <stdbool.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f429i_discovery.h"
#include "main.h"
#include "tim.h"
#include "hardware.h"

#define ENCODER_DEBOUNCE_TIME 300

extern volatile int32_t encoder_value;
extern bool encoder_btn_flag;
extern bool user_btn_flag;

void encoder_init(void);
void encoder_callback(TIM_HandleTypeDef *htim);
void encoder_button_callback(uint16_t GPIO_Pin);

#endif /*__ENCODER_H */