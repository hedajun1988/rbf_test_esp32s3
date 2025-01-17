/**
 * @file rbf_event.c
 * @author Jio (hedajun@hzdusun.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "rbf_event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

rbf_event_group_hanle_t rbf_event_group_create()
{
    return xEventGroupCreate();
}

void rbf_event_group_set_bits(rbf_event_group_hanle_t group, uint32_t bits)
{
    xEventGroupSetBits(group,  bits);
}

void rbf_event_group_clear_bits(rbf_event_group_hanle_t group, uint32_t bits)
{
    xEventGroupClearBits(group,  bits);
}

uint32_t rbf_event_wait_bits(rbf_event_group_hanle_t group, uint32_t bits, uint32_t timeoutMs)
{
    EventBits_t uxBits;
    const TickType_t xTicksToWait = timeoutMs / portTICK_PERIOD_MS;

    uxBits = xEventGroupWaitBits(
               group,   /* The event group being tested. */
               bits,            /* The bits within the event group to wait for. */
               pdTRUE,         
               pdFALSE,         /* Don't wait for both bits, either bit will do. */
               xTicksToWait ); /* Wait a maximum of 100ms for either bit to be set. */

    return uxBits&0xFFFFFFFF;
}