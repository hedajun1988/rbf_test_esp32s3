/**
 * @file rbf_time.c
 * @author Jio (hedajun@hzdusun.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "rbf_time.h"
#include "freertos/FreeRTOS.h"

void rbf_time_get_ms(rbf_time_t* time)
{
    *time = xTaskGetTickCount() * portTICK_PERIOD_MS;
}