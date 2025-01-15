/**
 * @file rbf_api.h
 * @author Jio (hedajun@hzdusun.com)
 * @brief rbf stack API
 * @version 0.1
 * @date 2024-12-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef RBF_API_H
#define RBF_API_H

#ifdef __cplusplus
extern "C"
{
#endif 

#define RB_SDK_VERSION             0
#define RB_SDK_REVISION            2
#define RB_SDK_PATCH               0

#define RBF_DEVICE_MAC_LEN         (8)         /**< RBF sub-device MAC length */
#define RBF_DEVICE_SN_LEN          (16)        /**< RBF sub-device serial number length */


/**
 * @brief Device category
 * @note There can be multiple sub-device types under a category
 * 
 */
typedef enum
{
    RBF_DEV_IO           = 1,                       /**< RBF sub-device IO category */
    RBF_DEV_SOUNDER      = 2,                       /**< RBF sub-device sounder category  */
    RBF_DEV_KEYPAD       = 3,                       /**< RBF sub-device keypad category */
    RBF_DEV_KEYFOB       = 4,                       /**< RBF sub-device key fob category */
    RBF_DEV_UNKNOW
}RBF_dev_cat_t;


/**
 * @brief RBF sub-device types
 * 
 */
typedef enum
{
    RBF_DEV_TYPE_MC           = 0,     /**< Door sensor */
    RBF_DEV_TYPE_PIR          = 1,     /**< Infrared sensor */
    RBF_DEV_TYPE_WATERT_LEAK  = 2,     /**< Water leak sensor */
    RBF_DEV_TYPE_FIXED_PA     = 3,     /**< Fixed emergency button */
    RBF_DEV_TYPE_PORT_PA      = 4,     /**< Portable emergency button */
    RBF_DEV_TYPE_SMOKE        = 5,     /**< Smoke detector */
    RBF_DEV_TYPE_TEMP_HUMI    = 6,     /**< Temperature and humidity sensor */
    RBF_DEV_TYPE_SMART_PLUG   = 7,     /**< Smart plug */
    RBF_DEV_TYPE_RELAY        = 8,     /**< Weak relay */
    RBF_DEV_TYPE_WALL_SWITCH  = 9,     /**< Strong relay */
    RBF_DEV_TYPE_WATER_VALVE  = 10,     /**< Water valve */
    RBF_DEV_TYPE_IN_SOUND     = 11,     /**< Indoor sounder */
    RBF_DEV_TYPE_OUT_SOUND    = 12,     /**< Outdoor sounder */
    RBF_DEV_TYPE_LED_KEYPAD   = 13,     /**< LED keypad */
    RBF_DEV_TYPE_KEYFOB       = 14,     /**< Key fob */
    RBF_DEV_TYPE_UNKNOW,            /**< Unknown device */
} RBF_dev_type_t;


/**
 * @brief Sub-device ID addressed by device category and device registration number
 * 
 */
typedef struct 
{
    RBF_dev_cat_t cat;  /**< Device category */
    unsigned char no;   /**< Device registration number */
}RBF_dev_id_t;



/**
 * @brief 布撤防状态
 * 
 */
typedef enum 
{
    RBF_IO_ALARM_ENABLE = 0,   /**< IO arm enable */
    RBF_IO_ALARM_DISABLE,      /**< IO disarm */
} RBF_io_alarm_status_t;


typedef enum
{
    RBF_LED_INDICATE_BLUE_LIGHT_TWICE     = 0,   /**< Blue light flashes twice*/
    RBF_LED_INDICATE_GREEN_LIGHT_TWICE    = 1,   /**< Green light flashes twice*/
    RBF_LED_INDICATE_RED_LIGHT_TWICE      = 2,    /**< Red light flashes twice*/
    RBF_LED_INDICATE_ORANGE_LIGHT_TWICE   = 3,   /**< Orange light flashes twice*/
    RBF_LED_INDICATE_GREEN_LIGHT_THIRD    = 4    /**< Green light flashes three times*/
} RBF_led_indicate_mode_t;

typedef enum
{
    RBF_LED_INDICATE_DURATION_500MS      = 0, /**< Flash every 500ms*/
    RBF_LED_INDICATE_DURATION_1000MS     = 1, /**< Flash every 1000ms*/
    RBF_LED_INDICATE_DURATION_1500MS     = 2, /**< Flash every 1500ms*/
    RBF_LED_INDICATE_DURATION_2000MS     = 3, /**< Flash every 2000ms*/
    RBF_LED_INDICATE_DURATION_2500MS     = 4, /**< Flash every 2500ms*/
    RBF_LED_INDICATE_DURATION_3000MS     = 5, /**< Flash every 3000ms*/
} RBF_led_indicate_duration_t;

/**
 * @brief RBF sub-device universal LED indicator light status setting
 * 
 */
typedef struct 
{
    RBF_led_indicate_mode_t mode;          /**< Light flashing mode */
    RBF_led_indicate_duration_t duration;  /**< Light flashing interval  */
}RBF_led_indicate_t;

/**
 * @brief RBF frequency bands
 * 
 */
typedef enum
{
    RBF_FREQ_868 = 0,  /**< 868MHz frequency band */
    RBF_FREQ_915 = 1,  /**< 915MHz frequency band */
    RBF_FREQ_433 = 2,  /**< 433MHz frequency band */
} RBF_Freq_t;

/**
 * @brief 
 * The RBF module typically connects to the MUC via an 
 * UART port, so RBF_port usually refers to the serial read
 *  and write interface
 */
typedef struct{
    /**
     * @brief RBF Module read
     * @param buf Read buffer
     * @param bufsize Buffer size
     * @param timeout Block timeout in milliseconds
     * @return int  Number of bytes read 
     */
    int (*read)(unsigned char *buf, int bufsize, int timeout);
    /**
     * @brief RBF Module write
     * @param data  Data to be written to the RBF buffer
     * @param dataLen Length of data to be written to the RBF buffer
     */
    int (*write)(unsigned char *data, int dataLen);
}RBF_port_t;


/**
 * @brief RBF sub-device registration method
 * 
 */
typedef enum
{
    RBF_REGISTER_LOCAL = 0,   /**< Local registration */
    RBF_REGISTER_MAC,         /**< MAC address registration */
    RBF_REGISTER_SERIAL_NUMBER  /**< Serial number registration */
}RBF_register_type_t;


/**
 * @brief RBF sub-device registration parameters
 * 
 */
typedef struct 
{
    RBF_register_type_t type;   /**< Registration type  */
    /**
     * @brief register parameters
     * 
     */
    union {
        unsigned char mac[RBF_DEVICE_MAC_LEN];            /**< Sub-device MAC address, to be filled in when registration type is MAC address registration */
        unsigned char serialNumber[RBF_DEVICE_SN_LEN];    /**< Sub-device SN address, to be filled in when registration type is SN registration */
    }param;
}RBF_register_param_t;


/**
 * @brief RBF sub-device registration response
 * 
 */
typedef struct{
    RBF_dev_cat_t cat;         /**< Sub-device category */
    unsigned char no;          /**< Sub-device registration number */
    RBF_dev_type_t type;       /**< Sub-device sub-type*/
    unsigned char ver[3];     /**< Sub-device version number*/
    unsigned char sn[16];     /**< Sub-device serial number */
    unsigned char err;        /**< Error code for registration failure*/
    unsigned char mac[32];    /**< MAC address */
}RBF_register_response_t;


/**
 * @brief RBF hub module event report
 * 
 */
typedef struct 
{
    unsigned char  sync_flag;   /**< Option for synchronization request after hub startup, QRFH, 1: sync request; 0xFF invalid */
}BBF_hub_event_t;


/**
 * @brief HUB  event callback functions
 * 
 */
typedef struct 
{
    /**
     * @brief Callback for sub-device registration response
     * @param reponse response Sub-device registration response
     */
    int (*rbf_dev_register_reponse_handle)(RBF_register_response_t* reponse);  
    /**
     * @brief Callback for hub parameter synchronization request
     * The hub requests the user to synchronize hub parameters each time 
     * it starts up, and the user should synchronize hub parameters upon receiving this request
     * 
     */
    int (*rbf_hub_sync_handle)(void);     
    /**
     * @brief Callback for hub module event reporting
     * @param event event Hub module event
     */
    int (*rbf_hub_event_handle)(BBF_hub_event_t* event);     


    /**
     * @brief  Callback for sub-device registration information
     * @param ids  List of sub-device IDs
     * @param count Number of sub-devices
     * @note This callback is asynchronously triggered after sending a query request 
     * to the hub via rbf_get_register_info()
     */
    int (*rbf_dev_register_info_handle)(RBF_dev_id_t* ids, int count);   
}RBF_evt_callbacks_t;

/**
 * @brief Set the read and write interface for the RBF port used by rbfsdk
 * 
 * @param port The RBF interface includes read and write handles
 * @return int Return 0 on success, -1 on failure
 */
int rbf_set_port(RBF_port_t* port);


/**
 * @brief Register RBF event callbacks
 * 
 * @param cbs  Event callback functions cluster
 * @return int 0： Successfully enable Find Me mode， -1: Failed to enable Find Me mode
 */
int rbf_register_evt_callback(RBF_evt_callbacks_t* cbs);


/**
 * @brief Initialize the protocol stack
 * 
 * @return int Return 0 on success, -1 on failure
 */
int  rbf_init(void);

/**
 * @brief Delete initializ the protocol stack
 * 
 * @return int unsigned int Return 0 on success, -1 on failure
 */
int  rbf_delinit(void);


/**
 * @brief Enter registration mode
 * 
 * @param param Registration parameters, fill in mac/sn according to the registration type. 
 * If it is in local registration mode, mac and sn can be omitted
 * @return int 0: Successfully entered registration mode, -1: Failed to enter registration mode
 */
int  rbf_start_hub_register( RBF_register_param_t* param);



/**
 * @brief Exit registration mode
 * 
 * @return int 0: Successfully exited registration mode, -1: Failed to exit registration mode
 */
int  rbf_stop_hub_register(void);


/**
 * @brief Get registration information for all sub-devices in the RBF hub
 * 
 * @return int 0：Successful request for registration information, -1: Failed to request registration information
 * @note This interface is asynchronous; calling this interface will send a request for sub-device information
 */
int rbf_get_register_info();


/**
 * @brief Delete a sub-device
 * 
 * @param id Sub-device addressing address
 * @return int 0: Deletion successful, -1: Deletion failed
 * @note A successful response from the interface indicates that the command was successfully issued and in most
 *  cases can be deleted successfully. If further confirmation is needed, the rbf_get_register_info() interface
 *  can be used to query all sub-device registration information
 */
int rbf_device_delete(RBF_dev_id_t* id);

/**
 * @brief Delete all sub-devices
 * 
 * @return int 0:Deletion successful, -1: Deletion failed
 * @note A successful response from the interface indicates that the command was successfully issued and in 
 * most cases can be deleted successfully. If further confirmation is needed, the rbf_get_register_info() interface 
 * can be used to query all sub-device registration information
 */
int rbf_device_delete_all(void);


/**
 * @brief RBF Set LED indicator light status for RBF sub-devices
 * 
 * @param id Sub-device addressing address
 * @param indicate LED indicator light status
 * @return int 0: Set successful, -1: Set failed
 * @note For battery-powered devices, the LED indicator light status needs to be sent in the transmission window when 
 * the sub-device reports information. Use cases include:
 * Remote control LED indicator light for arming/disarming status; when a user presses the arming/disarming button on the remote control,
 *  the application receives the key value and sends the LED light indicator status. The remote control receives the previous 
 * LED light indicator status and provides the corresponding indication.
 */
int rbf_device_led_indicate_set(RBF_dev_id_t* id, RBF_led_indicate_t* indicate);


/**
 * @brief Enable Find Me broadcast
 * 
 * @param ids  List of RBF devices to enable Find Me
 * @param count Number of devices in the list to enable Find Me
 * @param retry Retry
 * @return int 0： Successfully enable Find Me mode, -1: Failed to enable Find Me mode
 * @note Find Me broadcast is a bidirectional broadcast. If a device does not respond after the broadcast, a retransmission will be
 *  initiated based on the retry count until a response is successfully received
 * @par Example:
 * @code 
 * RBF_dev_id_t ids[2];
 * ids[0].type = RBF_DEV_IO;
 * ids[0].no = 1;
 * ids[1].type = RBF_DEV_IO;
 * ids[1].no = 2;
 * rbf_start_findme(ids, 2, 2);
 * @endcode
 */
int rbf_start_findme(RBF_dev_id_t* ids, unsigned char count, unsigned char retry);


/**
 * @brief Disable Find Me broadcast
 * 
 * @param ids  List of RBF devices to disable Find Me
 * @param count Number of devices in the list to disable Find Me
 * @return int 0：Successfully disable Find Me mode, -1: Failed to disable Find Me mode
 * @par Example:
 * @code 
 * RBF_dev_id_t ids[2];
 * ids[0].type = RBF_DEV_IO;
 * ids[0].no = 1;
 * ids[1].type = RBF_DEV_IO;
 * ids[1].no = 2;
 * rbf_stop_findme(ids, 2);
 * @endcode
 */
int rbf_stop_findme(RBF_dev_id_t* ids, unsigned char count);


/**
 * @brief Enable RSSI broadcast
 * 
 * @param ids List of RBF devices to enable RSSI broadcast
 * @param count Number of devices in the list to enable RSSI broadcast
 * @return int 0： Successfully enable RSSI broadcast, -1: Failed to enable RSSI broadcast
 * @par Example:
 * @code 
 * RBF_dev_id_t ids[2];
 * ids[0].type = RBF_DEV_IO;
 * ids[0].no = 1;
 * ids[1].type = RBF_DEV_IO;
 * ids[1].no = 2;
 * rbf_start_rssi(ids, 2);
 * @endcode
 */
int rbf_start_rssi(RBF_dev_id_t* ids, unsigned char count);


/**
 * @brief Disable RSSI broadcast
 * 
 * @param ids  List of RBF devices to disable RSSI broadcast
 * @param count Number of devices in the list to disable RSSI broadcast
 * @return int 0： Successfully disable RSSI broadcast, -1: Failed to disable RSSI broadcast
 * @par Example:
 * @code 
 * RBF_dev_id_t ids[2];
 * ids[0].type = RBF_DEV_IO;
 * ids[0].no = 1;
 * ids[1].type = RBF_DEV_IO;
 * ids[1].no = 2;
 * rbf_stop_rssi(ids, 2);
 * @endcode
 */
int rbf_stop_rssi(RBF_dev_id_t* ids, unsigned char count);





/**
 * @brief Batch set the arming/disarming status of IO devices through synchronous broadcast
 * 
 * @param io_list  List of IO devices
 * @param count Number of IO devices
 * @param status Arming/disarming status
 * @return int 0： Successfully set IO arming/disarming status broadcast, -1: Failed to set IO arming/disarming status broadcast
 */
int rbf_device_io_alarm_set(unsigned char* io_list, unsigned char count, RBF_io_alarm_status_t status);



/**
 * @brief Set the frequency band for the RBF hub
 * 
 * @param freq  Currently supports RBF_FREQ_868/RBF_FREQ_915/RBF_FREQ_433
 * @return int 0: Setting successful, -1: Setting failed
 */
int rbf_set_freq(RBF_Freq_t freq);

#ifdef __cplusplus
}
#endif

#endif