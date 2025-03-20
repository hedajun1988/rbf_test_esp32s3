/**
 * @file rbf_keypad.h
 * @author Jio (hedajun@hzdusun.com)
 * @brief keypad
 * @version 0.1
 * @date 2024-12-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef RBF_KEYPAD_H
#define RBF_KEYPAD_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif



/**
 * @brief Structure to represent a keypad device heartbeat event.
 */
typedef struct 
{
    uint8_t power;  /**< keypad battery level 0-100 */
    int32_t rssi;   /**< keypad RSSI */
}rbf_keypad_heartbeat_t;


/**
 * @brief Keypad alarm information
 * 
 */
typedef struct 
{
    uint8_t emergency_alarm;  /**< Keypad emergency alarm status: 0 - emergency alarm clear, 1 - emergency alarm */
    uint8_t fire_alarm;  /**< Keypad fire alarm status: 0 - fire alarm clear, 1 - fire alarm */
    uint8_t medical_alarm;  /**< Keypad medical alarm status: 0 - medical alarm clear, 1 - medical alarm */
    uint8_t tamper;  /**< Keypad tamper status: 0 - Keypad tamper normal, 1 - Keypad tamper alarm  */
}rbf_keypad_alarm_status_t;


typedef enum 
{
    RBF_KEYPAD_ALARM_TONE_NONE = 0,   /**< No alarm tone */
    RBF_KEYPAD_ALARM_TONE_INTRUSION,  /**< Invading alarm tone */
    RBF_KEYPAD_ALARM_TONE_FIRE,       /**< Fire alarm tone */
    RBF_KEYPAD_ALARM_TONE_EMERGENCY,  /**< Emergency alarm tone */
    RBF_KEYPAD_ALARM_TONE_MEDICAL,    /**< Medical alarm tone */
}rbf_keypad_alarm_tone_type_t;


typedef enum
{
    RBF_KEYPAD_BACKLIGHT_TIME_10S = 0, /**< Backlight 10 seconds */
    RBF_KEYPAD_BACKLIGHT_TIME_15S,     /**< Backlight 15 seconds */
    RBF_KEYPAD_BACKLIGHT_TIME_20S,     /**< Backlight 20 seconds */
    RBF_KEYPAD_BACKLIGHT_TIME_25S,     /**< Backlight 25 seconds */
}rbf_keypad_backlight_time_t;

typedef struct 
{
    rbf_keypad_alarm_tone_type_t alarm_tone_play; /**< Play alarm tone */
    uint8_t err_led_state;                        /**< Error led: 0-off when wakeup, 1-on when wakeup */
    uint8_t arm_led_state;                        /**< Arm led: 0-off when wakeup, 1-on when wakeup */
    uint8_t warn_led_state;                       /**< Warn led: 0-off when wakeup, 1-on when wakeup, 2-blink when wakup */
    uint8_t enable_key_tone;                      /**< Key tone when pressd: 0-disable, 1-enable */
    rbf_keypad_backlight_time_t backlight_time;   /**< backlight time */
    uint8_t lock_state;                           /**< keypad lock: 0-unlock, 1-lock */
}rbf_keypad_settings_t;


/**
 * @brief Keypad heartbeat callback handler
 * @param no Device registration number
 * @param heartbeat Heartbeat information
 */
typedef int (*rbf_keypad_heartbeat_callback_t)(uint8_t no, rbf_keypad_heartbeat_t* heartbeat);  


/**
 * @brief Keypad alarm callback handler
 * @param no   Registration number of the Keypad sub-device 
 * @param alarm_state  Keypad alarm state
 */
typedef int (*rbf_keypad_alarm_callback_t)(uint8_t no, rbf_keypad_alarm_status_t* alarm_status);  



/**
 * @brief Keypad key press callback handler
 * @param no   Registration number of the remote control sub-device 
 * @param input_keys  The Keypad key values ​​in the order they were pressed
 * @param input_count  Keypad input count
 */
typedef int (*rbf_keypad_key_input_callback_t)(uint8_t no, uint8_t input_keys[32], uint8_t input_count); 

/**
 * @brief Keypad callback functions cluster
 * 
 */
typedef struct 
{
    rbf_keypad_heartbeat_callback_t hb_cb; /**<  Heartbeat callback function  */
    rbf_keypad_alarm_callback_t alarm_cb;  /**<  Alarm callback function */
    rbf_keypad_key_input_callback_t key_input_cb;  /**<  Key press callback function */
}rbf_keypad_callbacks_t;


/**
 * @brief Register Keypad callback functions cluster
 * 
 * @param cbs Keypad callback functions cluster
 * @return int 0: Registration successful, -1: Registration failed
 */
int rbf_keypad_register_callbacks(rbf_keypad_callbacks_t* cbs);


/**
 * @brief Set the keyboard, and it will take effect the next time you wake up the keyboard
 * 
 * @param no Device registration number
 * @param settings Keypad settings
 */
int rbf_keypad_set(uint8_t no, rbf_keypad_settings_t* settings);

#ifdef __cplusplus
}
#endif



#endif