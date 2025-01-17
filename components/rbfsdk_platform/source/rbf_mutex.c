/**
 * @file rbf_mutex.c
 * @author Jio (hedajun@hzdusun.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "rbf_mutex.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

rbf_mutex_t rbf_mutex_create()
{
    return xSemaphoreCreateMutex();
}


void rbf_mutex_lock(rbf_mutex_t mutex)
{
    xSemaphoreTake( mutex, portMAX_DELAY );
}

void rbf_mutex_unlock(rbf_mutex_t mutex)
{
    xSemaphoreGive( mutex );
}