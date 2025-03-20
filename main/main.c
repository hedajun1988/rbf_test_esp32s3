/**
 * @file main.c
 * @author Jio (hedajun@hzdusun.com)
 * @brief esp32s3 rbf test example
 * @version 0.1
 * @date 2024-12-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"

#include "argtable3/argtable3.h"
#include "esp_check.h"
#include "esp_console.h"

#include "rbftest.h"
#include "driver/uart_vfs.h"
#include "driver/uart.h"

#include "rbf_api.h"
#define  CONFIG_RPC_ENABLE          0


#define RBF_UART_PORT          2

#if defined(CONFIG_RPC_ENABLE) && (CONFIG_RPC_ENABLE == 1)
#include "rpc.h"
#define RPC_UART_PORT        1
rpc_helper_t rpc_helper;
#endif

static const char* TAG = "rbftest";
static char rbf_args[10][10];


static int rbf_port_read(unsigned char *buf, int bufsize, int timeout)
{
    int len = uart_read_bytes(RBF_UART_PORT, buf, bufsize, timeout / portTICK_PERIOD_MS);
    return len;
}

static int rbf_port_write(unsigned char *data, int dataLen)
{
    return uart_write_bytes(RBF_UART_PORT, (const char *) data, dataLen);
}

#if defined(CONFIG_RPC_ENABLE) && (CONFIG_RPC_ENABLE == 1)

static int rpc_read(uint8_t *buf, uint32_t bufsize, uint32_t timeout)
{
    int len = uart_read_bytes(RPC_UART_PORT, buf, bufsize, timeout / portTICK_PERIOD_MS);
    return len;
}

static int rpc_write(uint8_t *data, uint32_t dataLen)
{
    return uart_write_bytes(RPC_UART_PORT, (const char *) data, dataLen);
}

static void rbf_port_reset()
{
    rpc_module_reset();
}

#endif

static int do_test_init(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    init_rbf(NULL, 0);
    return 0;
}

static struct {
     struct arg_lit *stop;
     struct arg_end *end;
}enroll_args;

static int do_test_enroll(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void **) &enroll_args);
    if (nerrors != 0) 
    {
        arg_print_errors(stderr, enroll_args.end, argv[0]);
        return 0;
    }

    if (enroll_args.stop->count) 
    {
        test_stop_enroll(NULL, 0);
    }
    else 
    {
        test_enroll(NULL, 0);
    }

    return 0;
}

static struct {
     struct arg_str *cat_id;
     struct arg_str *no;
     struct arg_lit *stop;
     struct arg_end *end;
}boardcast_args;

static int do_test_findme(int argc, char **argv)
{
    char* findme_argv[4];
    int nerrors = arg_parse(argc, argv, (void **) &boardcast_args);
    if (nerrors != 0) 
    {
        arg_print_errors(stderr, boardcast_args.end, argv[0]);
        return 0;
    }

    strcpy(&rbf_args[0][0], boardcast_args.cat_id->sval[0]);
    findme_argv[0] = &rbf_args[0][0];

    strcpy(&rbf_args[1][0], boardcast_args.no->sval[0]);
    findme_argv[1] = &rbf_args[1][0];

    if (boardcast_args.stop->count) 
    {
        test_findme_stop(findme_argv, 2);
    }
    else 
    {
        test_findme_start(findme_argv, 2);
    }

    return 0;
}

static int do_test_rssi(int argc, char **argv)
{
    char* rssi_argv[4];
    int nerrors = arg_parse(argc, argv, (void **) &boardcast_args);
    if (nerrors != 0) 
    {
        arg_print_errors(stderr, boardcast_args.end, argv[0]);
        return 0;
    }

    strcpy(&rbf_args[0][0], boardcast_args.cat_id->sval[0]);
    rssi_argv[0] = &rbf_args[0][0];

    strcpy(&rbf_args[1][0], boardcast_args.no->sval[0]);
    rssi_argv[1] = &rbf_args[1][0];

    if (boardcast_args.stop->count) 
    {
        test_rssi_stop(rssi_argv, 2);
    }
    else 
    {
        test_rssi_start(rssi_argv, 2);
    }

    return 0;
}


static struct {
     struct arg_str *cat_id;
     struct arg_str *no;
     struct arg_str *led_mode;
     struct arg_str *led_duration;
     struct arg_end *end;
}led_args;

static int do_test_ledset(int argc, char **argv)
{
    char* led_argv[4];
    int nerrors = arg_parse(argc, argv, (void **) &led_args);
    if (nerrors != 0) 
    {
        arg_print_errors(stderr, led_args.end, argv[0]);
        return 0;
    }

    strcpy(&rbf_args[0][0], led_args.cat_id->sval[0]);
    led_argv[0] = &rbf_args[0][0];

    strcpy(&rbf_args[1][0], led_args.no->sval[0]);
    led_argv[1] = &rbf_args[1][0];

    strcpy(&rbf_args[2][0], led_args.led_mode->sval[0]);
    led_argv[2] = &rbf_args[2][0];

    strcpy(&rbf_args[3][0], led_args.led_duration->sval[0]);
    led_argv[3] = &rbf_args[3][0];


    test_ledset(led_argv, 4);
    return 0;
}

static struct {
     struct arg_lit *delete;
     struct arg_lit *all;
     struct arg_str *cat_id;
     struct arg_str *no;
     struct arg_end *end;
}device_args;

static int do_test_device(int argc, char **argv)
{
    char* device_argv[4];
    int nerrors = arg_parse(argc, argv, (void **) &device_args);
    if (nerrors != 0) 
    {
        arg_print_errors(stderr, device_args.end, argv[0]);
        return 0;
    }

    if (device_args.delete->count) 
    {
        if (device_args.all->count) 
        {
            strcpy(&rbf_args[0][0], "all");
        }
        else 
        {
            strcpy(&rbf_args[0][0], device_args.cat_id->sval[0]);
        }
        device_argv[0] = &rbf_args[0][0];

        strcpy(&rbf_args[1][0], device_args.no->sval[0]);
        device_argv[1] = &rbf_args[1][0];

        test_delete(device_argv, 2);
    }
    else 
    {
        test_list(NULL, 0);
    }

    return 0;
}

static struct {
     struct arg_str *no;
     struct arg_str *mode;
     struct arg_str *action;
     struct arg_str *volume;
     struct arg_end *end;
}sounder_args;

static int do_test_out_sounder(int argc, char **argv)
{
    char* sounder_argv[4];
    int nerrors = arg_parse(argc, argv, (void **) &sounder_args);
    if (nerrors != 0) 
    {
        arg_print_errors(stderr, sounder_args.end, argv[0]);
        return 0;
    }


    strcpy(&rbf_args[0][0], sounder_args.no->sval[0]);
    sounder_argv[0] = &rbf_args[0][0];

    if (sounder_args.volume->count > 0) 
    {
        strcpy(&rbf_args[1][0], sounder_args.volume->sval[0]);
        sounder_argv[1] = &rbf_args[1][0];

        test_outdoor_sounder(sounder_argv, 2);
    }
    else 
    {
        strcpy(&rbf_args[1][0], sounder_args.mode->sval[0]);
        sounder_argv[1] = &rbf_args[1][0];

        strcpy(&rbf_args[2][0], sounder_args.action->sval[0]);
        sounder_argv[2] = &rbf_args[2][0];

        test_outdoor_sounder(sounder_argv, 3);
    }

    return 0;
}

static int do_test_indoor_siren(int argc, char **argv)
{
    char* indoor_siren_argv[4];
    int nerrors = arg_parse(argc, argv, (void **) &sounder_args);
    if (nerrors != 0) 
    {
        arg_print_errors(stderr, sounder_args.end, argv[0]);
        return 0;
    }


    strcpy(&rbf_args[0][0], sounder_args.no->sval[0]);
    indoor_siren_argv[0] = &rbf_args[0][0];

    if (sounder_args.volume->count > 0) 
    {
        strcpy(&rbf_args[1][0], sounder_args.volume->sval[0]);
        indoor_siren_argv[1] = &rbf_args[1][0];

        test_indoor_siren(indoor_siren_argv, 2);
    }
    else 
    {
        strcpy(&rbf_args[1][0], sounder_args.mode->sval[0]);
        indoor_siren_argv[1] = &rbf_args[1][0];

        strcpy(&rbf_args[2][0], sounder_args.action->sval[0]);
        indoor_siren_argv[2] = &rbf_args[2][0];

        test_indoor_siren(indoor_siren_argv, 3);
    }

    return 0;
}

static struct {
     struct arg_str *no;
     struct arg_str *tamper;
     struct arg_str *sensitivity;
     struct arg_end *end;
}pir_args;

static int do_test_pir(int argc, char **argv)
{
    char* pir_argv[4];
    int nerrors = arg_parse(argc, argv, (void **) &pir_args);
    if (nerrors != 0) 
    {
        arg_print_errors(stderr, pir_args.end, argv[0]);
        return 0;
    }


    strcpy(&rbf_args[0][0], pir_args.no->sval[0]);
    pir_argv[0] = &rbf_args[0][0];

    strcpy(&rbf_args[1][0], pir_args.tamper->sval[0]);
    pir_argv[1] = &rbf_args[1][0];

    strcpy(&rbf_args[2][0], pir_args.sensitivity->sval[0]);
    pir_argv[2] = &rbf_args[2][0];

    test_pir(pir_argv, 3);
    
    return 0;
}

static struct {
     struct arg_str *no;
     struct arg_str *units;
     struct arg_str *temp_threshold;
     struct arg_str *humi_threshold;
     struct arg_end *end;
}temphumi_args;

static int do_test_temphumi(int argc, char **argv)
{
    char* temp_humi_argv[4];
    int nerrors = arg_parse(argc, argv, (void **) &temphumi_args);
    if (nerrors != 0) 
    {
        arg_print_errors(stderr, temphumi_args.end, argv[0]);
        return 0;
    }


    strcpy(&rbf_args[0][0], temphumi_args.no->sval[0]);
    temp_humi_argv[0] = &rbf_args[0][0];

    strcpy(&rbf_args[1][0], temphumi_args.units->sval[0]);
    temp_humi_argv[1] = &rbf_args[1][0];

    strcpy(&rbf_args[2][0], temphumi_args.temp_threshold->sval[0]);
    temp_humi_argv[2] = &rbf_args[2][0];

    strcpy(&rbf_args[3][0], temphumi_args.humi_threshold->sval[0]);
    temp_humi_argv[3] = &rbf_args[3][0];

    test_temphumi(temp_humi_argv, 4);
    
    return 0;
}

static struct {
     struct arg_str *freq;
     struct arg_end *end;
}setfreq_args;


static int do_test_setfreq(int argc, char **argv)
{
    char* setfreq_argv[4];
    int nerrors = arg_parse(argc, argv, (void **) &setfreq_args);
    if (nerrors != 0) 
    {
        arg_print_errors(stderr, setfreq_args.end, argv[0]);
        return 0;
    }


    strcpy(&rbf_args[0][0], setfreq_args.freq->sval[0]);
    setfreq_argv[0] = &rbf_args[0][0];

    test_setfreq(setfreq_argv, 1);
    
    return 0;
}

static struct {
     struct arg_lit *enable;
     struct arg_lit *disable;
     struct arg_str *start_no;
     struct arg_str *count;
     struct arg_end *end;
}arming_args;

static int do_test_setarming(int argc, char **argv)
{
    char* arming_argv[4];
    int nerrors = arg_parse(argc, argv, (void **) &arming_args);
    if (nerrors != 0) 
    {
        arg_print_errors(stderr, arming_args.end, argv[0]);
        return 0;
    }

    if (arming_args.enable->count) 
    {
        strcpy(&rbf_args[0][0], "enable");
        arming_argv[0] = &rbf_args[0][0];
    }
    else 
    {
        strcpy(&rbf_args[0][0], "disable");
        arming_argv[0] = &rbf_args[0][0];
    }

    strcpy(&rbf_args[1][0], arming_args.start_no->sval[0]);
    arming_argv[1] = &rbf_args[1][0];
    
    strcpy(&rbf_args[2][0], arming_args.count->sval[0]);
    arming_argv[2] = &rbf_args[2][0];
    
    test_setarming(arming_argv, 3);
    return 0;
}


static struct {
     struct arg_str *no;
     struct arg_str *tone_play;
     struct arg_str *err_led;
     struct arg_str *arm_led;
     struct arg_str *warn_led;
     struct arg_str *key_tone_enable;
     struct arg_str *backlight_time;
     struct arg_str *lock;
     struct arg_end *end;
}keypad_args;

static int do_test_keypad(int argc, char **argv)
{
    char* keypad_argv[10];
    int nerrors = arg_parse(argc, argv, (void **) &keypad_args);
    if (nerrors != 0) 
    {
        arg_print_errors(stderr, keypad_args.end, argv[0]);
        return 0;
    }

    strcpy(&rbf_args[0][0], keypad_args.no->sval[0]);
    keypad_argv[0] = &rbf_args[0][0];

    strcpy(&rbf_args[1][0], keypad_args.tone_play->sval[0]);
    keypad_argv[1] = &rbf_args[1][0];
    
    strcpy(&rbf_args[2][0], keypad_args.err_led->sval[0]);
    keypad_argv[2] = &rbf_args[2][0];

    strcpy(&rbf_args[3][0], keypad_args.arm_led->sval[0]);
    keypad_argv[3] = &rbf_args[3][0];

    strcpy(&rbf_args[4][0], keypad_args.warn_led->sval[0]);
    keypad_argv[4] = &rbf_args[4][0];

    strcpy(&rbf_args[5][0], keypad_args.key_tone_enable->sval[0]);
    keypad_argv[5] = &rbf_args[5][0];

    strcpy(&rbf_args[6][0], keypad_args.backlight_time->sval[0]);
    keypad_argv[6] = &rbf_args[6][0];

    strcpy(&rbf_args[7][0], keypad_args.lock->sval[0]);
    keypad_argv[7] = &rbf_args[7][0];
    
    test_keypad(keypad_argv, 8);
    return 0;
}

static struct {
     struct arg_str *no;
     struct arg_str *action;
     struct arg_end *end;
}relay_args;

static int do_test_relay(int argc, char **argv)
{
    char* relay_argv[4];
    int nerrors = arg_parse(argc, argv, (void **) &relay_args);
    if (nerrors != 0) 
    {
        arg_print_errors(stderr, relay_args.end, argv[0]);
        return 0;
    }


    strcpy(&rbf_args[0][0], relay_args.no->sval[0]);
    relay_argv[0] = &rbf_args[0][0];

    strcpy(&rbf_args[1][0], relay_args.action->sval[0]);
    relay_argv[1] = &rbf_args[1][0];

    test_relay(relay_argv, 2);
    return 0;
}

static int do_test_wallswitch(int argc, char **argv)
{
    char* wall_switch_argv[4];
    int nerrors = arg_parse(argc, argv, (void **) &relay_args);
    if (nerrors != 0) 
    {
        arg_print_errors(stderr, relay_args.end, argv[0]);
        return 0;
    }


    strcpy(&rbf_args[0][0], relay_args.no->sval[0]);
    wall_switch_argv[0] = &rbf_args[0][0];

    strcpy(&rbf_args[1][0], relay_args.action->sval[0]);
    wall_switch_argv[1] = &rbf_args[1][0];

    test_wall_switch(wall_switch_argv, 2);
    return 0;
}


static struct {
     struct arg_str *no;
     struct arg_str *action;
     struct arg_str *lock;
     struct arg_end *end;
}smartplug_args;

static int do_test_smartplug(int argc, char **argv)
{
    char* smartplug_plug[4];
    int nerrors = arg_parse(argc, argv, (void **) &smartplug_args);
    if (nerrors != 0) 
    {
        arg_print_errors(stderr, smartplug_args.end, argv[0]);
        return 0;
    }


    strcpy(&rbf_args[0][0], smartplug_args.no->sval[0]);
    smartplug_plug[0] = &rbf_args[0][0];

    strcpy(&rbf_args[1][0], smartplug_args.action->sval[0]);
    smartplug_plug[1] = &rbf_args[1][0];

    strcpy(&rbf_args[2][0], smartplug_args.lock->sval[0]);
    smartplug_plug[2] = &rbf_args[2][0];

    test_smartplug(smartplug_plug, 3);
    return 0;
}

static struct {
     struct arg_lit *version;
     struct arg_lit *noise;
     struct arg_end *end;
}hub_args;

static int do_test_hub(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void **) &hub_args);
    if (nerrors != 0) 
    {
        arg_print_errors(stderr, hub_args.end, argv[0]);
        return 0;
    }

    if (hub_args.version->count) 
    {
        test_hub_get_version();
    }

    if (hub_args.noise->count) 
    {
        test_hub_get_noise();
    }
    
    return 0;
}



static struct {
     struct arg_lit *start;
     struct arg_end *end;
}ota_args;

static int do_test_ota(int argc, char **argv)
{
    char* ota[1];
    int nerrors = arg_parse(argc, argv, (void **) &ota_args);
    if (nerrors != 0) 
    {
        arg_print_errors(stderr, ota_args.end, argv[0]);
        return 0;
    }

    if (ota_args.start->count) 
    {
        strcpy(&rbf_args[0][0], "start");
        ota[0] = &rbf_args[0][0];
    }

    test_ota(ota, 1);
    return 0;
}
/* test cmds */
const esp_console_cmd_t cmds[] = {
    {
        .help = "init rfb",
        .hint = NULL,
        .func = do_test_init,
        .command = "init",
    },
    {
        .help = "come in/stop enroll",
        .hint = NULL,
        .func = do_test_enroll,
        .command = "enroll",
        .argtable = &enroll_args,
    },
    {
        .help = "send findme boardcast",
        .hint = NULL,
        .func = do_test_findme,
        .command = "findme",
        .argtable = &boardcast_args,
    },
    {
        .help = "send rssi test boardcast",
        .hint = NULL,
        .func = do_test_rssi,
        .command = "rssi",
        .argtable = &boardcast_args,
    },
    {
        .help = "hub set/get information",
        .hint = NULL,
        .func = do_test_hub,
        .command = "hub",
        .argtable = &hub_args,
    },
    {
        .help = "led indicate set",
        .hint = NULL,
        .func = do_test_ledset,
        .command = "ledset",
        .argtable = &led_args,
    },
    {
        .help = "list/delete device",
        .hint = NULL,
        .func = do_test_device,
        .command = "device",
        .argtable = &device_args,
    },
    {
        .help = "Outdoor sounder test",
        .hint = NULL,
        .func = do_test_out_sounder,
        .command = "outsounder",
        .argtable = &sounder_args
    },
    {
        .help = "InDoor siren test",
        .hint = NULL,
        .func = do_test_indoor_siren,
        .command = "insiren",
        .argtable = &sounder_args
    },
    {
        .help = "pir config",
        .hint = NULL,
        .func = do_test_pir,
        .command = "pir",
        .argtable = &pir_args
    },
    {
        .help = "temphumi config",
        .hint = NULL,
        .func = do_test_temphumi,
        .command = "temphumi",
        .argtable = &temphumi_args
    },
    {
        .help = "set rbf frequency",
        .hint = NULL,
        .func = do_test_setfreq,
        .command = "setfreq",
        .argtable = &setfreq_args
    },
    {
        .help = "set arming/disarming status",
        .hint = NULL,
        .func = do_test_setarming,
        .command = "setarming",
        .argtable = &arming_args
    },
    {
        .help = "keypad test",
        .hint = NULL,
        .func = do_test_keypad,
        .command = "keypad",
        .argtable = &keypad_args
    },
    {
        .help = "relay test",
        .hint = NULL,
        .func = do_test_relay,
        .command = "relay",
        .argtable = &relay_args
    },
    {
        .help = "wall switch test",
        .hint = NULL,
        .func = do_test_wallswitch,
        .command = "wallswitch",
        .argtable = &relay_args
    },
    {
        .help = "smart plug test",
        .hint = NULL,
        .func = do_test_smartplug,
        .command = "smartplug",
        .argtable = &smartplug_args
    },
    {
        .help = "ota test",
        .hint = NULL,
        .func = do_test_ota,
        .command = "ota",
        .argtable = &ota_args
    },
};

esp_err_t app_console_init(void)
{
    esp_console_repl_t *repl = NULL;
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    esp_console_dev_uart_config_t uart_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    repl_config.prompt = "rbf>";
    repl_config.task_stack_size = 32*1024;

    ESP_ERROR_CHECK(esp_console_new_repl_uart(&uart_config, &repl_config, &repl));

    enroll_args.stop = arg_lit0("s", "stop", "stop enroll");
    enroll_args.end = arg_end(2);

    boardcast_args.cat_id = arg_str1(NULL, NULL,"<CAT_ID>", "cat id: 1-io 2-sounder 3-keypad 4-keyfob");
    boardcast_args.no = arg_str1(NULL, NULL,"<DEV_NUM>", "device number");
    boardcast_args.stop = arg_lit0("s", "stop", "stop");
    boardcast_args.end = arg_end(3);
    
    hub_args.version = arg_lit0("v", "version","HUB version");
    hub_args.noise = arg_lit0("n", "noise", "HUB noise");
    hub_args.end = arg_end(2);

    led_args.cat_id = arg_str1(NULL, NULL,"<CAT_ID>", "cat id: 1-io 2-sounder 3-keypad 4-keyfob");
    led_args.no = arg_str1(NULL, NULL,"<DEV_NUM>", "device number");
    led_args.led_mode = arg_str1(NULL, NULL,"<MODE>", "led mode");
    led_args.led_duration = arg_str1(NULL, NULL,"<DURATION>", "led duration");
    led_args.end = arg_end(4);

    device_args.delete = arg_lit0("d", "delete",  "delete devices");
    device_args.all = arg_lit0("a", "all",  "delete all devices");
    device_args.cat_id = arg_str0(NULL, NULL,"<CAT_ID>", "cat id: 1-io 2-sounder 3-keypad 4-keyfob");
    device_args.no = arg_str0(NULL, NULL,"<DEV_NUM>", "device number");
    device_args.end = arg_end(2);

    sounder_args.no = arg_str1(NULL, NULL,"<DEV_NUM>", "device number");
    sounder_args.mode = arg_str0(NULL, NULL,"<SOUNDER_MODE>", "sounder mode");
    sounder_args.action = arg_str0(NULL, NULL,"<SOUNDER_ACTION>", "sounder action");
    sounder_args.volume = arg_str0("v", "volume","<VOLLUME>", "sounder volume");
    sounder_args.end = arg_end(3);

    pir_args.no = arg_str1(NULL, NULL,"<DEV_NUM>", "device number");
    pir_args.tamper = arg_str1(NULL, NULL,"<PIR_TAMPER>", "PIR tamper");
    pir_args.sensitivity = arg_str1(NULL, NULL,"<PIR_SENSITIVITY>", "PIR sensitivity");
    pir_args.end = arg_end(3);

    temphumi_args.no = arg_str1(NULL, NULL,"<DEV_NUM>", "device number");
    temphumi_args.units = arg_str1(NULL, NULL,"<TEMP_UNITS>", "temprature units");
    temphumi_args.temp_threshold = arg_str1(NULL, NULL,"<TEMP_THRES>", "temprature threshold");
    temphumi_args.humi_threshold = arg_str1(NULL, NULL,"<HUMI_THRES>", "humidity threshold");
    temphumi_args.end = arg_end(4);

    setfreq_args.freq = arg_str1(NULL, NULL,"<freq>", "0:868 1:915 2:433");
    setfreq_args.end = arg_end(1);

    arming_args.enable = arg_lit0("e", "enable",  "set arming state");
    arming_args.disable = arg_lit0("d", "disable",  "set disarming state");
    arming_args.start_no = arg_str0(NULL, NULL,"<DEV_NUM>", "start device number");
    arming_args.count = arg_str0(NULL, NULL,  "<DEV_CNT>", "device count");
    arming_args.end = arg_end(2);

    /* keypad 0 1 1 1 2 1 0 0 */
    keypad_args.no = arg_str1(NULL, NULL,"<DEV_NUM>", "device number");
    keypad_args.tone_play = arg_str1(NULL, NULL,"<TONE_PLAY>", "0:none, 1:intrusion, 2:fire, 3:energy, 4:medical");
    keypad_args.err_led = arg_str1(NULL, NULL,"<ERR_LED>", "0:off, 1:on");
    keypad_args.arm_led = arg_str1(NULL, NULL,"<ARM_LED>", "0:off, 1:on");
    keypad_args.warn_led = arg_str1(NULL, NULL,"<WARN_LED>", "0:off, 1:on, 2:blink");
    keypad_args.key_tone_enable = arg_str1(NULL, NULL,"<KEY_TONE_ENABLE>", "0:disable, 1:enable");
    keypad_args.backlight_time = arg_str1(NULL, NULL,"<BACKLIGHT_TIME>", "0:10s, 1:15s, 2:20s, 3:25s");
    keypad_args.lock = arg_str1(NULL, NULL,"<LOCK>", "0:unlock, 1:lock");
    keypad_args.end = arg_end(8);

    relay_args.no = arg_str1(NULL, NULL,"<DEV_NUM>", "device number");
    relay_args.action = arg_str1(NULL, NULL,"<ACTION>", "0:off, 1:on, 2: toogle");
    relay_args.end = arg_end(2);

    smartplug_args.no = arg_str1(NULL, NULL,"<DEV_NUM>", "device number");
    smartplug_args.action = arg_str1(NULL, NULL,"<ACTION>", "0:off, 1:on, 2: toogle");
    smartplug_args.lock = arg_str1(NULL, NULL,"<LOCK>", "0:off, 1:on");
    smartplug_args.end = arg_end(3);

    ota_args.start = arg_lit0(NULL, "start",  "start ota upgrade");
    ota_args.end = arg_end(1);

    for (int i=0; i<sizeof(cmds)/sizeof(esp_console_cmd_t); i++)
    {
        ESP_ERROR_CHECK(esp_console_cmd_register(&cmds[i]));
    }

    ESP_ERROR_CHECK(esp_console_start_repl(repl));
    return ESP_OK;
}

esp_err_t app_rbf_port_config(void)
{
    RBF_port_t rbf_port = {0};

    if (uart_driver_install(RBF_UART_PORT, 2 * 1024, 0, 0, NULL, 0) != ESP_OK) {
        ESP_LOGE(TAG, "Driver installation failed");
        return ESP_FAIL;
    }

    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    ESP_ERROR_CHECK(uart_param_config(RBF_UART_PORT, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(RBF_UART_PORT, 4, 5, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
     // We have a driver now installed so set up the read/write functions to use driver also.
    rbf_port.read = &rbf_port_read;
    rbf_port.write = &rbf_port_write;
#if defined(CONFIG_RPC_ENABLE) && (CONFIG_RPC_ENABLE == 1)
    rbf_port.reset = &rbf_port_reset;
#endif
    rbf_set_port(&rbf_port);
    return ESP_OK;
}

#if defined(CONFIG_RPC_ENABLE) && (CONFIG_RPC_ENABLE == 1)

esp_err_t app_rpc_port_config(void)
{
    if (uart_driver_install(RPC_UART_PORT, 2 * 1024, 0, 0, NULL, 0) != ESP_OK) {
        ESP_LOGE(TAG, "Driver installation failed");
        return ESP_FAIL;
    }

    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    ESP_ERROR_CHECK(uart_param_config(RPC_UART_PORT, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(RPC_UART_PORT, 17, 18, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    return ESP_OK;
}

#endif

/**
 * @brief This is a freeRTOS thread entry point that starts our test routine.
 * 
 */
void app_main(void)
{
    printf("RBF Test!\n");

#if defined(CONFIG_RPC_ENABLE) && (CONFIG_RPC_ENABLE == 1)
    app_rpc_port_config();
    rpc_helper.read = &rpc_read;
    rpc_helper.write = &rpc_write;
    rpc_init(&rpc_helper);
#endif
    /* Configure the RBF serial port. */
    app_rbf_port_config();
    /* Initialize the test console. */
    app_console_init();
    /* Print chip information */
    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
           (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
           (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);
    if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        printf("Get flash size failed");
        return;
    }

    printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());
}
