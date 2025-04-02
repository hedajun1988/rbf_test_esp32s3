/**
 * @file rbf_subdev_ota.h
 * @author Jio (hedajun@hzdusun.com)
 * @brief 
 * @version 0.1
 * @date 2025-04-01
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef RBF_SUBDEV_OTA_H
#define RBF_SUBDEV_OTA_H

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
    RBF_SUBDEV_OTA_EVT_UPGRADE_COMPLETE = 0,                   /**< Upgrade Successfully */
    RBF_SUBDEV_OTA_EVT_UPGRADE_FAIL,                           /**< Upgrade Failed */
    RBF_SUBDEV_OTA_EVT_UPGRADE_REQUEST_TIMEOUT,                /**< Upgrade request timeout */
}RBF_subdev_ota_evt_t;


typedef enum 
{
    RBF_SUBDEV_OTA_ERR_CODE_UNKNOWN = 0,                     /**< Upgrade Fail error code: unknow */
    RBF_SUBDEV_OTA_ERR_CODE_SUBDEV_NOT_SUPPORTED,            /**< Upgrade Fail error code: subdev is not supported */
}RBF_subdev_ota_err_code_t;

/**
 * @brief Subdev OTA upgrade status
 * 
 */
typedef struct 
{
    bool upgrading;  /**< Is upgrading */
    uint8_t percent; /**< Upgrade Progress: 0-100 */
}RBF_subdev_ota_status_t;

typedef struct 
{
    uint8_t devno;
    RBF_subdev_ota_err_code_t errorcode;
}RBF_subdev_ota_faild_response_t;

typedef struct 
{
    /**
     * @brief Subdev OTA service request to upgrade firmware data
     * @param offset Firmware offset address
     * @param size Request data length
     * @param data Request returned data
     * @return int Whether the firmware data is successfully obtained： 0-sucess Other values-failed
     */
    int (*rbf_subdev_ota_request_upgrade_data_handle)(unsigned int offset, unsigned int size, unsigned char* data); 

    /**
     * @brief Subdev OTA event reporting
     * @param evt Subdev OTA event
     * @param responses "Responses" is a set containing responses for multiple sub-devices that failed to 
     * upgrade: Sub-device upgrades allow for simultaneous upgrades of multiple devices, which may result i
     * n failures for one or more devices among them.
     * @param response_count The number of "Responses"
     * @return int  Results of processing event： 0-sucess Other values-failed
     */
    int (*rbf_subdev_ota_evt_handle)(RBF_subdev_ota_evt_t evt, RBF_subdev_ota_faild_response_t* responses, uint8_t response_count);
}RBF_subdev_ota_evt_callbacks_t;


/**
 * @brief Register Subdev OTA callback function cluster
 * 
 * @param cbs  Subdev OTA callback function cluster
 * @return int 0-sucess -1-failed
 */
int rbf_subdev_ota_register_evt_callback(RBF_subdev_ota_evt_callbacks_t* cbs);

/**
 * @brief Start Subdev OTA update
 * 
 * @param cat The subdev category
 * @param no_list The list of subdev 
 * @param count The count of no_list, RBF supports upgrading up to 12 sub-devices simultaneously.
 * @param fw_size Firmware size
 * @return int 
 */
int rbf_subdev_ota_start(uint8_t cat, uint8_t* no_list, uint8_t count, uint32_t fw_size);


#ifdef __cplusplus
}
#endif

#endif