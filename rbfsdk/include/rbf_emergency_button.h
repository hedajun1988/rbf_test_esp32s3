/**
 * @file rbf_emergency_button.h
 * @author Jio (hedajun@hzdusun.com)
 * @brief Emergency Button
 * @version 0.1
 * @date 2024-12-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef RBF_EMERGENCY_BUTTON_H
#define RBF_EMERGENCY_BUTTON_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @brief Structure to represent a emergency button device heartbeat event.
 */
typedef struct 
{
    uint8_t power;  /**< Emergency Button level 0-100 */
}rbf_emergency_button_heartbeat_t;


typedef enum 
{
    RBF_EMERGENCY_BUTTON_INPUT_EVT_ALARM = 0,  /**< Emergency Button Alarm */
}rbf_emergency_button_input_evt_t;

/**
 * @brief Callback function to handle  Emergency Button device heartbeat events.
 * 
 * This function is called whenever a  Emergency Button device sends a heartbeat event.
 */
typedef int (*rbf_emergency_button_heartbeat_callback_t)(uint8_t no, rbf_emergency_button_heartbeat_t* heartbeat);  


/**
 * @brief Callback for Emergency Button input event 
 * 
 */
typedef int (*rbf_emergency_button_input_evt_callback_t)(uint8_t no, rbf_emergency_button_input_evt_t evt); 
/**
 * @brief  Emergency Button callback functions cluster
 * 
 */
typedef struct 
{
    rbf_emergency_button_heartbeat_callback_t hb_cb;
    rbf_emergency_button_input_evt_callback_t input_evt_cb;  
}rbf_emergency_button_callbacks_t;



/**
 * @brief Register Emergency Button callback functions cluster
 * 
 * @param cbs Emergency Button  callback functions cluster
 * @return int 0 - Registration successful, 1 - Registration failed
 */
int rbf_emergency_button_register_callbacks(rbf_emergency_button_callbacks_t* cbs);


#ifdef __cplusplus
}
#endif

#endif