#ifndef ZH_JSON_H
#define ZH_JSON_H

#include "esp_timer.h"
#include "esp_app_desc.h"
#include "mqtt_client.h"
#include "esp_mac.h"
#include "zh_espnow.h"
#include "zh_config.h"

#define JSON_TASK_PRIORITY 2
#define JSON_STACK_SIZE 2048

#define MAC_STR "%02X%02X%02X%02X%02X%02X"

#ifdef __cplusplus
extern "C"
{
#endif

    void zh_send_mqtt_json_attributes_message_task(void *pvParameter);
    void zh_send_mqtt_json_config_message_task(void *pvParameter);
    void zh_send_mqtt_json_keep_alive_message_task(void *pvParameter);
    void zh_send_espnow_online_message_task(void *pvParameter);
    void zh_send_espnow_offline_message_task(void *pvParameter);

    void zh_espnow_switch_send_mqtt_json_attributes_message(zh_espnow_data_t data, uint8_t mac[6]);
    void zh_espnow_switch_send_mqtt_json_config_message(zh_espnow_data_t data, uint8_t mac[6]);
    void zh_espnow_switch_send_mqtt_json_keep_alive_message(zh_espnow_data_t data, uint8_t mac[6]);

#ifdef __cplusplus
}
#endif

#endif