#ifndef ZH_ETH_H
#define ZH_ETH_H

#include "esp_err.h"
#include "driver/gpio.h"
#include "esp_netif.h"
#include "esp_eth.h"
#include "zh_sntp.h"
#include "zh_mqtt.h"
#include "zh_ota.h"
#include "zh_config.h"

#define LAN_POWER_PIN GPIO_NUM_16

#ifdef __cplusplus
extern "C"
{
#endif

    void zh_eth_init(void);
    void zh_eth_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

#ifdef __cplusplus
}
#endif

#endif