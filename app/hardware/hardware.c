#include "hardware.h"

static volatile uint32_t timer_counter = 0;
static uint8_t *usb_buffer[USB_MAX_BUFF];

void hardware_init(void)
{
    uint8_t ret = 0;
    led_init();
    led_test();
    user_button_init();
    encoder_init();
    ret = imu_init();
    if (ret != 0) {
        error_handler();
    }
    time_base_init();
}

void led_init(void)
{
    BSP_LED_Init(GREEN_LED);
    BSP_LED_Init(RED_LED);
}

void user_button_init(void)
{
    BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
}

void time_base_init(void)
{
    HAL_TIM_Base_Start_IT(&htim7);
}

void led_test(void)
{
    for (int i = 0; i < 8; i++) {
        BSP_LED_Toggle(GREEN_LED);
        HAL_Delay(50);
    }
}

void system_clock_config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    /** Configure the main internal regulator output voltage  */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the CPU, AHB and APB busses clocks  */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 360;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    /* Activate the Over-Drive mode */
    HAL_PWREx_EnableOverDrive();

    /** Initializes the CPU, AHB and APB busses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | 
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
    PeriphClkInitStruct.PLLSAI.PLLSAIR = 4;
    PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_8;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
}

void usb_callback(uint8_t *buf, uint32_t len)
{
    if (len <= USB_MAX_BUFF) {
        memcpy(usb_buffer, buf, len);
    }
}

uint32_t timer_get_current_tick(void)
{
	return (timer_counter);
}

void timer_update_callback(void)
{
	++timer_counter;
}