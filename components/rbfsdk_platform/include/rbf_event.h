/**
 * @file rbf_event.h
 * @author Jio (hedajun@hzdusun.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef RBF_EVENT_H
#define RBF_EVENT_H

#include <stdint.h>

typedef void* rbf_event_group_hanle_t;

rbf_event_group_hanle_t rbf_event_group_create();

void rbf_event_group_set_bits(rbf_event_group_hanle_t group, uint32_t bits);
void rbf_event_group_clear_bits(rbf_event_group_hanle_t group, uint32_t bits);
uint32_t rbf_event_wait_bits(rbf_event_group_hanle_t group, uint32_t bits, uint32_t timeoutMs);

#endif