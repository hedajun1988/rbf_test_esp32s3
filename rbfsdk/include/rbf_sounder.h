/**
 * @file rbf_sounder.h
 * @author Jio (hedajun@hzdusun.com)
 * @brief Outdoor Siren
 * @version 0.1
 * @date 2024-12-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef RBF_SOUNDER_H
#define RBF_SOUNDER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @brief Structure to represent a sounder device heartbeat event.
 */
typedef struct 
{
    uint8_t power;  /**<  Outdoor siren battery level 0-100 */
}rbf_sounder_heartbeat_t;


/**
 * @brief 门磁输入状态信息
 * 
 */
typedef struct 
{
    uint8_t power_supply_status; /**< External power supply: 0 - External power supply disconnected, 1 - External power supply connected */
    uint8_t solar_panels_fault; /**< Solar panel fault status: 0 - No fault, 1 - Fault */
    uint8_t battery_fault; /**< Battery status: 0 - Battery connected, 1 - Battery not connected */
    uint8_t charge_status;  /**<  Solar charging status: 0 - Not charging, 1 - Charging */
    uint8_t tamper;  /**< Outdoor siren tamper status: 0 - Tamper normal, 1 - Tamper alarm */
}rbf_sounder_input_status_t;

typedef enum
{
    RBF_SOUNDER_CTRL_DISABLED = 0,
    RBF_SOUNDER_LIGHT_ALARM_ONCE,
    RBF_SOUNDER_SOUND_ALARM_ONCE,
    RBF_SOUNDER_MIXED_ALARM_ONCE,
    RBF_SOUNDER_LIGHT_ALARM_TWICE,
    RBF_SOUNDER_SOUND_ALARM_TWICE,
    RBF_SOUNDER_MIXED_ALARM_TWICE,
} RBF_sounder_ctrl_action_t;


typedef enum
{
    RBF_SOUNDER_MODE_DISABLED = 0,
    RBF_SOUNDER_MODE_INVASION_LIGHT_ALARM,
    RBF_SOUNDER_MODE_INVASION_SOUND_ALARM,
    RBF_SOUNDER_MODE_INVASION_MIXED_ALARM,
    RBF_SOUNDER_MODE_FIRE_LIGHT_ALARM,
    RBF_SOUNDER_MODE_FIRE_SOUND_ALARM,
    RBF_SOUNDER_MODE_FIRE_MIXED_ALARM,
    RBF_SOUNDER_MODE_CONTINUOUS_EMERGENCY_LIGHT_ALARM,
    RBF_SOUNDER_MODE_CONTINUOUS_EMERGENCY_SOUND_ALARM,
    RBF_SOUNDER_MODE_CONTINUOUS_EMERGENCY_MIXED_ALARM,
    RBF_SOUNDER_MODE_CONTINUOUS_MEDICAL_LIGHT_ALARM,
    RBF_SOUNDER_MODE_CONTINUOUS_MEDICAL_SOUND_ALARM,
    RBF_SOUNDER_MODE_CONTINUOUS_MEDICAL_MIXED_ALARM,
} RBF_sounder_ctrl_mode_t;


/**
 * @brief Sounder volume level
 * 
 */
typedef enum 
{
    RBF_SOUNDER_VOLUME_LOW = 0,
    RBF_SOUNDER_VOLUME_MID,
    RBF_SOUNDER_VOLUME_HIGH,
} RBF_sounder_volume_t;

/**
 * @brief Sounder control parameters
 * Sounder mode represents a persistent state, while sounder action represents an action.
 *  You can use the sounder mode parameter alone to put the sounder into a certain alarm mode, 
 * while setting the sounder action to RBF_SOUNDER_CTRL_DISABLED. Alternatively, 
 * you can use the sounder action alone to instruct the sounder to perform a specific action, 
 * while setting the sounder mode to RBF_SOUNDER_MODE_DISABLED. When you set both the sounder 
 * mode and sounder action, the sounder will first execute the action and then enter the sounder mode.
 */
typedef struct 
{
    RBF_sounder_ctrl_action_t action; /**< sounder control action */
    RBF_sounder_ctrl_mode_t mode; /**< sounder control mode */
} RBF_sounder_param_t;


/**
 * @brief Callback function to handle sounder device heartbeat events.
 * 
 * This function is called whenever a sounder device sends a heartbeat event.
 */
typedef int (*rbf_sounder_heartbeat_callback_t)(uint8_t no, rbf_sounder_heartbeat_t* heartbeat);  


/**
 * @brief Sounder input status information update callback
 * 
 */
typedef int (*rbf_sounder_input_status_update_callback_t)(uint8_t no, rbf_sounder_input_status_t* input_status);  



/**
 * @brief  Sounder callback functions cluster
 * 
 */
typedef struct 
{
    rbf_sounder_heartbeat_callback_t hb_cb;   /**< Heartbeat callback function */
    rbf_sounder_input_status_update_callback_t input_status_cb;  /**< Input status update callback function */
}rbf_sounder_callbacks_t;



/**
 * @brief Register sounder callback functions cluster
 * 
 * @param cbs  Sounder callback functions cluster
 * @return int 0-Registration successful, 1-Registration failed
 */
int rbf_sounder_register_callbacks(rbf_sounder_callbacks_t* cbs);

/**
 * @brief Batch control sounder through synchronous broadcast
 * 
 * @param no_list sounder number list
 * @param count Number of sounders
 * @param sounder_param Sounder control parameters
 * @return int 0： Successfully set sounder broadcast, -1: Failed to set sounder broadcast
 * @par Example:
 * @code
 * uint8_t no_list[3];
 * RBF_sounder_param_t param;
 * 
 * sounder_list[0] = 1;
 * sounder_list[1] = 2;
 * sounder_list[2] = 3;
 * param.action = RBF_SOUNDER_CTRL_DISABLED;
 * param.mode = RBF_SOUNDER_MODE_FIRE_MIXED_ALARM;
 * rbf_sounder_boardcast_control(sounder_no_list, &param);
 * @endcode
 */
int rbf_sounder_boardcast_control(uint8_t* no_list, unsigned char count, RBF_sounder_param_t* sounder_param);


/**
 * @brief Set Sounder volume
 * 
 * @param no Sounder number
 * @param volume Sounder volume
 * @return int 0： Successfully, -1: Failed 
 */
int rbf_sounder_volume_set(uint8_t no, RBF_sounder_volume_t volume);

#ifdef __cplusplus
}
#endif


#endif
