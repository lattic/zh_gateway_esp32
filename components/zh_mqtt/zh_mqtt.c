#include "zh_mqtt.h"

bool mqtt_is_enable = false;
bool mqtt_is_connected = false;

static TaskHandle_t mqtt_json_attributes_message_task;
static TaskHandle_t mqtt_json_keep_alive_message_task;
static TaskHandle_t espnow_online_message_task;

esp_mqtt_client_handle_t client;

extern char *mqtt_broker_url;
extern char *mqtt_topic_prefix;

void zh_mqtt_init()
{
    esp_err_t err;
    esp_mqtt_client_config_t mqtt_config = {
        .broker.address.uri = mqtt_broker_url,
        .task.priority = MQTT_TASK_PRIORITY,
        .task.stack_size = MQTT_STACK_SIZE,
    };
    client = esp_mqtt_client_init(&mqtt_config);
    err = esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, zh_mqtt_event_handler, NULL);
    while (err == ESP_ERR_NO_MEM)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        err = esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, zh_mqtt_event_handler, NULL);
    }
    esp_mqtt_client_start(client);
    mqtt_is_enable = true;
}

void zh_mqtt_deinit()
{
    esp_mqtt_client_stop(client);
    mqtt_is_enable = false;
}

void zh_mqtt_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    switch (event_id)
    {
    case MQTT_EVENT_CONNECTED:
        char *topic_for_subscribe = NULL;
        char *supported_device_type = NULL;
        mqtt_is_connected = true;
        for (zh_device_type_t i = 1; i <= ZHDT_MAX; ++i)
        {
            supported_device_type = get_device_type_value_name(i);
            topic_for_subscribe = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(supported_device_type) + 4);
            while (topic_for_subscribe == NULL)
            {
                vTaskDelay(1 / portTICK_PERIOD_MS);
                topic_for_subscribe = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(supported_device_type) + 4);
            }
            sprintf(topic_for_subscribe, "%s/%s/#", mqtt_topic_prefix, supported_device_type);
            esp_mqtt_client_subscribe(client, topic_for_subscribe, 2);
            free(topic_for_subscribe);
        }
        xTaskCreatePinnedToCore(&zh_send_mqtt_json_config_message_task, "Send config message", JSON_STACK_SIZE, event->client, JSON_TASK_PRIORITY, NULL, tskNO_AFFINITY);
        xTaskCreatePinnedToCore(&zh_send_mqtt_json_attributes_message_task, "Send attributes message", JSON_STACK_SIZE, event->client, JSON_TASK_PRIORITY, &mqtt_json_attributes_message_task, tskNO_AFFINITY);
        xTaskCreatePinnedToCore(&zh_send_mqtt_json_keep_alive_message_task, "Send keep alive message", JSON_STACK_SIZE, event->client, JSON_TASK_PRIORITY, &mqtt_json_keep_alive_message_task, tskNO_AFFINITY);
        xTaskCreatePinnedToCore(&zh_send_espnow_online_message_task, "Send online message", JSON_STACK_SIZE, NULL, JSON_TASK_PRIORITY, &espnow_online_message_task, tskNO_AFFINITY);
        break;
    case MQTT_EVENT_DISCONNECTED:
        if (mqtt_is_connected == true)
        {
            vTaskDelete(mqtt_json_attributes_message_task);
            vTaskDelete(mqtt_json_keep_alive_message_task);
            vTaskDelete(espnow_online_message_task);
            xTaskCreatePinnedToCore(&zh_send_espnow_offline_message_task, "Send offline message", JSON_STACK_SIZE, NULL, JSON_TASK_PRIORITY, NULL, tskNO_AFFINITY);
        }
        mqtt_is_connected = false;
        break;
    case MQTT_EVENT_DATA:
        char incoming_topic[(event->topic_len) + 1];
        memset(incoming_topic, 0, sizeof(incoming_topic));
        strncat(incoming_topic, event->topic, event->topic_len);
        uint8_t incoming_data_mac[6];
        zh_device_type_t incoming_data_device_type = ZHDT_NONE;
        zh_payload_type_t incoming_data_payload_type = ZHPT_NONE;
        char *extracted_topic_data = strtok(incoming_topic, "/"); // Extract topic prefix.
        extracted_topic_data = strtok(NULL, "/");                 // Extract device type.
        if (extracted_topic_data == NULL)
        {
            break;
        }
        for (zh_device_type_t i = 1; i < ZHDT_MAX; ++i)
        {
            if (strncmp(extracted_topic_data, get_device_type_value_name(i), strlen(extracted_topic_data) + 1) == 0)
            {
                incoming_data_device_type = i;
                break;
            }
        }
        extracted_topic_data = strtok(NULL, "/"); // Extract MAC address.
        if (extracted_topic_data == NULL)
        {
            break;
        }
        sscanf(extracted_topic_data, "%hhX-%hhX-%hhX-%hhX-%hhX-%hhX", &incoming_data_mac[0], &incoming_data_mac[1], &incoming_data_mac[2], &incoming_data_mac[3], &incoming_data_mac[4], &incoming_data_mac[5]);
        extracted_topic_data = strtok(NULL, "/"); // Extract payload type.
        if (extracted_topic_data != NULL)
        {
            for (zh_payload_type_t i = 1; i < ZHPT_MAX; ++i)
            {
                if (strncmp(extracted_topic_data, get_payload_type_value_name(i), strlen(extracted_topic_data) + 1) == 0)
                {
                    incoming_data_payload_type = i;
                    break;
                }
            }
        }
        char incoming_payload[(event->data_len) + 1];
        memset(incoming_payload, 0, sizeof(incoming_payload));
        strncat(incoming_payload, event->data, event->data_len);
        switch (incoming_data_device_type)
        {
        case ZHDT_GATEWAY:
            uint8_t self_mac[6];
            esp_read_mac(self_mac, ESP_MAC_WIFI_SOFTAP);
            if (memcmp(self_mac, incoming_data_mac, 6) == 0)
            {
                if (strncmp(incoming_payload, "update", strlen(incoming_payload) + 1) == 0)
                {
                    xTaskCreatePinnedToCore(&zh_self_ota_update_task, "Self OTA update", OTA_STACK_SIZE, NULL, OTA_TASK_PRIORITY, NULL, tskNO_AFFINITY);
                }
                else if (strncmp(incoming_payload, "restart", strlen(incoming_payload) + 1) == 0)
                {
                    esp_restart();
                }
                else
                {
                    break;
                }
            }
            break;
        case ZHDT_SWITCH:
            switch (incoming_data_payload_type)
            {
            case ZHPT_SET:
                for (ha_on_off_type_t i = 1; i < HAONOFT_MAX; ++i)
                {
                    if (strncmp(incoming_payload, get_on_off_type_value_name(i), strlen(incoming_payload) + 1) == 0)
                    {
                        zh_send_espnow_switch_setup_message(i, incoming_data_mac);
                        break;
                    }
                }
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        break;
    }
}