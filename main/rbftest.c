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
#include "rbf_indoor_siren.h"
#include "rbf_pir.h"
#include "rbf_emergency_button.h"
#include "rbf_temphumi.h"
#include "rbf_water_leak.h"
#include "rbf_smoke.h"
#include "rbf_keypad.h"
#include "rbf_relay.h"
#include "rbf_wall_switch.h"
#include "rbf_smartplug.h"
#include "rbf_ota.h"
#define  CONFIG_RPC_ENABLE          0
#if defined(CONFIG_RPC_ENABLE) && (CONFIG_RPC_ENABLE == 1)
#include "rpc.h"
#endif

static int s_newDevice = 0;
RBF_Freq_t s_setFreq = RBF_FREQ_915;


int test_rbf_dev_register_reponse(RBF_register_response_t* reponse)
{
    s_newDevice = 1;
    printf("cat:%d, type:%d, no:%d\r\n",reponse->cat, reponse->type, reponse->no);
    printf("ver:%d.%d.%d\r\n",reponse->ver[0], reponse->ver[1], reponse->ver[2]);
    printf("err:%x\r\n",reponse->err);

    printf("sn:");
    for (int i=0; i<RBF_DEVICE_SN_LEN; i++)
    {
        printf("%02X", reponse->sn[i]);
    }
    printf("\r\n");


    printf("mac:");
    for (int i=0; i<RBF_DEVICE_MAC_LEN; i++)
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

int test_rbf_ver_info_handle(RBF_hub_sw_ver_t* ver)
{
    printf("Get hub version: V%d.%d.%d_build%02d%02d%02d%02d\r\n", 
             ver->major, ver->sub, ver->patch,
             ver->build_year, ver->build_month, ver->build_date, ver->build_no);
    return 0;
}

int test_rbf_noise_info_handle(RBF_hub_noise_t* noise)
{
    printf("Get hub noise: real time noise %ld, average noise %ld\r\n", noise->realtime_rssi, noise->avg_rssi);
    return 0;
}

int test_rbf_jamming_handle(bool jamming)
{
    printf("Jamming %d\r\n", (jamming == true) ? 1 : 0);
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


int rbf_smoke_heartbeat_callback(uint8_t no, rbf_smoke_heartbeat_t* heartbeat)
{
    printf("smoke [%d] heartbeat: power %u\n", no, heartbeat->power);
    return 0;
}

int rbf_smoke_input_status_callback(uint8_t no, rbf_smoke_input_status_t* input_status)
{
    printf("smoke [%d] input status:  alarm %d\n", no, input_status->alarm);
    return 0;
}


int rbf_keypad_heartbeat_callback(uint8_t no, rbf_keypad_heartbeat_t* heartbeat)
{
    printf("keypad [%d] heartbeat: power %u\n", no, heartbeat->power);
    return 0;
}

int rbf_keypad_alarm_callback(uint8_t no, rbf_keypad_alarm_status_t* alarm_status)
{
    printf("keypad [%d] emergency_alarm: %d, fire_alarm: %d, medical_alarm: %d, tamper: %d\n", no, alarm_status->emergency_alarm, alarm_status->fire_alarm,
                                                                            alarm_status->medical_alarm, alarm_status->tamper);
    return 0;
}

int rbf_keypad_key_input_callback(uint8_t no, uint8_t input_keys[32], uint8_t input_count)
{
    printf("keypad [%d]  %d keys pressed \n", no, input_count);
    for (int i = 0; i < input_count; i++)
    {
        printf("key %u: %u\n", i, input_keys[i]);
    }
    return 0;
}


int rbf_indoor_siren_heartbeat_callback(uint8_t no, rbf_indoor_siren_heartbeat_t* heartbeat)
{
    printf("Indoor siren [%d] heartbeat: power %u\n", no, heartbeat->power);
    return 0;
}

int rbf_indoor_siren_input_status_callback(uint8_t no, rbf_indoor_siren_input_status_t* input_status)
{
    printf("Indoor siren [%d] input status:  battery_fault %d\n", no, input_status->battery_fault);
    return 0;
}

int rbf_relay_heartbeat_callback(uint8_t no, rbf_relay_heartbeat_t* heartbeat)
{
    printf("Relay [%d] output status:  %d, RSSI: %ld\n", no, heartbeat->onoff, heartbeat->rssi);
    return 0;
}


int rbf_relay_output_status_callback(uint8_t no, rbf_relay_output_status_t* output_status)
{
    printf("Relay [%d] output status:  %d\n", no, output_status->onoff);
    return 0;
}

int rbf_wall_switch_heartbeat_status_callback(uint8_t no, rbf_wall_switch_heartbeat_t* heartbeat)
{
    printf("Wall switch [%d] output status:  %d, over volt: %d, over curr: %d, under volt: %d, over load: %d,"
            "instantaneous voltage %lu, instantaneous current %lu, instantaneous power %lu\n", no, heartbeat->onoff,
                                                                heartbeat->over_volt,
                                                                heartbeat->over_curr,
                                                                heartbeat->under_volt,
                                                                heartbeat->over_load,
                                                                heartbeat->inst_volt,
                                                                heartbeat->inst_curr,
                                                                heartbeat->inst_power);
    return 0;
}
int rbf_wall_switch_output_status_callback(uint8_t no, rbf_wall_switch_output_status_t* output_status)
{
    printf("Wall switch [%d] output status:  %d, over volt: %d, over curr: %d, under volt: %d, over load: %d\n", no, output_status->onoff,
                                                                output_status->over_volt,
                                                                output_status->over_curr,
                                                                output_status->under_volt,
                                                                output_status->over_load);
    return 0;
}



int rbf_smartplug_heartbeat_status_callback(uint8_t no, rbf_smartplug_heartbeat_t* heartbeat)
{
    printf("Smartplug [%d] heartbeat status:  %d, over volt: %d, over curr: %d, under volt: %d, over load: %d, lock: %d,"
            "instantaneous voltage %lu, instantaneous current %lu, instantaneous power %lu,"
            "runtime %lu, Cumulative power %lu\n", no, heartbeat->onoff,
                                                                heartbeat->over_volt,
                                                                heartbeat->over_curr,
                                                                heartbeat->under_volt,
                                                                heartbeat->over_load,
                                                                heartbeat->lock,
                                                                heartbeat->inst_volt,
                                                                heartbeat->inst_curr,
                                                                heartbeat->inst_power,
                                                                heartbeat->run_time,
                                                                heartbeat->cumu_power);
    return 0;
}

int rbf_smartplug_output_status_callback(uint8_t no, rbf_smartplug_output_status_t* output_status)
{
    printf("Smartplug [%d] output status:  %d, over volt: %d, over curr: %d, under volt: %d, over load: %d, lock: %d\n", no, output_status->onoff,
                                                                output_status->over_volt,
                                                                output_status->over_curr,
                                                                output_status->under_volt,
                                                                output_status->over_load,
                                                                output_status->lock);
    return 0;
}

int rbf_ota_request_upgrade_data_callback(unsigned int offset, unsigned int size, unsigned char* data)
{
    printf("OTA request offset: %u, size: %u\n", offset, size);
#if defined(CONFIG_RPC_ENABLE) && (CONFIG_RPC_ENABLE == 1)
    return rpc_fw_data_get(offset, size, data);
#else 
    return -1;
#endif
}

int rbf_ota_upgrade_evt_handle(RBF_ota_evt_t evt, RBF_ota_status_t* status)
{
    printf("OTA evt: %u, upgrading: %u, percent: %u\n", evt, status->upgrading, status->percent);
    return 0;
}

int test_rbf_init(void)
{
    RBF_evt_callbacks_t cbs = {0};
    rbf_magnetic_callbacks_t mc_cbs = {0};
    rbf_keyfob_callbacks_t keyfob_cbs = {0};
    rbf_sounder_callbacks_t sounder_cbs = {0};
    rbf_indoor_siren_callbacks_t indoor_siren_cbs = {0};
    rbf_pir_callbacks_t pir_cbs = {0};
    rbf_emergency_button_callbacks_t emergency_button_cbs = {0};
    rbf_temp_humi_callbacks_t temp_humi_cbs = {0};
    rbf_water_leak_callbacks_t water_leak_cbs = {0};
    rbf_smoke_callbacks_t smoke_cbs = {0};
    rbf_keypad_callbacks_t keypad_cbs = {0};
    rbf_relay_callbacks_t relay_cbs = {0};
    rbf_wall_switch_callbacks_t wall_switch_cbs = {0};
    rbf_smartplug_callbacks_t smartplug_cbs = {0};
    RBF_ota_evt_callbacks_t ota_cbs = {0};

    rbf_init();

    cbs.rbf_dev_register_reponse_handle = &test_rbf_dev_register_reponse;
    cbs.rbf_hub_sync_handle = &test_rbf_set_hub;
    cbs.rbf_hub_event_handle = &test_rbf_dev_hub_event;
    cbs.rbf_dev_register_info_handle = &test_rbf_dev_info_handle;
    cbs.rbf_hub_ver_handle = &test_rbf_ver_info_handle;
    cbs.rbf_get_hub_noise = &test_rbf_noise_info_handle;
    cbs.rbf_jamming_handle = &test_rbf_jamming_handle;
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

    smoke_cbs.hb_cb = &rbf_smoke_heartbeat_callback;
    smoke_cbs.input_status_cb = &rbf_smoke_input_status_callback;
    rbf_smoke_register_callbacks(&smoke_cbs);

    keypad_cbs.hb_cb = &rbf_keypad_heartbeat_callback;
    keypad_cbs.alarm_cb = &rbf_keypad_alarm_callback;
    keypad_cbs.key_input_cb = &rbf_keypad_key_input_callback;
    rbf_keypad_register_callbacks(&keypad_cbs);

    indoor_siren_cbs.hb_cb = &rbf_indoor_siren_heartbeat_callback;
    indoor_siren_cbs.input_status_cb = &rbf_indoor_siren_input_status_callback;
    rbf_indoor_siren_register_callbacks(&indoor_siren_cbs);

    relay_cbs.hb_cb = &rbf_relay_heartbeat_callback;
    relay_cbs.output_status_cb = &rbf_relay_output_status_callback;
    rbf_relay_register_callbacks(&relay_cbs);

    wall_switch_cbs.hb_cb = &rbf_wall_switch_heartbeat_status_callback;
    wall_switch_cbs.output_status_cb = &rbf_wall_switch_output_status_callback;
    rbf_wall_switch_register_callbacks(&wall_switch_cbs);

    smartplug_cbs.hb_cb = &rbf_smartplug_heartbeat_status_callback;
    smartplug_cbs.output_status_cb = &rbf_smartplug_output_status_callback;
    rbf_smartplug_register_callbacks(&smartplug_cbs);

    ota_cbs.rbf_ota_request_upgrade_data_handle = &rbf_ota_request_upgrade_data_callback;
    ota_cbs.rbf_ota_evt_handle = &rbf_ota_upgrade_evt_handle;
    rbf_ota_register_evt_callback(&ota_cbs);
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

void test_findme_start(char *argv[], int argc)
{
    if (argc >= 2)
    {
        RBF_dev_id_t ids;
        ids.cat = (RBF_dev_cat_t)atoi(argv[0]);
        ids.no = atoi(argv[1]);

        printf("findme start: catid %d, no %d\n", ids.cat, ids.no);
        rbf_start_findme(&ids, 1);
    }
}




void test_findme_stop(char *argv[], int argc)
{
    if (argc >= 2)
    {
        RBF_dev_id_t ids;
        ids.cat = (RBF_dev_cat_t)atoi(argv[0]);
        ids.no = atoi(argv[1]);

        printf("findme stop: catid %d, no %d\n", ids.cat, ids.no);
        rbf_stop_findme(&ids, 1);
    }
}

void test_rssi_start(char *argv[], int argc)
{
    if (argc >= 2)
    {
        RBF_dev_id_t ids;
        ids.cat = (RBF_dev_cat_t)atoi(argv[0]);
        ids.no = atoi(argv[1]);

        printf("rssi start: catid %d, no %d\n", ids.cat, ids.no);
        rbf_start_rssi(&ids, 1);
    }
}
void test_rssi_stop(char *argv[], int argc)
{
    if (argc >= 2)
    {
        RBF_dev_id_t ids;
        ids.cat = (RBF_dev_cat_t)atoi(argv[0]);
        ids.no = atoi(argv[1]);

        printf("rssi stop: catid %d, no %d\n", ids.cat, ids.no);
        rbf_stop_rssi(&ids, 1);
    }
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

void test_outdoor_sounder(char *argv[], int argc)
{
    unsigned char sounder_list[1];
    RBF_sounder_param_t souner_param;

    sounder_list[0] = atoi(argv[0]);

    if (argc >= 3)
    {
        souner_param.mode = atoi(argv[1]);
        souner_param.action = atoi(argv[2]);
        printf("Outdoor sounder [%d] control: mode %d, action %d\n",sounder_list[0], souner_param.mode, souner_param.action);
        rbf_sounder_boardcast_control(&sounder_list[0], 1, &souner_param);
    }
    else 
    {
        RBF_sounder_volume_t volume = (RBF_sounder_volume_t)atoi(argv[1]);
        printf("Outdoor sounder [%d] set volume %d\n",sounder_list[0], volume);
        rbf_sounder_volume_set(sounder_list[0], volume);
    }
}

void test_indoor_siren(char *argv[], int argc)
{
    unsigned char indoor_siren_list[1];
    RBF_indoor_siren_param_t indoor_siren_param;

    indoor_siren_list[0] = atoi(argv[0]);

    if (argc >= 3)
    {
        indoor_siren_param.mode = atoi(argv[1]);
        indoor_siren_param.action = atoi(argv[2]);
        printf("Indoor siren [%d] control: mode %d, action %d\n",indoor_siren_list[0], indoor_siren_param.mode, indoor_siren_param.action);
        rbf_indoor_siren_boardcast_control(&indoor_siren_list[0], 1, &indoor_siren_param);
    }
    else 
    {
        RBF_indoor_siren_volume_t volume = (RBF_indoor_siren_volume_t)atoi(argv[1]);
        printf("Indoor siren [%d] set volume %d\n",indoor_siren_list[0], volume);
        rbf_indoor_siren_volume_set(indoor_siren_list[0], volume);
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

void test_setarming(char *argv[], int argc)
{
    if (argc >= 3)
    {
        unsigned char io_list[10];
        unsigned char count = 0;
        RBF_io_alarm_status_t status = RBF_IO_ALARM_DISABLE;

        if (strcmp(argv[0], "enable") == 0) 
        {
            status = RBF_IO_ALARM_ENABLE;
        }

        uint8_t no = atoi(argv[1]);
        count = atoi(argv[2]);

        printf("============set %s==============\r\n", (status ==  RBF_IO_ALARM_ENABLE) ? "arming" : "disarming");
        for (int i = 0; i < count; i++)
        {
            io_list[i] = no+i;
            printf("%d ", io_list[i]);
        }
        printf("\r\n=======================================\r\n");

        rbf_device_io_alarm_set(io_list, count, status);
    }
}



void test_keypad(char *argv[], int argc)
{
    if (argc >= 8)
    {
        rbf_keypad_settings_t settings = {0};
        uint8_t no = atoi(argv[0]);

        settings.alarm_tone_play = atoi(argv[1]);
        settings.err_led_state = atoi(argv[2]);
        settings.arm_led_state = atoi(argv[3]);
        settings.warn_led_state = atoi(argv[4]);
        settings.enable_key_tone = atoi(argv[5]);
        settings.backlight_time = atoi(argv[6]);
        settings.lock_state = atoi(argv[7]);

        printf("set keypad %d: tone play %d, error led %d, arm led %d, warn led %d, key tone %d, backlight time %d, lock %d\r\n", no, settings.alarm_tone_play,
                                                            settings.err_led_state,
                                                            settings.arm_led_state,
                                                            settings.warn_led_state,
                                                            settings.enable_key_tone,
                                                            settings.backlight_time,
                                                            settings.lock_state);
        rbf_keypad_set(no, &settings);
    }
}

void test_relay(char *argv[], int argc)
{
    if (argc >= 2)
    {
        rbf_relay_ctrl_t ctrl = {0};
        uint8_t no = atoi(argv[0]);
        uint8_t action = atoi(argv[1]);

        if (action == 0) {
            ctrl.action = RBF_RELAY_ACTION_OFF;
            printf("relay [%d] set off\r\n", no);
            rbf_relay_ctrl(no, &ctrl);
        } else if (action == 1) {
            ctrl.action = RBF_RELAY_ACTION_ON;
            printf("relay [%d] set on\r\n", no);
            rbf_relay_ctrl(no, &ctrl);
        } else if (action == 2) {
            ctrl.action = RBF_RELAY_ACTION_TOOGLE;
            printf("relay [%d] set toogle\r\n", no);
            rbf_relay_ctrl(no, &ctrl);
        }
    }
}


void test_wall_switch(char *argv[], int argc)
{
    if (argc >= 2)
    {
        rbf_wall_switch_ctrl_t ctrl = {0};
        uint8_t no = atoi(argv[0]);
        uint8_t action = atoi(argv[1]);

        if (action == 0) {
            ctrl.action = RBF_RELAY_ACTION_OFF;
            printf("wall_switch [%d] set off\r\n", no);
            rbf_wall_switch_ctrl(no, &ctrl);
        } else if (action == 1) {
            ctrl.action = RBF_RELAY_ACTION_ON;
            printf("wall_switch [%d] set on\r\n", no);
            rbf_wall_switch_ctrl(no, &ctrl);
        } else if (action == 2) {
            ctrl.action = RBF_RELAY_ACTION_TOOGLE;
            printf("wall_switch [%d] set toogle\r\n", no);
            rbf_wall_switch_ctrl(no, &ctrl);
        }
    }
}

void test_smartplug(char *argv[], int argc)
{
    if (argc >= 3)
    {
        rbf_smartplug_ctrl_t ctrl = {0};
        uint8_t no = atoi(argv[0]);
        uint8_t action = atoi(argv[1]);
        uint8_t lock = atoi(argv[2]);

        ctrl.lock = lock;
        if (action == 0) {
            ctrl.action = RBF_SMARTPLUG_ACTION_OFF;
            printf("smartplug [%d] set off,lock[%d]\r\n", no, lock);
            rbf_smartplug_ctrl(no, &ctrl);
        } else if (action == 1) {
            ctrl.action = RBF_SMARTPLUG_ACTION_ON;
            printf("smartplug [%d] set on,lock[%d]\r\n", no, lock);
            rbf_smartplug_ctrl(no, &ctrl);
        } else if (action == 2) {
            ctrl.action = RBF_SMARTPLUG_ACTION_TOOGLE;
            printf("smartplug [%d] set toogle,lock[%d]\r\n", no, lock);
            rbf_smartplug_ctrl(no, &ctrl);
        }
    }
}

void test_ota(char *argv[], int argc)
{
    if (argc >= 1)
    {
        if (strcmp(argv[0], "start") == 0) 
        {
            printf("start ota\r\n");
            uint32_t fw_size = 0;
#if defined(CONFIG_RPC_ENABLE) && (CONFIG_RPC_ENABLE == 1)
            rpc_fw_size_get();
#endif
            rbf_ota_start(fw_size);
        }
    }
}

void test_hub_get_version(void)
{
    rbf_get_hub_version();
}

void test_hub_get_noise(void)
{
    rbf_get_hub_noise();
}


void init_rbf(char *argv[], int argc)
{
    test_rbf_init();
}
