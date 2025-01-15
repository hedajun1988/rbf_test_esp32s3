/**
 * @file rbftest.c
 * @author Jio (hedajun@hzdusun.com)
 * @brief rbf test interface
 * @version 0.1
 * @date 2024-12-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "rbf_api.h"
#include "rbftest.h"
#include "rbf_magnetic.h"
#include "rbf_keyfob.h"
#include "rbf_sounder.h"
#include "rbf_pir.h"
#include "rbf_emergency_button.h"
#include "rbf_temphumi.h"
#include "rbf_water_leak.h"

static int s_newDevice = 0;
RBF_Freq_t s_setFreq = RBF_FREQ_915;


int test_rbf_dev_register_reponse(RBF_register_response_t* reponse)
{
    s_newDevice = 1;
    printf("cat:%d, type:%d, no:%d\r\n",reponse->cat, reponse->type, reponse->no);
    printf("ver:%d.%d.%d\r\n",reponse->ver[0], reponse->ver[1], reponse->ver[2]);
    printf("err:%x\r\n",reponse->err);

    printf("sn:");
    for (int i=0; i<16; i++)
    {
        printf("%02X", reponse->sn[i]);
    }
    printf("\r\n");


    printf("mac:");
    for (int i=0; i<32; i++)
    {
        printf("%02X", reponse->mac[i]);
    }
    printf("\r\n");
    return 0;
}


int test_rbf_set_hub(void)
{
    printf("init param freq[%d] \n", s_setFreq);
    rbf_set_freq(s_setFreq);
    return 0;
}

int test_rbf_dev_hub_event(BBF_hub_event_t* event)
{
    printf(" RBF_MSG_NORMALHUB sync flag: %d\n", event->sync_flag);
    return 0;
}

int test_rbf_dev_info_handle(RBF_dev_id_t* ids, int count)
{
    printf(" test_rbf_dev_info_handle [%d] device\n", count);
    for (int i = 0; i < count; i++)
    {
        printf("cat:%d, no:%d\r\n", ids[i].cat, ids[i].no);
    }
    return 0;
}

int rbf_magnetic_heartbeat_callback(uint8_t no, rbf_magnetic_heartbeat_t* heartbeat)
{
    printf("magnetic [%d] heartbeat: power %u\n", no, heartbeat->power);
    return 0;
}


int rbf_magnetic_input_status_callback(uint8_t no, rbf_magnetic_input_status_t* input_status)
{
    printf("magnetic [%d] input status:  alarm %d, tamper %d\n", no, input_status->alarm, input_status->tamper);
    return 0;
}

int rbf_sounder_heartbeat_callback(uint8_t no, rbf_sounder_heartbeat_t* heartbeat)
{
    printf("sounder [%d] heartbeat: power %u\n", no, heartbeat->power);
    return 0;
}

int rbf_sounder_input_status_callback(uint8_t no, rbf_sounder_input_status_t* input_status)
{
    printf("sounder [%d] input status:  power_supply_status %d, solar_panels_fault %d,"
                        "battery_fault %d,  charge_status %d,  tamper %d\n", no, input_status->power_supply_status, 
                                                                            input_status->solar_panels_fault,
                                                                            input_status->battery_fault,
                                                                            input_status->charge_status,
                                                                            input_status->tamper);
    return 0;
}

int rbf_pir_heartbeat_callback(uint8_t no, rbf_pir_heartbeat_t* heartbeat)
{
    printf("pir [%d] heartbeat: power %u\n", no, heartbeat->power);
    return 0;
}

int rbf_pir_input_evt_callback(uint8_t no, rbf_pir_input_evt_t input_evt)
{
    printf("pir [%d] input evt:  %x\n", no, input_evt);
    return 0;
}

int rbf_pir_input_status_callback(uint8_t no, rbf_pir_input_status_t* input_status)
{
    printf("pir [%d] input status:  tamper %d, fault %d\n", no, input_status->tamper, input_status->fault);
    return 0;
}

int rbf_keyfob_key_press_callback(uint8_t no, uint8_t key)
{
    RBF_dev_id_t id;
    RBF_led_indicate_t indicate;

    printf("keyfob [%d] key press: %u\n", no, key);
    id.cat = RBF_DEV_KEYFOB;
    id.no = no;

    /* Provide different LED indications for Button 1 and Button 2. */
    if (key == 1) 
    {
        indicate.mode = RBF_LED_INDICATE_GREEN_LIGHT_TWICE;
        indicate.duration = RBF_LED_INDICATE_DURATION_500MS;
        rbf_device_led_indicate_set(&id, &indicate);
    }
    else if (key == 2)
    {
        indicate.mode = RBF_LED_INDICATE_RED_LIGHT_TWICE;
        indicate.duration = RBF_LED_INDICATE_DURATION_500MS;
        rbf_device_led_indicate_set(&id, &indicate);
    }
    return 0;
}

int rbf_emergency_button_heartbeat_callback(uint8_t no, rbf_emergency_button_heartbeat_t* heartbeat)
{
    printf("emergency button [%d] heartbeat: power %u\n", no, heartbeat->power);
    return 0;
}

int rbf_emergency_button_input_evt_callback(uint8_t no, rbf_emergency_button_input_evt_t input_evt)
{
    printf("emergency button [%d] input evt:  %x\n", no, input_evt);
    return 0;
}

int rbf_temp_humi_heartbeat_callback(uint8_t no, rbf_temp_humi_heartbeat_t* heartbeat)
{
    printf("Temperature and humidity sensor [%d] heartbeat: power %u, humi %f, temp %f\n", no, heartbeat->power, heartbeat->humi, heartbeat->temp);
    return 0;
}

int rbf_temp_humi_input_status_callback(uint8_t no, rbf_temp_humi_status_t* input_status)
{
    printf("Temperature and humidity sensor [%d] input status:  over temp %d, low temp %d, over humi %d, low humi %d\n", no, 
                                                                                                            input_status->over_temp_alarm,
                                                                                                            input_status->low_temp_alarm,
                                                                                                            input_status->over_humi_alarm,
                                                                                                            input_status->low_humi_alarm);
    return 0;
}

int rbf_water_leak_heartbeat_callback(uint8_t no, rbf_water_leak_heartbeat_t* heartbeat)
{
    printf("Water leak [%d] heartbeat: power %u\n", no, heartbeat->power);
    return 0;
}

int rbf_water_leak_input_status_callback(uint8_t no, rbf_water_leak_input_status_t* input_status)
{
    printf("Water leak [%d] input status:  leakage alarm %d\n", no, input_status->alarm);
    return 0;
}


int test_rbf_init(void)
{
    RBF_evt_callbacks_t cbs = {0};
    rbf_magnetic_callbacks_t mc_cbs = {0};
    rbf_keyfob_callbacks_t keyfob_cbs = {0};
    rbf_sounder_callbacks_t sounder_cbs = {0};
    rbf_pir_callbacks_t pir_cbs = {0};
    rbf_emergency_button_callbacks_t emergency_button_cbs = {0};
    rbf_temp_humi_callbacks_t temp_humi_cbs = {0};
    rbf_water_leak_callbacks_t water_leak_cbs = {0};

    rbf_init();

    cbs.rbf_dev_register_reponse_handle = &test_rbf_dev_register_reponse;
    cbs.rbf_hub_sync_handle = &test_rbf_set_hub;
    cbs.rbf_hub_event_handle = &test_rbf_dev_hub_event;
    cbs.rbf_dev_register_info_handle = &test_rbf_dev_info_handle;
    rbf_register_evt_callback(&cbs);

    mc_cbs.hb_cb = &rbf_magnetic_heartbeat_callback;
    mc_cbs.input_status_cb = &rbf_magnetic_input_status_callback;
    rbf_magnetic_register_callbacks(&mc_cbs);

    keyfob_cbs.key_press_cb = &rbf_keyfob_key_press_callback;
    rbf_keyfob_register_callbacks(&keyfob_cbs);

    sounder_cbs.hb_cb = &rbf_sounder_heartbeat_callback;
    sounder_cbs.input_status_cb = &rbf_sounder_input_status_callback;
    rbf_sounder_register_callbacks(&sounder_cbs);

    pir_cbs.hb_cb = &rbf_pir_heartbeat_callback;
    pir_cbs.input_evt_cb = &rbf_pir_input_evt_callback;
    pir_cbs.input_status_cb = &rbf_pir_input_status_callback;
    rbf_pir_register_callbacks(&pir_cbs);

    emergency_button_cbs.hb_cb = &rbf_emergency_button_heartbeat_callback;
    emergency_button_cbs.input_evt_cb = &rbf_emergency_button_input_evt_callback;
    rbf_emergency_button_register_callbacks(&emergency_button_cbs);

    temp_humi_cbs.hb_cb = &rbf_temp_humi_heartbeat_callback;
    temp_humi_cbs.input_status_cb = &rbf_temp_humi_input_status_callback;
    rbf_temp_humi_register_callbacks(&temp_humi_cbs);

    water_leak_cbs.hb_cb = &rbf_water_leak_heartbeat_callback;
    water_leak_cbs.input_status_cb = &rbf_water_leak_input_status_callback;
    rbf_water_leak_register_callbacks(&water_leak_cbs);
    return 0;
}

int test_start_hub_enroll()
{

    RBF_register_param_t register_param = {0};
    register_param.type = RBF_REGISTER_LOCAL;
    s_newDevice = 0;
    int ret = rbf_start_hub_register(&register_param);
    int i=0;
    while(!s_newDevice)
    {
        if(s_newDevice || i>60)
        {
            i = 0;
            break;
        }
        sleep(1);
        printf("enroll[%d]\n",i++);
    }
    /* Stop batch registration. */
    ret = rbf_stop_hub_register();
    return ret;
}

int test_stop_hub_enroll()
{
    printf("test_stop_hub_enroll\n");
    rbf_stop_hub_register();
    return 0;
}


void test_enroll(char *argv[], int argc)
{
    test_start_hub_enroll();
}
void test_stop_enroll(char *argv[], int argc)
{
    test_stop_hub_enroll();
}


void test_ledset(char *argv[], int argc)
{
    RBF_dev_id_t id;
    RBF_led_indicate_t indicator;

    if (argc < 4)
    {
        return;
    }

    id.cat = atoi(argv[0]);
    id.no = atoi(argv[1]);
    indicator.mode = atoi(argv[2]);
    indicator.duration = atoi(argv[3]);
    printf("led set: cat %d, no %d, mode %d, duration %d\n", id.cat, id.no, indicator.mode, indicator.duration);
    rbf_device_led_indicate_set(&id, &indicator);
}

void test_delete(char *argv[], int argc)
{
    RBF_dev_id_t id;

    if (argc < 2)
    {
        return;
    }

    if (strcmp(argv[0], "all") == 0) 
    {
        printf("device delete all\n");
        rbf_device_delete_all();
    }
    else 
    {
        id.cat = atoi(argv[0]);
        id.no = atoi(argv[1]);
        printf("device delete: cat %d, no %d\n", id.cat, id.no);
        rbf_device_delete(&id);
    }
}

void test_list(char *argv[], int argc)
{
    (void)argv;
    (void)argc;

    /* Request registration information, which will be returned through a callback function. */
    rbf_get_register_info();
}

void test_sounder(char *argv[], int argc)
{
    unsigned char sounder_list[1];
    RBF_sounder_param_t souner_param;

    sounder_list[0] = atoi(argv[0]);

    if (argc >= 3)
    {
        souner_param.mode = atoi(argv[1]);
        souner_param.action = atoi(argv[2]);
        printf("sounder [%d] control: mode %d, action %d\n",sounder_list[0], souner_param.mode, souner_param.action);
        rbf_sounder_boardcast_control(&sounder_list[0], 1, &souner_param);
    }
    else 
    {
        RBF_sounder_volume_t volume = (RBF_sounder_volume_t)atoi(argv[1]);
        printf("sounder [%d] set volume %d\n",sounder_list[0], volume);
        rbf_sounder_volume_set(sounder_list[0], volume);
    }
}

void test_pir(char *argv[], int argc)
{
    rbf_pir_config_t config;
    uint8_t no = 0;

    no = atoi(argv[0]);
    config.tamper_enable = atoi(argv[1]);
    config.sensitivity = atoi(argv[2]);

    printf("pir [%d] set tamper %d, sensitivity %d\n",no, config.tamper_enable, config.sensitivity);
    rbf_pir_set(no, &config);
}

void test_temphumi(char *argv[], int argc)
{
    if (argc >= 3)
    {
        rbf_temp_humi_config_t config;
        uint8_t no = 0;

        no = atoi(argv[0]);
        config.temp_units = atoi(argv[1]);
        config.temp_threshold = atoi(argv[2]);
        config.humi_threshold = atoi(argv[3]);

        printf("temphumi [%d] set unit %d, temp_thres %d, humi_thres %d\n",no, config.temp_units, config.temp_threshold, config.humi_threshold);
        rbf_temp_humi_set(no, &config);
    }
}

void test_setfreq(char *argv[], int argc)
{
    if (argc >= 1)
    {
        int freq_a = atoi(argv[0]);
        switch (freq_a)
        {
        case 0:
            printf("set hub frequency to 868\r\n");
            rbf_set_freq(RBF_FREQ_868);
            s_setFreq = RBF_FREQ_868;
            break;

        case 1:
            printf("set hub frequency to 915\r\n");
            rbf_set_freq(RBF_FREQ_915);
            s_setFreq = RBF_FREQ_915;
            break;

        case 2: 
            printf("set hub frequency to 433\r\n");
            rbf_set_freq(RBF_FREQ_433);
            s_setFreq = RBF_FREQ_433;
            break;
        
        default:
            printf("unknow hub frequency\r\n");
            break;
        }
    }
}

void init_rbf(char *argv[], int argc)
{
    test_rbf_init();
}
