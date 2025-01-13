/**
 * @file rbf_magnetic.h
 * @author Jio (hedajun@hzdusun.com)
 * @brief Door Magnetic Sensor
 * @version 0.1
 * @date 2024-12-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef RBF_MAGNETIC_H
#define RBF_MAGNETIC_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif



/**
 * @brief Structure to represent a magnetic device heartbeat event.
 */
typedef struct 
{
    uint8_t power;  /**< Magnetic sensor battery level 0-100 */
}rbf_magnetic_heartbeat_t;


/**
 * @brief Magnetic sensor input information
 * 
 */
typedef struct 
{
    uint8_t alarm;  /**< Magnetic sensor alarm status: 0 - Magnetic sensor closed, 1 - Magnetic sensor open */
    uint8_t tamper;  /**< Magnetic sensor tamper status: 0 - Magnetic sensor tamper normal, 1 - Magnetic sensor tamper alarm  */
}rbf_magnetic_input_status_t;


/**
 * @brief Callback function to handle magnetic device heartbeat events.
 * 
 * This function is called whenever a magnetic device sends a heartbeat event.
 */
typedef int (*rbf_magnetic_heartbeat_callback_t)(uint8_t no, rbf_magnetic_heartbeat_t* heartbeat);  


/**
 * @brief Callback for updating magnetic input status information
 * 
 */
typedef int (*rbf_magnetic_input_status_update_callback_t)(uint8_t no, rbf_magnetic_input_status_t* input_status);  




/**
 * @brief  Magnetic sensor callback functions cluster
 * 
 */
typedef struct 
{
    rbf_magnetic_heartbeat_callback_t hb_cb;
    rbf_magnetic_input_status_update_callback_t input_status_cb;  
}rbf_magnetic_callbacks_t;



/**
 * @brief Register magnetic sensor callback functions cluster
 * 
 * @param cbs Magnetic sensor callback functions cluster
 * @return int 0 - Registration successful, 1 - Registration failed
 */
int rbf_magnetic_register_callbacks(rbf_magnetic_callbacks_t* cbs);




#ifdef __cplusplus
}
#endif

#endif