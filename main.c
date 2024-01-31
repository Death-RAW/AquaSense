/*
  This project is implemented as a partial requirement 
  of the IOT course of the 
  Masters in Computer Science and Engineering.

 */

/**
 * @ingroup     Sensing_Layer
 * @{
 *
 * @file
 * @brief      Sensor reading are observed to monitor the quatic conditions of a aquarium
 *
 * @author      Add names here
 * 
 *
 * @}
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include "timex.h"
#include "thread.h"
#include "mutex.h"
#include "ztimer.h"
#include "shell.h"
#include "msg.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "periph/wdt.h"

#ifndef EMCUTE_ID
#define EMCUTE_ID ("Aqua_Sense")
#endif

#define EMCUTE_PORT (1884U)
#define EMCUTE_PRIO (THREAD_PRIORITY_MAIN - 1)

// lpsxxx senosr headers
#include "lpsxxx.h"
#include "lpsxxx_params.h"

// isl29020 rsenosr headers
#include "isl29020.h"
#include "isl29020_params.h"
#include "isl29020-internal.h"

// Declare the LPS331AP device variable
static lpsxxx_t lpsxxx;

// Declare the ISL29020 device variable
static isl29020_t isl29020;

static int16_t temp;
static int lux;

// Struct that contains sensor values
typedef struct sensors {
    int16_t temperature;
    int ambient_light;
} t_sensors;

static char stack[THREAD_STACKSIZE_DEFAULT];

// Thread stacks and handler functions
static char lps331ap_stack_T[THREAD_STACKSIZE_MAIN];
static char isl29020_stack[THREAD_STACKSIZE_MAIN];

static msg_t queue[8];


// Function declarations
static void *emcute_thread(void *arg);
static int disconnect(void);
static void *thread_handler_temp(void *arg);
static void *thread_handler_ambient_light(void *arg);
static int publisher(char *topic, char *data, int qos);
static int con(char *addr, int port);
static void sensors_values(t_sensors *sensors);
static int aqua_sense_start(int argc, char **argv);


// Function definitions...

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL; /* should never be reached */
}

static int con(char *addr, int port) {
    sock_udp_ep_t gw = {.family = AF_INET6, .port = EMCUTE_PORT};
    gw.port = port;

    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, addr) == NULL) {
        printf("error parsing IPv6 address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%i\n", addr, port);
        
        return 1;
    }
    printf("Successfully connected to gateway at [%s]:%i\n", addr, port);
    return 0;
}

static int disconnect(void) {
    int res = emcute_discon();
    if (res == EMCUTE_NOGW) {
        puts("error: No active broker found");
        return 1;
    } else if (res != EMCUTE_OK) {
        puts("error: broker disconnection failed");
        return 1;
    }
    puts("Broker Disconnected");
    return 0;
}

static void *thread_handler_temp(void *arg) {
    (void)arg;
    //  constant for sleep duration in milliseconds
    const uint32_t SLEEP_DURATION_MS = 3000;
    while (1) {
        if (lpsxxx_read_temp(&lpsxxx, &temp) != LPSXXX_OK) {
            fprintf(stderr, "Error reading temperature\n");
            break;
        }
        // Sleep for SLEEP_DURATION_MS
        ztimer_sleep(ZTIMER_MSEC, SLEEP_DURATION_MS);
    }
    return NULL;
}

static void *thread_handler_ambient_light(void *arg) {
    (void)arg;
    isl29020_enable(&isl29020);
    if (isl29020_init(&isl29020, &isl29020_params[0]) == 0) {
        while (1) {
            if (isl29020_read(&isl29020) != 0) {
                lux = isl29020_read(&isl29020);
            }
            ztimer_sleep(ZTIMER_MSEC, 5000);
        }
    } else {
        printf("[Failed]");
    }
    return NULL;
}

static void sensors_values(t_sensors *sensors) {
    
    sensors->temperature = (temp/100);
    sensors->ambient_light = lux;
}

static int publisher(char *topic, char *data, int qos) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    switch (qos) {
        case 1:
            flags |= EMCUTE_QOS_1;
            break;
        case 2:
            flags |= EMCUTE_QOS_2;
            break;
        default:
            flags |= EMCUTE_QOS_0;
            break;
    }

    t.name = topic;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to get topic ID");
        return 1;
    }

    if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
        printf("error: unable to publish data to topic '%s [%i]'\n", t.name, (int)t.id);
        return 1;
    }

    printf("published %s on topic %s\n", data, topic);
    return 0;
}

static const shell_command_t shell_commands[] = {
    {"Run", "Start Monitoring the aquarium", aqua_sense_start},
    {NULL, NULL, NULL}
};

static int aqua_sense_start(int argc, char **argv) {
    if (argc < 4) {
        printf("usage: %s <address> <port> <id>\n", argv[0]);
        return 1;
    }

    t_sensors sensors;
    char topic[32];
    char json[256];

    sprintf(topic,"sensor/aquarium%d", atoi(argv[3]));    

    while (1) {
        if (con(argv[1], atoi(argv[2]))) {
            continue;
        }
        sensors_values(&sensors);
        sprintf(json, "{\"id\": \"%d\", \"temperature\": "
                      "\"%d\", \"ambient_light\": \"%d\"}",
                atoi(argv[3]), sensors.temperature, sensors.ambient_light);
        publisher(topic, json, 0);
        disconnect();
        ztimer_sleep(ZTIMER_SEC, 10);
    }
    return 0;
}


int main(void) {
    lpsxxx_init(&lpsxxx, &lpsxxx_params[0]);
    isl29020_init(&isl29020, &isl29020_params[0]);
    // The main thread needs a message queue for running `ping6`
    msg_init_queue(queue, ARRAY_SIZE(queue));
    // Start the emcute thread
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");
    // Create threads for LPS331AP and ISL29020
    thread_create(lps331ap_stack_T, sizeof(lps331ap_stack_T), THREAD_PRIORITY_MAIN - 1, 0,
                  thread_handler_temp, NULL, "thread_temp");
    thread_create(isl29020_stack, sizeof(isl29020_stack), THREAD_PRIORITY_MAIN - 3, 0,
                  thread_handler_ambient_light, NULL, "thread_ambient");
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}
