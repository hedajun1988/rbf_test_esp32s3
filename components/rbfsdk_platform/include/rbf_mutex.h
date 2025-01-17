/**
 * @file rbf_mutex.h
 * @author Jio (hedajun@hzdusun.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef RBF_MUTEX_H
#define RBF_MUTEX_H


typedef void* rbf_mutex_t;

rbf_mutex_t rbf_mutex_create();
void rbf_mutex_lock(rbf_mutex_t mutex);
void rbf_mutex_unlock(rbf_mutex_t mutex);


#endif