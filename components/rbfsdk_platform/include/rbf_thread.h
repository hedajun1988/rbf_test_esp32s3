/**
 * @file rbf_thread.h
 * @author Jio (hedajun@hzdusun.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef RBF_THREAD_H
#define RBF_THREAD_H 

#include <stdint.h>
#include <stdio.h>


#define THREAD_STACK_MIN_VAL                       16384

#define THREAD_SLEEP_500MS                         500
#define THREAD_SLEEP_100MS                         100


int rbf_thread_create(const char* name, size_t iThreadStackSize, 
                                void *pThreadFunc, 
                                void *pTreadArg);
void rbf_thread_sleep(uint32_t timeoutMs);

#endif