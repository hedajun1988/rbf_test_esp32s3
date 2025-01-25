/**
 * @file rbf_pir.h
 * @author Jio (hedajun@hzdusun.com)
 * @brief PIR sensor
 * @version 0.1
 * @date 2024-12-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef RBF_PIR_H
#define RBF_PIR_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @brief Structure to represent a pir device heartbeat event.
 */
typedef struct 
{
    uint8_t power;  /**< PIR sensor battery level 0-100 */
    uint8_t rssi;   /**< PIR sensor RSSI */
}rbf_pir_heartbeat_t;


typedef enum 
{
    RBF_PIR_INPUT_EVT_ALARM = 0,  /**< pir sensor detect motion */
}rbf_pir_input_evt_t;

/**
 * @brief PIR sensor input information
 * 
 */
typedef struct 
{
    uint8_t fault;  /**< PIR sensor fault: 0 - pir sensor fault,  1 - pir sensor normal */
    uint8_t tamper;  /**< PIR sensor tamper status: 0 - pir sensor tamper normal, 1 - pir sensor tamper alarm  */
}rbf_pir_input_status_t;



typedef enum 
{
    RBF_PID_SENSITIVEITY_AUTO = 0,
    RBF_PID_SENSITIVEITY_LOW,
    RBF_PID_SENSITIVEITY_MID,
    RBF_PID_SENSITIVEITY_HIGH,
}rbf_pir_sensitive_t;

typedef struct 
{
    uint8_t tamper_enable; /**< PIR sensor tamper enable: 0 - disable,  1 - enable */
    rbf_pir_sensitive_t sensitivity;  /**< PIR sensor sensitivity */
}rbf_pir_config_t;


/**
 * @brief Callback function to handle PIR device heartbeat events.
 * 
 * This function is called whenever a PIR device sends a heartbeat event.
 */
typedef int (*rbf_pir_heartbeat_callback_t)(uint8_t no, rbf_pir_heartbeat_t* heartbeat);  


/**
 * @brief Callback for PIR input event 
 * 
 */
typedef int (*rbf_pir_input_evt_callback_t)(uint8_t no, rbf_pir_input_evt_t evt);  
/**
 * @brief Callback for updating PIR input status information
 * 
 */
typedef int (*rbf_pir_input_status_update_callback_t)(uint8_t no, rbf_pir_input_status_t* input_status);  


/**
 * @brief  pir sensor callback functions cluster
 * 
 */
typedef struct 
{
    rbf_pir_heartbeat_callback_t hb_cb;
    rbf_pir_input_evt_callback_t input_evt_cb;
    rbf_pir_input_status_update_callback_t input_status_cb;  
}rbf_pir_callbacks_t;



/**
 * @brief Register pir sensor callback functions cluster
 * 
 * @param cbs pir sensor callback functions cluster
 * @return int 0 - Registration successful, 1 - Registration failed
 */
int rbf_pir_register_callbacks(rbf_pir_callbacks_t* cbs);


/**
 * @brief Set pir configuration
 * 
 * @param no pir number
 * @param config pir configuration
 * @return int 0： Successfully, -1: Failed 
 */
int rbf_pir_set(uint8_t no, rbf_pir_config_t* config);


#ifdef __cplusplus
}
#endif

#endif