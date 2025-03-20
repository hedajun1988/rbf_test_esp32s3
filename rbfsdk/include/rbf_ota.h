/**
 * @file rbf_ota.h
 * @author Jio (hedajun@hzdusun.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef RBF_OTA_H
#define RBF_OTA_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif 


/**
 * @brief OTA upgrade event
 * 
 */
typedef enum
{
    RBF_OTA_EVT_ENTER_BOOTLOADER_SUCESS = 0,        /**< Entering bootloader successfully */
    RBF_OTA_EVT_ENTER_BOOTLOADER_FAIL,              /**< Failed to enter bootloader*/
    RBF_OTA_EVT_START_SUCCESS,                      /**< Start Upgrading */
    RBF_OTA_EVT_START_FAIL,                         /**< Failed to start the upgrade */
    RBF_OTA_EVT_UPGRADE_PROGRESS_UPDATE,            /**< Upgrade progress update */
    RBF_OTA_EVT_UPGRADE_COMPLETE,                   /**< Upgrade Successfully */
    RBF_OTA_EVT_UPGRADE_FAIL,                       /**< Upgrade Failed */
}RBF_ota_evt_t;


/**
 * @brief OTA upgrade status
 * 
 */
typedef struct 
{
    bool upgrading;  /**< Is upgrading */
    uint8_t percent; /**< Upgrade Progress: 0-100 */
}RBF_ota_status_t;


typedef struct 
{
    /**
     * @brief OTA service request to upgrade firmware data
     * @param offset Firmware offset address
     * @param size Request data length
     * @param data Request returned data
     * @return int Whether the firmware data is successfully obtained： 0-sucess Other values-failed
     */
    int (*rbf_ota_request_upgrade_data_handle)(unsigned int offset, unsigned int size, unsigned char* data); 

    /**
     * @brief OTA event reporting
     * @param evt OTA event
     * @param status OTA status information
     * @return int  Results of processing event： 0-sucess Other values-failed
     */
    int (*rbf_ota_evt_handle)(RBF_ota_evt_t evt, RBF_ota_status_t* status);
}RBF_ota_evt_callbacks_t;


/**
 * @brief Register OTA callback function cluster
 * 
 * @param cbs  OTA callback function cluster
 * @return int 0-sucess -1-failed
 */
int rbf_ota_register_evt_callback(RBF_ota_evt_callbacks_t* cbs);

/**
 * @brief Get OTA status information
 * 
 * @param status OTA status information
 * @return int  0-sucess -1-failed
 */
int rbf_ota_status_get(RBF_ota_status_t* status);

/**
 * @brief Start OTA update
 * 
 * @param fw_size Firmware size
 * @return int 0-sucess -1-failed
 */
int rbf_ota_start(uint32_t fw_size);



#ifdef __cplusplus
}
#endif


#endif 
