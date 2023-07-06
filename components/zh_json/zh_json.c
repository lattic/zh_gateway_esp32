#include "zh_json.h"

extern char *mqtt_topic_prefix;

void zh_send_mqtt_json_attributes_message_task(void *pvParameter)
{
    esp_mqtt_client_handle_t client = pvParameter;
    uint8_t self_mac[6];
    esp_read_mac(self_mac, ESP_MAC_WIFI_SOFTAP);
    char *device_type = get_device_type_value_name(ZHDT_GATEWAY);
    char *chip_type = get_chip_type_value_name(HACHT_ESP32);
    const esp_app_desc_t *app_info = esp_app_get_description();
    uint32_t secs;
    uint32_t mins;
    uint32_t hours;
    uint32_t days;
    for (;;)
    {
        char *type = (char *)malloc(strlen(device_type) + 10 + 1);
        while (type == NULL)
        {
            vTaskDelay(1 / portTICK_PERIOD_MS);
            type = (char *)malloc(strlen(device_type) + 10 + 1);
        }
        sprintf(type, "\"Type\": \"%s\"", device_type);
        char *mac = (char *)malloc(21 + 1);
        while (mac == NULL)
        {
            vTaskDelay(1 / portTICK_PERIOD_MS);
            mac = (char *)malloc(21 + 1);
        }
        sprintf(mac, "\"MAC\": \"" MAC_STR "\"", MAC2STR(self_mac));
        char *chip = (char *)malloc(strlen(chip_type) + 10 + 1);
        while (chip == NULL)
        {
            vTaskDelay(1 / portTICK_PERIOD_MS);
            chip = (char *)malloc(strlen(chip_type) + 10 + 1);
        }
        sprintf(chip, "\"Chip\": \"%s\"", chip_type);
        char *app_name = (char *)malloc(strlen(app_info->project_name) + 14 + 1);
        while (app_name == NULL)
        {
            vTaskDelay(1 / portTICK_PERIOD_MS);
            app_name = (char *)malloc(strlen(app_info->project_name) + 14 + 1);
        }
        sprintf(app_name, "\"App name\": \"%s\"", app_info->project_name);
        char *app_ver = (char *)malloc(strlen(app_info->version) + 17 + 1);
        while (app_ver == NULL)
        {
            vTaskDelay(1 / portTICK_PERIOD_MS);
            app_ver = (char *)malloc(strlen(app_info->version) + 17 + 1);
        }
        sprintf(app_ver, "\"App version\": \"%s\"", app_info->version);
        secs = esp_timer_get_time() / 1000000;
        mins = secs / 60;
        hours = mins / 60;
        days = hours / 24;
        char *uptime = (char *)malloc(56 + 1);
        while (uptime == NULL)
        {
            vTaskDelay(1 / portTICK_PERIOD_MS);
            uptime = (char *)malloc(56 + 1);
        }
        sprintf(uptime, "\"Uptime\": \"Days:%lu Hours:%lu Mins:%lu\"", days, hours - (days * 24), mins - (hours * 60));
        char *topic = (char *)malloc(strlen(mqtt_topic_prefix) + strlen(device_type) + 25 + 1);
        while (topic == NULL)
        {
            vTaskDelay(1 / portTICK_PERIOD_MS);
            topic = (char *)malloc(strlen(mqtt_topic_prefix) + strlen(device_type) + 25 + 1);
        }
        sprintf(topic, "%s/%s/" MAC_STR "/attributes", mqtt_topic_prefix, device_type, MAC2STR(self_mac));
        char *data = (char *)malloc(strlen(type) + strlen(mac) + strlen(chip) + strlen(app_ver) + strlen(app_name) + strlen(uptime) + 14 + 1);
        while (data == NULL)
        {
            vTaskDelay(1 / portTICK_PERIOD_MS);
            data = (char *)malloc(strlen(type) + strlen(mac) + strlen(chip) + strlen(app_ver) + strlen(app_name) + strlen(uptime) + 14 + 1);
        }
        sprintf(data, "{ %s, %s, %s, %s, %s, %s }", type, mac, chip, app_name, app_ver, uptime);
        esp_mqtt_client_publish(client, topic, data, 0, 2, true);
        free(mac);
        free(chip);
        free(app_name);
        free(app_ver);
        free(uptime);
        free(topic);
        free(data);
        vTaskDelay(60000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void zh_send_mqtt_json_config_message_task(void *pvParameter)
{
    esp_mqtt_client_handle_t client = pvParameter;
    uint8_t self_mac[6];
    esp_read_mac(self_mac, ESP_MAC_WIFI_SOFTAP);
    char *device_type = get_device_type_value_name(ZHDT_GATEWAY);
    char *platform = "\"platform\": \"mqtt\"";
    char *name = (char *)malloc(strlen(device_type) + 23 + 1);
    while (name == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        name = (char *)malloc(strlen(device_type) + 23 + 1);
    }
    sprintf(name, "\"name\": \"%s " MAC_STR "\"", device_type, MAC2STR(self_mac));
    char *unique_id = (char *)malloc(29 + 1);
    while (unique_id == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        unique_id = (char *)malloc(29 + 1);
    }
    sprintf(unique_id, "\"unique_id\": \"" MAC_STR "-1\"", MAC2STR(self_mac));
    char *device_class = "\"device_class\": \"connectivity\"";
    char *state_topic = (char *)malloc(strlen(mqtt_topic_prefix) + strlen(device_type) + 38 + 1);
    while (state_topic == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        state_topic = (char *)malloc(strlen(mqtt_topic_prefix) + strlen(device_type) + 38 + 1);
    }
    sprintf(state_topic, "\"state_topic\": \"%s/%s/" MAC_STR "/status\"", mqtt_topic_prefix, device_type, MAC2STR(self_mac));
    char *json_attributes_topic = (char *)malloc(strlen(mqtt_topic_prefix) + strlen(device_type) + 52 + 1);
    while (json_attributes_topic == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        json_attributes_topic = (char *)malloc(strlen(mqtt_topic_prefix) + strlen(device_type) + 52 + 1);
    }
    sprintf(json_attributes_topic, "\"json_attributes_topic\": \"%s/%s/" MAC_STR "/attributes\"", mqtt_topic_prefix, device_type, MAC2STR(self_mac));
    char *payload_on = "\"payload_on\": \"online\"";
    char *expire_after = "\"expire_after\": 30";
    char *force_update = "\"force_update\": \"true\"";
    char *qos = "\"qos\": 2";
    char *topic = (char *)malloc(strlen(mqtt_topic_prefix) + 36 + 1);
    while (topic == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        topic = (char *)malloc(strlen(mqtt_topic_prefix) + 36 + 1);
    }
    sprintf(topic, "%s/binary_sensor/" MAC_STR "-1/config", mqtt_topic_prefix, MAC2STR(self_mac));
    char *data = (char *)malloc(strlen(platform) + strlen(name) + strlen(unique_id) + strlen(device_class) + strlen(state_topic) +
                                strlen(json_attributes_topic) + strlen(payload_on) + strlen(expire_after) + strlen(force_update) + strlen(qos) + 22 + 1);
    while (data == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        data = (char *)malloc(strlen(platform) + strlen(name) + strlen(unique_id) + strlen(device_class) + strlen(state_topic) +
                              strlen(json_attributes_topic) + strlen(payload_on) + strlen(expire_after) + strlen(force_update) + strlen(qos) + 22 + 1);
    }
    sprintf(data, "{ %s, %s, %s, %s, %s, %s, %s, %s, %s, %s }", platform, name, unique_id, device_class, state_topic, json_attributes_topic,
            payload_on, expire_after, force_update, qos);
    esp_mqtt_client_publish(client, topic, data, 0, 2, true);
    free(name);
    free(unique_id);
    free(state_topic);
    free(json_attributes_topic);
    free(topic);
    free(data);
    vTaskDelete(NULL);
}

void zh_send_mqtt_json_keep_alive_message_task(void *pvParameter)
{
    esp_mqtt_client_handle_t client = pvParameter;
    uint8_t self_mac[6];
    esp_read_mac(self_mac, ESP_MAC_WIFI_SOFTAP);
    char *device_type = get_device_type_value_name(ZHDT_GATEWAY);
    char *status = "online";
    for (;;)
    {
        char *topic = (char *)malloc(strlen(mqtt_topic_prefix) + strlen(device_type) + 21 + 1);
        while (topic == NULL)
        {
            vTaskDelay(1 / portTICK_PERIOD_MS);
            topic = (char *)malloc(strlen(mqtt_topic_prefix) + strlen(device_type) + 21 + 1);
        }
        sprintf(topic, "%s/%s/" MAC_STR "/status", mqtt_topic_prefix, device_type, MAC2STR(self_mac));
        esp_mqtt_client_publish(client, topic, status, 0, 2, true);
        free(topic);
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void zh_send_espnow_online_message_task(void *pvParameter)
{
    uint8_t broadcast_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    zh_keep_alive_message_t keep_alive_message;
    keep_alive_message.online_status = ONLINE;
    keep_alive_message.message_frequency = 1;
    zh_espnow_data_t data;
    strcpy(data.net_name, ESPNOW_NET_NAME);
    data.device_type = ZHDT_GATEWAY;
    data.payload_type = ZHPT_KEEP_ALIVE;
    data.payload_data = (zh_payload_data_t)keep_alive_message;
    for (;;)
    {
        zh_espnow_send(broadcast_mac, (uint8_t *)&data, sizeof(zh_espnow_data_t));
        vTaskDelay(keep_alive_message.message_frequency * 1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void zh_send_espnow_offline_message_task(void *pvParameter)
{
    uint8_t broadcast_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    zh_keep_alive_message_t keep_alive_message;
    keep_alive_message.online_status = OFFLINE;
    zh_espnow_data_t data;
    strcpy(data.net_name, ESPNOW_NET_NAME);
    data.device_type = ZHDT_GATEWAY;
    data.payload_type = ZHPT_KEEP_ALIVE;
    data.payload_data = (zh_payload_data_t)keep_alive_message;
    zh_espnow_send(broadcast_mac, (uint8_t *)&data, sizeof(zh_espnow_data_t));
    vTaskDelete(NULL);
}

void zh_espnow_switch_send_mqtt_json_attributes_message(zh_espnow_data_t device_data, uint8_t device_mac[6])
{
    extern esp_mqtt_client_handle_t client;
    char *device_type = get_device_type_value_name(device_data.device_type);
    char *chip_type = get_chip_type_value_name(device_data.payload_data.attributes_message.chip_type);
    uint32_t secs;
    uint32_t mins;
    uint32_t hours;
    uint32_t days;
    char *type = (char *)malloc(strlen(device_type) + 10 + 1);
    while (type == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        type = (char *)malloc(strlen(device_type) + 10 + 1);
    }
    sprintf(type, "\"Type\": \"%s\"", device_type);
    char *mac = (char *)malloc(21 + 1);
    while (mac == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        mac = (char *)malloc(21 + 1);
    }
    sprintf(mac, "\"MAC\": \"" MAC_STR "\"", MAC2STR(device_mac));
    char *chip = (char *)malloc(strlen(chip_type) + 10 + 1);
    while (chip == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        chip = (char *)malloc(strlen(chip_type) + 10 + 1);
    }
    sprintf(chip, "\"Chip\": \"%s\"", chip_type);
    char *app_name = (char *)malloc(strlen(device_data.payload_data.attributes_message.app_name) + 14 + 1);
    while (app_name == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        app_name = (char *)malloc(strlen(device_data.payload_data.attributes_message.app_name) + 14 + 1);
    }
    sprintf(app_name, "\"App name\": \"%s\"", device_data.payload_data.attributes_message.app_name);
    char *app_ver = (char *)malloc(strlen(device_data.payload_data.attributes_message.app_version) + 17 + 1);
    while (app_ver == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        app_ver = (char *)malloc(strlen(device_data.payload_data.attributes_message.app_version) + 17 + 1);
    }
    sprintf(app_ver, "\"App version\": \"%s\"", device_data.payload_data.attributes_message.app_version);
    secs = device_data.payload_data.attributes_message.uptime;
    mins = secs / 60;
    hours = mins / 60;
    days = hours / 24;
    char *uptime = (char *)malloc(56 + 1);
    while (uptime == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        uptime = (char *)malloc(56 + 1);
    }
    sprintf(uptime, "\"Uptime\": \"Days:%lu Hours:%lu Mins:%lu\"", days, hours - (days * 24), mins - (hours * 60));
    char *topic = (char *)malloc(strlen(mqtt_topic_prefix) + strlen(device_type) + 25 + 1);
    while (topic == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        topic = (char *)malloc(strlen(mqtt_topic_prefix) + strlen(device_type) + 25 + 1);
    }
    sprintf(topic, "%s/%s/" MAC_STR "/attributes", mqtt_topic_prefix, device_type, MAC2STR(device_mac));
    char *data = (char *)malloc(strlen(type) + strlen(mac) + strlen(chip) + strlen(app_ver) + strlen(app_name) + strlen(uptime) + 14 + 1);
    while (data == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        data = (char *)malloc(strlen(type) + strlen(mac) + strlen(chip) + strlen(app_ver) + strlen(app_name) + strlen(uptime) + 14 + 1);
    }
    sprintf(data, "{ %s, %s, %s, %s, %s, %s }", type, mac, chip, app_name, app_ver, uptime);
    esp_mqtt_client_publish(client, topic, data, 0, 2, true);
    free(mac);
    free(chip);
    free(app_name);
    free(app_ver);
    free(uptime);
    free(topic);
    free(data);
    vTaskDelete(NULL);
}

void zh_espnow_switch_send_mqtt_json_config_message(zh_espnow_data_t device_data, uint8_t device_mac[6])
{
}

void zh_espnow_switch_send_mqtt_json_keep_alive_message(zh_espnow_data_t device_data, uint8_t device_mac[6])
{
    extern esp_mqtt_client_handle_t client;
    char *device_type = get_device_type_value_name(device_data.device_type);
    char *status = NULL;
    if (device_data.payload_data.keep_alive_message.online_status == ONLINE)
    {
        status = "online";
    }
    else if (device_data.payload_data.keep_alive_message.online_status == OFFLINE)
    {
        status = "offline";
    }
    char *topic = (char *)malloc(strlen(mqtt_topic_prefix) + strlen(device_type) + 21 + 1);
    while (topic == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        topic = (char *)malloc(strlen(mqtt_topic_prefix) + strlen(device_type) + 21 + 1);
    }
    sprintf(topic, "%s/%s/" MAC_STR "/status", mqtt_topic_prefix, device_type, MAC2STR(device_mac));
    esp_mqtt_client_publish(client, topic, status, 0, 2, true);
    free(topic);
}