/**
 * @file rbf_indoor_siren.h
 * @author Jio (hedajun@hzdusun.com)
 * @brief Indoor siren
 * @version 0.1
 * @date 2025-01-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef RBF_INDOOR_SIREN_H
#define RBF_INDOOR_SIREN_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @brief Structure to represent a indoor siren device heartbeat event.
 */
typedef struct 
{
    uint8_t power;  /**<  Indoor siren battery level 0-100 */
    uint8_t rssi;   /**<  Indoor siren RSSI */
}rbf_indoor_siren_heartbeat_t;


/**
 * @brief indoor siren status information
 * 
 */
typedef struct 
{
    uint8_t battery_fault; /**< Battery status: 0 - Battery connected, 1 - Battery not connected */
}rbf_indoor_siren_input_status_t;

typedef enum
{
    RBF_INDOOR_SIREN_CTRL_DISABLED = 0,
    RBF_INDOOR_SIREN_LIGHT_ALARM_ONCE,
    RBF_INDOOR_SIREN_SOUND_ALARM_ONCE,
    RBF_INDOOR_SIREN_MIXED_ALARM_ONCE,
    RBF_INDOOR_SIREN_LIGHT_ALARM_TWICE,
    RBF_INDOOR_SIREN_SOUND_ALARM_TWICE,
    RBF_INDOOR_SIREN_MIXED_ALARM_TWICE,
} RBF_indoor_siren_ctrl_action_t;


typedef enum
{
    RBF_INDOOR_SIREN_MODE_DISABLED = 0,
    RBF_INDOOR_SIREN_MODE_INVASION_LIGHT_ALARM,
    RBF_INDOOR_SIREN_MODE_INVASION_SOUND_ALARM,
    RBF_INDOOR_SIREN_MODE_INVASION_MIXED_ALARM,
    RBF_INDOOR_SIREN_MODE_FIRE_LIGHT_ALARM,
    RBF_INDOOR_SIREN_MODE_FIRE_SOUND_ALARM,
    RBF_INDOOR_SIREN_MODE_FIRE_MIXED_ALARM,
    RBF_INDOOR_SIREN_MODE_CONTINUOUS_EMERGENCY_LIGHT_ALARM,
    RBF_INDOOR_SIREN_MODE_CONTINUOUS_EMERGENCY_SOUND_ALARM,
    RBF_INDOOR_SIREN_MODE_CONTINUOUS_EMERGENCY_MIXED_ALARM,
    RBF_INDOOR_SIREN_MODE_CONTINUOUS_MEDICAL_LIGHT_ALARM,
    RBF_INDOOR_SIREN_MODE_CONTINUOUS_MEDICAL_SOUND_ALARM,
    RBF_INDOOR_SIREN_MODE_CONTINUOUS_MEDICAL_MIXED_ALARM,
} RBF_indoor_siren_ctrl_mode_t;


/**
 * @brief Indoor siren volume level
 * 
 */
typedef enum 
{
    RBF_INDOOR_SIREN_VOLUME_LOW = 0,
    RBF_INDOOR_SIREN_VOLUME_MID,
    RBF_INDOOR_SIREN_VOLUME_HIGH,
} RBF_indoor_siren_volume_t;

/**
 * @brief Indoor siren control parameters
 * Indoor siren mode represents a persistent state, while indoor siren action represents an action.
 *  You can use the indoor siren mode parameter alone to put the indoor siren into a certain alarm mode, 
 * while setting the indoor siren action to RBF_INDOOR_SIREN_CTRL_DISABLED. Alternatively, 
 * you can use the indoor siren action alone to instruct the indoor siren to perform a specific action, 
 * while setting the indoor siren mode to RBF_INDOOR_SIREN_MODE_DISABLED. When you set both the indoor siren 
 * mode and indoor siren action, the indoor siren will first execute the action and then enter the indoor siren mode.
 */
typedef struct 
{
    RBF_indoor_siren_ctrl_action_t action; /**< indoor siren control action */
    RBF_indoor_siren_ctrl_mode_t mode; /**< indoor siren control mode */
} RBF_indoor_siren_param_t;


/**
 * @brief Callback function to handle indoor siren device heartbeat events.
 * 
 * This function is called whenever a indoor siren device sends a heartbeat event.
 */
typedef int (*rbf_indoor_siren_heartbeat_callback_t)(uint8_t no, rbf_indoor_siren_heartbeat_t* heartbeat);  


/**
 * @brief Indoor siren input status information update callback
 * 
 */
typedef int (*rbf_indoor_siren_input_status_update_callback_t)(uint8_t no, rbf_indoor_siren_input_status_t* input_status);  



/**
 * @brief  Indoor siren callback functions cluster
 * 
 */
typedef struct 
{
    rbf_indoor_siren_heartbeat_callback_t hb_cb;   /**< Heartbeat callback function */
    rbf_indoor_siren_input_status_update_callback_t input_status_cb;  /**< Input status update callback function */
}rbf_indoor_siren_callbacks_t;



/**
 * @brief Register indoor siren callback functions cluster
 * 
 * @param cbs  Indoor siren callback functions cluster
 * @return int 0-Registration successful, 1-Registration failed
 */
int rbf_indoor_siren_register_callbacks(rbf_indoor_siren_callbacks_t* cbs);

/**
 * @brief Batch control indoor siren through synchronous broadcast
 * 
 * @param no_list indoor siren number list
 * @param count Number of indoor sirens
 * @param indoor siren_param Indoor siren control parameters
 * @return int 0： Successfully set indoor siren broadcast, -1: Failed to set indoor siren broadcast
 * @par Example:
 * @code
 * uint8_t no_list[3];
 * RBF_indoor siren_param_t param;
 * 
 * indoor siren_list[0] = 1;
 * indoor siren_list[1] = 2;
 * indoor siren_list[2] = 3;
 * param.action = RBF_INDOOR_SIREN_CTRL_DISABLED;
 * param.mode = RBF_INDOOR_SIREN_MODE_FIRE_MIXED_ALARM;
 * rbf_indoor siren_boardcast_control(indoor siren_no_list, &param);
 * @endcode
 */
int rbf_indoor_siren_boardcast_control(uint8_t* no_list, unsigned char count, RBF_indoor_siren_param_t* indoor_siren_param);


/**
 * @brief Set Indoor siren volume
 * 
 * @param no Indoor siren number
 * @param volume Indoor siren volume
 * @return int 0： Successfully, -1: Failed 
 */
int rbf_indoor_siren_volume_set(uint8_t no, RBF_indoor_siren_volume_t volume);

#ifdef __cplusplus
}
#endif


#endif
