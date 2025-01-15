/**
 * @file rbf_thread.c
 * @author Jio (hedajun@hzdusun.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "rbf_thread.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

int rbf_thread_create(const char* name, size_t iThreadStackSize, 
                                void *pThreadFunc, 
                                void *pTreadArg)
{
    BaseType_t xReturned = xTaskCreate(
                    pThreadFunc,       /* Function that implements the task. */
                    name,             /* Text name for the task. */
                    iThreadStackSize/4,      /* Stack size in words, not bytes. */
                    pTreadArg,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,/* Priority at which the task is created. */
                    NULL );      /* Used to pass out the created task's handle. */

    if( xReturned == pdPASS ) {
        return -1;
    } else {
        return 0;
    }
}



void rbf_thread_sleep(uint32_t timeoutMs)
{
    const TickType_t xDelay = timeoutMs / portTICK_PERIOD_MS;
     vTaskDelay( xDelay );
}