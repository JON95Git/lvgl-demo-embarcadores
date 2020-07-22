#include "encoder.h"

volatile int32_t encoder_value;
bool encoder_btn_flag = false;
bool user_btn_flag = false;

void encoder_init(void)
{
    HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_1);
    HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_2);
    HAL_TIM_Encoder_Start_IT(&htim2, TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start_IT(&htim2, TIM_CHANNEL_2);
}

void encoder_callback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) {
        encoder_value = TIM2->CNT;
    }
}

void encoder_button_callback(uint16_t GPIO_Pin)
{
	static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = timer_get_current_tick();
    
    if (GPIO_Pin == ENCODER_SW_Pin) {
		if (interrupt_time - last_interrupt_time > 300) {
            encoder_btn_flag = true;
		}
	}
	last_interrupt_time = interrupt_time;
}