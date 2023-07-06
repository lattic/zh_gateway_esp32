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
        char *topic = NULL;
        char *device_type = NULL;
        mqtt_is_connected = true;
        for (zh_device_type_t i = 1; i < ZHDT_MAX; ++i)
        {
            device_type = get_device_type_value_name(i);
            topic = (char *)malloc(strlen(mqtt_topic_prefix) + strlen(device_type) + 4);
            while (topic == NULL)
            {
                vTaskDelay(1 / portTICK_PERIOD_MS);
                topic = (char *)malloc(strlen(mqtt_topic_prefix) + strlen(device_type) + 4);
            }
            sprintf(topic, "%s/%s/#", mqtt_topic_prefix, device_type);
            esp_mqtt_client_subscribe(client, topic, 2);
            free(topic);
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
        // printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        // printf("DATA=%.*s\r\n", event->data_len, event->data);
        break;
    default:
        break;
    }
}