/**
 * @file rbf_wall_switch.h
 * @author Jio (hedajun@hzdusun.com)
 * @brief Strong relay
 * @version 0.1
 * @date 2025-01-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef RBF_WALL_SWITCH_H
#define RBF_WALL_SWITCH_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif



typedef struct 
{
    uint8_t onoff;  /**< Wall switch output status:  0-off, 1-on */
    uint8_t over_volt; /**< Wall switch Over-voltage: 0-nomal, 1-overvoltage */
    uint8_t over_curr; /**< Wall switch Over-current: 0-nomal, 1-overcurrent */
    uint8_t under_volt;  /**< Wall switch undervoltage: 0-nomal, 1-undervoltage */
    uint8_t over_load; /**<  Wall switch over load: 0-nomal, 1-overload */
    uint32_t inst_volt;  /**< Wall switch  instantaneous voltage */
    uint32_t inst_curr;  /**< Wall switch  instantaneous current */
    uint32_t inst_power;  /**< Wall switch  instantaneous power */
    int32_t rssi; /**< Wall switch RSSI */
}rbf_wall_switch_heartbeat_t;


/**
 * @brief Wall switch output status
 * 
 */
typedef struct 
{
    uint8_t onoff;  /**< Wall switch output status:  0-off, 1-on */
    uint8_t over_volt; /**< Wall switch Over-voltage: 0-nomal, 1-overvoltage */
    uint8_t over_curr; /**< Wall switch Over-current: 0-nomal, 1-overcurrent */
    uint8_t under_volt;  /**< Wall switch undervoltage: 0-nomal, 1-undervoltage */
    uint8_t over_load; /**<  Wall switch over load: 0-nomal, 1-overload */
}rbf_wall_switch_output_status_t;

typedef enum 
{
    RBF_WALL_SWITCH_ACTION_NONE = 0,
    RBF_WALL_SWITCH_ACTION_ON,
    RBF_WALL_SWITCH_ACTION_OFF,
    RBF_WALL_SWITCH_ACTION_TOOGLE,
}rbf_wall_switch_action_t;


typedef struct 
{
    rbf_wall_switch_action_t action;
}rbf_wall_switch_ctrl_t;

/**
 * @brief Smartplug heartbeat callback handler
 * @param no Device registration number
 * @param heartbeat Heartbeat information
 */
typedef int (*rbf_wall_switch_heartbeat_callback_t)(uint8_t no, rbf_wall_switch_heartbeat_t* heartbeat);  

/**
 * @brief Callback for wall switch output status changed 
 * 
 */
typedef int (*rbf_wall_switch_output_status_callback_t)(uint8_t no, rbf_wall_switch_output_status_t* status);  

/**
 * @brief  Wall switch callback functions cluster
 * 
 */
typedef struct 
{
    rbf_wall_switch_heartbeat_callback_t hb_cb;
    rbf_wall_switch_output_status_callback_t output_status_cb;  
}rbf_wall_switch_callbacks_t;


/**
 * @brief Register wall switch callback functions cluster
 * 
 * @param cbs wall switch callback functions cluster
 * @return int 0 - Registration successful, 1 - Registration failed
 */
int rbf_wall_switch_register_callbacks(rbf_wall_switch_callbacks_t* cbs);

/**
 * @brief Wall switch control
 * 
 * @param no Wall switch no
 * @param ctrl Wall switch control action
 * @return int int 0 -sucessful 1 - failed
 */
int rbf_wall_switch_ctrl(uint8_t no, rbf_wall_switch_ctrl_t* ctrl);

#ifdef __cplusplus
}
#endif

#endif