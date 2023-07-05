#ifndef ZH_WIFI_H
#define ZH_WIFI_H

#include "esp_wifi.h"
#include "esp_timer.h"
#include "zh_sntp.h"
#include "zh_mqtt.h"
#include "zh_ota.h"
#include "zh_espnow.h"
#include "zh_config.h"

#define WIFI_MAXIMUM_RETRY 5
#define WIFI_RECONNECT_TIME 5 // In sec.

#ifdef __cplusplus
extern "C"
{
#endif

    void zh_wifi_init_sta(void);
    void zh_wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

#ifdef __cplusplus
}
#endif

#endif