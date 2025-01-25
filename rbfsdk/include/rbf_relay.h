/**
 * @file rbf_relay.h
 * @author Jio (hedajun@hzdusun.com)
 * @brief Weak relay
 * @version 0.1
 * @date 2025-01-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef RBF_RELAY_H
#define RBF_RELAY_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct 
{
    uint8_t onoff;  /**< Relay output status:  0-off, 1-on */
    uint8_t rssi;  /**< Relay RSSI */
}rbf_relay_heartbeat_t;

/**
 * @brief Relay output status
 * 
 */
typedef struct 
{
    uint8_t onoff;  /**< Relay output status:  0-off, 1-on */
}rbf_relay_output_status_t;

/**
 * @brief Relay heartbeat callback handler
 * @param no Device registration number
 * @param heartbeat Heartbeat information
 */
typedef int (*rbf_relay_heartbeat_callback_t)(uint8_t no, rbf_relay_heartbeat_t* heartbeat);  


/**
 * @brief Callback for relay output status changed 
 * 
 */
typedef int (*rbf_relay_output_status_callback_t)(uint8_t no, rbf_relay_output_status_t* status);  


/**
 * @brief  relay callback functions cluster
 * 
 */
typedef struct 
{
    rbf_relay_heartbeat_callback_t hb_cb;
    rbf_relay_output_status_callback_t output_status_cb;  
}rbf_relay_callbacks_t;


typedef enum 
{
    RBF_RELAY_ACTION_NONE = 0,
    RBF_RELAY_ACTION_ON,
    RBF_RELAY_ACTION_OFF,
    RBF_RELAY_ACTION_TOOGLE,
}rbf_relay_action_t;


typedef struct 
{
    rbf_relay_action_t action;
}rbf_relay_ctrl_t;

/**
 * @brief Register relay callback functions cluster
 * 
 * @param cbs relay callback functions cluster
 * @return int 0 - Registration successful, 1 - Registration failed
 */
int rbf_relay_register_callbacks(rbf_relay_callbacks_t* cbs);



/**
 * @brief Relay control
 * 
 * @param no Relay no
 * @param ctrl Relay control action
 * @return int int 0 -sucessful 1 - failed
 */
int rbf_relay_ctrl(uint8_t no, rbf_relay_ctrl_t* ctrl);


#ifdef __cplusplus
}
#endif

#endif