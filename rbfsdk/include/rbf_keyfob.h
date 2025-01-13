/**
 * @file rbf_keyfob.h
 * @author Jio (hedajun@hzdusun.com)
 * @brief Remote Control
 * @version 0.1
 * @date 2024-12-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef RBF_KEYFOB_H
#define RBF_KEYFOB_H

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
    uint8_t power;  /**< Remote control battery level 0-100 */
}rbf_keyfob_heartbeat_t;


/**
 * @brief Remote control heartbeat callback handler
 * @param no Device registration number
 * @param heartbeat Heartbeat information
 */
typedef int (*rbf_keyfob_heartbeat_callback_t)(uint8_t no, rbf_keyfob_heartbeat_t* heartbeat);  


/**
 * @brief Remote control key press callback handler
 * @param no   Registration number of the remote control sub-device 
 * @param key  Key value of the remote control
 */
typedef int (*rbf_keyfob_key_press_callback_t)(uint8_t no, uint8_t key);  


/**
 * @brief Remote control callback functions cluster
 * 
 */
typedef struct 
{
    rbf_keyfob_heartbeat_callback_t hb_cb; /**<  Heartbeat callback function  */
    rbf_keyfob_key_press_callback_t key_press_cb;  /**<  Key press callback function */
}rbf_keyfob_callbacks_t;




/**
 * @brief Register remote control callback functions cluster
 * 
 * @param cbs Remote control callback functions cluster
 * @return int 0: Registration successful, -1: Registration failed
 */
int rbf_keyfob_register_callbacks(rbf_keyfob_callbacks_t* cbs);


#ifdef __cplusplus
}
#endif

#endif