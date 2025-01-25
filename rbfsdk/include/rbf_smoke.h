/**
 * @file rbf_smoke.h
 * @author Jio (hedajun@hzdusun.com)
 * @brief Smoke sensor
 * @version 0.1
 * @date 2024-12-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef RBF_SMOKE_H
#define RBF_SMOKE_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Structure to represent a smoke device heartbeat event.
 */
typedef struct 
{
    uint8_t power;  /**< Smoke sensor battery level 0-100 */
    uint8_t rssi;  /**< Smoke sensor RSSI */
}rbf_smoke_heartbeat_t;

/**
 * @brief Smoke sensor input information
 * 
 */
typedef struct 
{
    uint8_t alarm;  /**< Smoke sensor alarm status: 0 - Smoke sensor disalarm , 1 - Smoke sensor alarm */
}rbf_smoke_input_status_t;


/**
 * @brief Callback function to handle smoke device heartbeat events.
 * 
 * This function is called whenever a smoke device sends a heartbeat event.
 */
typedef int (*rbf_smoke_heartbeat_callback_t)(uint8_t no, rbf_smoke_heartbeat_t* heartbeat);  


/**
 * @brief Callback for updating smoke input status information
 * 
 */
typedef int (*rbf_smoke_input_status_update_callback_t)(uint8_t no, rbf_smoke_input_status_t* input_status);  


/**
 * @brief  Smoke sensor callback functions cluster
 * 
 */
typedef struct 
{
    rbf_smoke_heartbeat_callback_t hb_cb;
    rbf_smoke_input_status_update_callback_t input_status_cb;
}rbf_smoke_callbacks_t;


int rbf_smoke_register_callbacks(rbf_smoke_callbacks_t* cbs);

#ifdef __cplusplus
}
#endif

#endif