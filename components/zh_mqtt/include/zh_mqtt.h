#ifndef ZH_MQTT_H
#define ZH_MQTT_H

#include "esp_err.h"
#include "mqtt_client.h"
#include "zh_json.h"
#include "zh_config.h"

#define MQTT_TASK_PRIORITY 3
#define MQTT_STACK_SIZE 6144

#ifdef __cplusplus
extern "C"
{
#endif

    void zh_mqtt_init(void);
    void zh_mqtt_deinit(void);
    void zh_mqtt_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

#ifdef __cplusplus
}
#endif

#endif