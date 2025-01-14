/**
 * @file rbf_mem.c
 * @author Jio (hedajun@hzdusun.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "rbf_mem.h"
void* rbf_malloc(size_t size)
{
    return malloc(size);
}

void rbf_free(void *ptr)
{
    free(ptr);
}