/**
 * @file rbf_dbg_log.c
 * @author Jio (hedajun@hzdusun.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

char *dbg_log_get_time_str()
{
    time_t stTimeval;
    struct timeval stTimevalus;
    struct tm* stCal;
    static char acBuf[64];

    memset(&stTimeval, 0, sizeof(stTimeval));
    memset(acBuf, 0, sizeof(acBuf));
    
    time(&stTimeval);
    stCal = localtime(&stTimeval);

    gettimeofday(&stTimevalus, NULL);

    snprintf(acBuf, sizeof(acBuf),
         "%04d-%02d-%02dT%02d:%02d:%02d.%03u:",
          stCal->tm_year + 1900,
          stCal->tm_mon + 1,
          stCal->tm_mday,
          stCal->tm_hour,
          stCal->tm_min,
          stCal->tm_sec,
          (unsigned)(stTimevalus.tv_usec / 1000));
          
    return acBuf;
}