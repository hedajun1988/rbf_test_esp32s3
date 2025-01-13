/**
 * @file rbf_water_leak.h
 * @author Jio (hedajun@hzdusun.com)
 * @brief Water Leak Sensor
 * @version 0.1
 * @date 2024-12-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef RBF_WATER_LEAK_H
#define RBF_WATER_LEAK_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Structure to represent a water leak device heartbeat event.
 */
typedef struct 
{
    uint8_t power;  /**<  Water leak sensor battery level 0-100 */
}rbf_water_leak_heartbeat_t;


/**
 * @brief Water leak sensor input information
 * 
 */
typedef struct 
{
    uint8_t alarm;  /**< Water leak sensor alarm status: 0 - Leakage alarm cleared, 1 -Leakage alarm */
}rbf_water_leak_input_status_t;

/**
 * @brief Callback function to handle water_leak device heartbeat events.
 * 
 * This function is called whenever a water_leak device sends a heartbeat event.
 */
typedef int (*rbf_water_leak_heartbeat_callback_t)(uint8_t no, rbf_water_leak_heartbeat_t* heartbeat);  


/**
 * @brief Callback for updating water_leak input status information
 * 
 */
typedef int (*rbf_water_leak_input_status_update_callback_t)(uint8_t no, rbf_water_leak_input_status_t* input_status);  



/**
 * @brief  Magnetic sensor callback functions cluster
 * 
 */
typedef struct 
{
    rbf_water_leak_heartbeat_callback_t hb_cb;
    rbf_water_leak_input_status_update_callback_t input_status_cb;  
}rbf_water_leak_callbacks_t;



/**
 * @brief Register water_leak sensor callback functions cluster
 * 
 * @param cbs Magnetic sensor callback functions cluster
 * @return int 0 - Registration successful, 1 - Registration failed
 */
int rbf_water_leak_register_callbacks(rbf_water_leak_callbacks_t* cbs);

#ifdef __cplusplus
}
#endif

#endif