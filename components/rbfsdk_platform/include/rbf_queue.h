/**
 * @file rbf_queue.h
 * @author Jio (hedajun@hzdusun.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef RBF_QUEUE_H
#define RBF_QUEUE_H

#include <stdio.h>


typedef void* rbf_queue_t;

rbf_queue_t rbf_queue_create(int count, size_t size);
int rbf_queue_send(rbf_queue_t queue, void* data, int timeoutMs);
int rbf_queue_receive(rbf_queue_t queue, void* data, int timeoutMs);

#endif