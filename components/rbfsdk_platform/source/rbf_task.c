/**
 * @file rbf_task.c
 * @author Jio (hedajun@hzdusun.com)
 * @brief 
 * @version 0.1
 * @date 2025-02-11
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "rbf_task.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


unsigned long rbf_task_get_high_watermark(void)
{
    UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
    return (unsigned long)uxHighWaterMark;
}