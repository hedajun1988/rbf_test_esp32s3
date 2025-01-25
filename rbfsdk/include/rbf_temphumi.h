/**
 * @file rbf_temphumi.h
 * @author Jio (hedajun@hzdusun.com)
 * @brief Temperature and humidity sensor
 * @version 0.1
 * @date 2024-12-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef RBF_TEMP_HUMI_H
#define RBF_TEMP_HUMI_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Structure to represent a Temperature and humidity sensor device heartbeat event.
 */
typedef struct 
{
    uint8_t power;  /**< Temperature and humidity sensor level 0-100 */
    float temp;  /**< Temperature */
    float humi;  /**< Humidity */
    uint8_t rssi; /**< Sensor RSSI */
}rbf_temp_humi_heartbeat_t;

/**
 * @brief Temperature and humidity sensor input information
 * 
 */
typedef struct 
{
    uint8_t over_temp_alarm;  /**< Temperature and humidity sensor over temperature alarm status: 0 - Over temperature recovery, 1 - Over temperature detected */
    uint8_t low_temp_alarm;  /**< Temperature and humidity sensor low temperature alarm status: 0 - Low temperature recovery, 1 - Low temperature detected  */
    uint8_t over_humi_alarm;  /**< Temperature and humidity sensor over humidity alarm status: 0 - Over humidity recovery, 1 - Over humidity detected */
    uint8_t low_humi_alarm;  /**< Temperature and humidity sensor low humidity alarm status: 0 - Low humidity recovery, 1 - Low humidity detected */
}rbf_temp_humi_status_t;


typedef enum 
{
    RBF_TEMP_UINT_CELS = 0, /**< Celsius */
    RBF_TEMP_UINT_FAHR,     /**<  Fahrenheit */
}rbf_temp_humi_temp_uint_t;


typedef enum 
{
    RBF_TEMP_REPORT_0_5_CHANGE = 0,
    RBF_TEMP_REPORT_1_CHANGE,
    RBF_TEMP_REPORT_5_CHANGE,
    RBF_TEMP_REPORT_10_CHANGE,
}rbf_temp_humi_temp_report_threshold_t;

typedef enum 
{
    RBF_HUMI_REPORT_5_CHANGE = 0,
    RBF_HUMI_REPORT_10_CHANGE,
    RBF_HUMI_REPORT_15_CHANGE,
    RBF_HUMI_REPORT_20_CHANGE,
}rbf_temp_humi_humi_report_threshold_t;

typedef struct 
{
    rbf_temp_humi_temp_uint_t temp_units; /**< Temperature Units */
    rbf_temp_humi_temp_report_threshold_t temp_threshold;  /**< Temperature reporting change */
    rbf_temp_humi_humi_report_threshold_t humi_threshold;  /**< Humidity reporting change */
}rbf_temp_humi_config_t;

/**
 * @brief Callback function to handle temp humi device heartbeat events.
 * 
 * This function is called whenever a temp humi device sends a heartbeat event.
 */
typedef int (*rbf_temp_humi_heartbeat_callback_t)(uint8_t no, rbf_temp_humi_heartbeat_t* heartbeat);  


/**
 * @brief Callback for updating temp humi input status information
 * 
 */
typedef int (*rbf_temp_humi_input_status_update_callback_t)(uint8_t no, rbf_temp_humi_status_t* input_status);  



/**
 * @brief  Temperature and humidity sensor callback functions cluster
 * 
 */
typedef struct 
{
    rbf_temp_humi_heartbeat_callback_t hb_cb;
    rbf_temp_humi_input_status_update_callback_t input_status_cb;  
}rbf_temp_humi_callbacks_t;



/**
 * @brief Register  Temperature and humidity  sensor callback functions cluster
 * 
 * @param cbs  Temperature and humidity sensor callback functions cluster
 * @return int 0 - Registration successful, 1 - Registration failed
 */
int rbf_temp_humi_register_callbacks(rbf_temp_humi_callbacks_t* cbs);


/**
 * @brief Set Temperature and humidity  sensor configuration
 * 
 * @param no Temperature and humidity  sensor number
 * @param config Temperature and humidity  sensor configuration
 * @return int 0： Successfully, -1: Failed 
 */
int rbf_temp_humi_set(uint8_t no, rbf_temp_humi_config_t* config);

#ifdef __cplusplus
}
#endif

#endif