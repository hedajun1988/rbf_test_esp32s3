/**
 * @file rbf_queue.c
 * @author Jio (hedajun@hzdusun.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "rbf_queue.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

rbf_queue_t rbf_queue_create(int count, size_t size)
{
    return xQueueCreate( count, size); 
}


int rbf_queue_send(rbf_queue_t queue, void* data, int timeoutMs)
{
    const TickType_t xTicksToWait = timeoutMs / portTICK_PERIOD_MS;

    if (pdPASS !=  xQueueSend( queue, data, xTicksToWait )) {
        return -1;
    } else {
        return 0;
    }
}


int rbf_queue_receive(rbf_queue_t queue, void* data, int timeoutMs)
{
    const TickType_t xTicksToWait = timeoutMs / portTICK_PERIOD_MS;

    if( xQueueReceive( queue, data, xTicksToWait ) != pdPASS ) {
        return -1;
    } else {
        return 0;
    }
}