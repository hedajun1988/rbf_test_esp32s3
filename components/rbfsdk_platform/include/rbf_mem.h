/**
 * @file rbf_mem.h
 * @author Jio (hedajun@hzdusun.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef RBF_MEM_H
#define RBF_MEM_H

#include <stdint.h>
#include <stdio.h>

void* rbf_malloc(size_t size);
void rbf_free(void *ptr);


#endif