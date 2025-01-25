/**
 * @file rbf_smartplug.h
 * @author Jio (hedajun@hzdusun.com)
 * @brief Smartplug implementation
 * @version 0.1
 * @date 2025-01-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef RBF_SMARTPLUG_H
#define RBF_SMARTPLUG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct 
{
    uint8_t onoff;  /**< Smartplug output status:  0-off, 1-on */
    uint8_t over_volt; /**< Smartplug Over-voltage: 0-nomal, 1-overvoltage */
    uint8_t over_curr; /**< Smartplug Over-current: 0-nomal, 1-overcurrent */
    uint8_t under_volt; /**< Smartplug undervoltage: 0-nomal, 1-undervoltage */
    uint8_t over_load; /**< Smartplug over load: 0-nomal, 1-overload */
    uint8_t lock; /**< Smartplug child lock: 0-nomal, 1-lock */
    uint32_t inst_volt;  /**< Smartplug  instantaneous voltage */
    uint32_t inst_curr;  /**< Smartplug  instantaneous current */
    uint32_t inst_power;  /**< Smartplug  instantaneous power */
    uint32_t run_time;  /**< Smartplug  run time in seconds */
    uint32_t cumu_power;  /**< Smartplug  Cumulative power (0.1Kwh) */
    uint8_t rssi; /**< Smartplug RSSI */ 
}rbf_smartplug_heartbeat_t;



/**
 * @brief Smartplug output status
 * 
 */
typedef struct 
{
    uint8_t onoff;  /**< Smartplug output status:  0-off, 1-on */
    uint8_t over_volt; /**< Smartplug Over-voltage: 0-nomal, 1-overvoltage */
    uint8_t over_curr; /**< Smartplug Over-current: 0-nomal, 1-overcurrent */
    uint8_t under_volt; /**< Smartplug undervoltage: 0-nomal, 1-undervoltage */
    uint8_t over_load; /**< Smartplug over load: 0-nomal, 1-overload */
    uint8_t lock; /**< Smartplug child lock: 0-nomal, 1-lock */
}rbf_smartplug_output_status_t;



/**
 * @brief Smartplug heartbeat callback handler
 * @param no Device registration number
 * @param heartbeat Heartbeat information
 */
typedef int (*rbf_smartplug_heartbeat_callback_t)(uint8_t no, rbf_smartplug_heartbeat_t* heartbeat);  

/**
 * @brief Callback for relay output status changed 
 * 
 */
typedef int (*rbf_smartplug_output_status_callback_t)(uint8_t no, rbf_smartplug_output_status_t* status);  


/**
 * @brief  relay callback functions cluster
 * 
 */
typedef struct 
{
    rbf_smartplug_heartbeat_callback_t hb_cb;
    rbf_smartplug_output_status_callback_t output_status_cb;  
}rbf_smartplug_callbacks_t;


typedef enum 
{
    RBF_SMARTPLUG_ACTION_NONE = 0,
    RBF_SMARTPLUG_ACTION_ON,
    RBF_SMARTPLUG_ACTION_OFF,
    RBF_SMARTPLUG_ACTION_TOOGLE,
}rbf_smartplug_action_t;


typedef struct 
{
    rbf_smartplug_action_t action;
    uint8_t lock;               /**< Smartplug child lock: 0-nomal, 1-lock (disable local control)*/
}rbf_smartplug_ctrl_t;

/**
 * @brief Register relay callback functions cluster
 * 
 * @param cbs relay callback functions cluster
 * @return int 0 - Registration successful, 1 - Registration failed
 */
int rbf_smartplug_register_callbacks(rbf_smartplug_callbacks_t* cbs);



/**
 * @brief Smartplug control
 * 
 * @param no Smartplug no
 * @param ctrl Smartplug control action
 * @return int int 0 -sucessful 1 - failed
 */
int rbf_smartplug_ctrl(uint8_t no, rbf_smartplug_ctrl_t* ctrl);


#ifdef __cplusplus
}
#endif

#endif