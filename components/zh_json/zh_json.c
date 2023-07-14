#include "zh_json.h"

extern char *mqtt_topic_prefix;

// **** Attributes message from gateway to MQTT broker ***
// Message view as:
// {
//   "Type": "gateway",
//   "MAC": "0C-B8-15-26-E0-79",
//   "Chip": "ESP32",
//   "App name": "zh_gateway_esp32",
//   "App version": "0.0.4",
//   "Uptime": "Days:0 Hours:0 Mins:0"
// }
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
        char *type = (char *)calloc(1, strlen(device_type) + 10 + 1);
        while (type == NULL)
        {
            vTaskDelay(1 / portTICK_PERIOD_MS);
            type = (char *)calloc(1, strlen(device_type) + 10 + 1);
        }
        sprintf(type, "\"Type\": \"%s\"", device_type);
        char *mac = (char *)calloc(1, 26 + 1);
        while (mac == NULL)
        {
            vTaskDelay(1 / portTICK_PERIOD_MS);
            mac = (char *)calloc(1, 26 + 1);
        }
        sprintf(mac, "\"MAC\": \"" MAC_STR "\"", MAC2STR(self_mac));
        char *chip = (char *)calloc(1, strlen(chip_type) + 10 + 1);
        while (chip == NULL)
        {
            vTaskDelay(1 / portTICK_PERIOD_MS);
            chip = (char *)calloc(1, strlen(chip_type) + 10 + 1);
        }
        sprintf(chip, "\"Chip\": \"%s\"", chip_type);
        char *app_name = (char *)calloc(1, strlen(app_info->project_name) + 14 + 1);
        while (app_name == NULL)
        {
            vTaskDelay(1 / portTICK_PERIOD_MS);
            app_name = (char *)calloc(1, strlen(app_info->project_name) + 14 + 1);
        }
        sprintf(app_name, "\"App name\": \"%s\"", app_info->project_name);
        char *app_ver = (char *)calloc(1, strlen(app_info->version) + 17 + 1);
        while (app_ver == NULL)
        {
            vTaskDelay(1 / portTICK_PERIOD_MS);
            app_ver = (char *)calloc(1, strlen(app_info->version) + 17 + 1);
        }
        sprintf(app_ver, "\"App version\": \"%s\"", app_info->version);
        secs = esp_timer_get_time() / 1000000;
        mins = secs / 60;
        hours = mins / 60;
        days = hours / 24;
        char *uptime = (char *)calloc(1, 56 + 1);
        while (uptime == NULL)
        {
            vTaskDelay(1 / portTICK_PERIOD_MS);
            uptime = (char *)calloc(1, 56 + 1);
        }
        sprintf(uptime, "\"Uptime\": \"Days:%lu Hours:%lu Mins:%lu\"", days, hours - (days * 24), mins - (hours * 60));
        char *topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(device_type) + 30 + 1);
        while (topic == NULL)
        {
            vTaskDelay(1 / portTICK_PERIOD_MS);
            topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(device_type) + 30 + 1);
        }
        sprintf(topic, "%s/%s/" MAC_STR "/attributes", mqtt_topic_prefix, device_type, MAC2STR(self_mac));
        char *data = (char *)calloc(1, strlen(type) + strlen(mac) + strlen(chip) + strlen(app_ver) + strlen(app_name) + strlen(uptime) + 14 + 1);
        while (data == NULL)
        {
            vTaskDelay(1 / portTICK_PERIOD_MS);
            data = (char *)calloc(1, strlen(type) + strlen(mac) + strlen(chip) + strlen(app_ver) + strlen(app_name) + strlen(uptime) + 14 + 1);
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

// **** Config message from gateway to MQTT broker ***
// Message view as:
// {
//   "platform": "mqtt",
//   "name": "gateway 0C-B8-15-26-E0-79",
//   "unique_id": "0C-B8-15-26-E0-79-1",
//   "device_class": "connectivity",
//   "state_topic": "zh-home/gateway/0C-B8-15-26-E0-79/status",
//   "json_attributes_topic": "zh-home/gateway/0C-B8-15-26-E0-79/attributes",
//   "payload_on": "online",
//   "expire_after": 30,
//   "force_update": "true",
//   "qos": 2
//   }
void zh_send_mqtt_json_config_message_task(void *pvParameter)
{
    esp_mqtt_client_handle_t client = pvParameter;
    uint8_t self_mac[6];
    esp_read_mac(self_mac, ESP_MAC_WIFI_SOFTAP);
    char *device_type = get_device_type_value_name(ZHDT_GATEWAY);
    char *platform = "\"platform\": \"mqtt\"";
    char *name = (char *)calloc(1, strlen(device_type) + 28 + 1);
    while (name == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        name = (char *)calloc(1, strlen(device_type) + 28 + 1);
    }
    sprintf(name, "\"name\": \"%s " MAC_STR "\"", device_type, MAC2STR(self_mac));
    char *unique_id = (char *)calloc(1, 34 + 1);
    while (unique_id == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        unique_id = (char *)calloc(1, 34 + 1);
    }
    sprintf(unique_id, "\"unique_id\": \"" MAC_STR "-1\"", MAC2STR(self_mac));
    char *device_class = "\"device_class\": \"connectivity\"";
    char *state_topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(device_type) + 43 + 1);
    while (state_topic == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        state_topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(device_type) + 43 + 1);
    }
    sprintf(state_topic, "\"state_topic\": \"%s/%s/" MAC_STR "/status\"", mqtt_topic_prefix, device_type, MAC2STR(self_mac));
    char *json_attributes_topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(device_type) + 57 + 1);
    while (json_attributes_topic == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        json_attributes_topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(device_type) + 57 + 1);
    }
    sprintf(json_attributes_topic, "\"json_attributes_topic\": \"%s/%s/" MAC_STR "/attributes\"", mqtt_topic_prefix, device_type, MAC2STR(self_mac));
    char *payload_on = "\"payload_on\": \"online\"";
    char *expire_after = "\"expire_after\": 30";
    char *force_update = "\"force_update\": \"true\"";
    char *qos = "\"qos\": 2";
    char *topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + 41 + 1);
    while (topic == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + 41 + 1);
    }
    sprintf(topic, "%s/binary_sensor/" MAC_STR "-1/config", mqtt_topic_prefix, MAC2STR(self_mac));
    char *data = (char *)calloc(1, strlen(platform) + strlen(name) + strlen(unique_id) + strlen(device_class) + strlen(state_topic) +
                                       strlen(json_attributes_topic) + strlen(payload_on) + strlen(expire_after) + strlen(force_update) + strlen(qos) + 22 + 1);
    while (data == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        data = (char *)calloc(1, strlen(platform) + strlen(name) + strlen(unique_id) + strlen(device_class) + strlen(state_topic) +
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

// **** Keep alive message (always "online") from gateway to MQTT broker ***
// Message view as:
// zh-home/gateway/0C-B8-15-26-E0-79/status=online
void zh_send_mqtt_json_keep_alive_message_task(void *pvParameter)
{
    esp_mqtt_client_handle_t client = pvParameter;
    uint8_t self_mac[6];
    esp_read_mac(self_mac, ESP_MAC_WIFI_SOFTAP);
    char *device_type = get_device_type_value_name(ZHDT_GATEWAY);
    char *status = "online";
    for (;;)
    {
        char *topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(device_type) + 26 + 1);
        while (topic == NULL)
        {
            vTaskDelay(1 / portTICK_PERIOD_MS);
            topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(device_type) + 26 + 1);
        }
        sprintf(topic, "%s/%s/" MAC_STR "/status", mqtt_topic_prefix, device_type, MAC2STR(self_mac));
        esp_mqtt_client_publish(client, topic, status, 0, 2, true);
        free(topic);
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

// **** "Online" message from gateway to ESPNOW network ***
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

// **** "Offline" message from gateway to ESPNOW network ***
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

// **** Attributes message from espnow_switch via gateway to MQTT broker ***
// Message view as:
// {
//   "Type": "espnow_switch",
//   "MAC": "34-94-54-24-A3-40",
//   "Chip": "ESP32",
//   "App name": "zh_espnow_switch_esp32",
//   "App version": "0.0.1",
//   "Uptime": "Days:0 Hours:0 Mins:4"
// }
void zh_espnow_switch_send_mqtt_json_attributes_message(zh_espnow_data_t device_data, uint8_t device_mac[6])
{
    extern esp_mqtt_client_handle_t client;
    char *device_type = get_device_type_value_name(device_data.device_type);
    char *chip_type = get_chip_type_value_name(device_data.payload_data.attributes_message.chip_type);
    uint32_t secs;
    uint32_t mins;
    uint32_t hours;
    uint32_t days;
    char *type = (char *)calloc(1, strlen(device_type) + 10 + 1);
    while (type == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        type = (char *)calloc(1, strlen(device_type) + 10 + 1);
    }
    sprintf(type, "\"Type\": \"%s\"", device_type);
    char *mac = (char *)calloc(1, 26 + 1);
    while (mac == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        mac = (char *)calloc(1, 26 + 1);
    }
    sprintf(mac, "\"MAC\": \"" MAC_STR "\"", MAC2STR(device_mac));
    char *chip = (char *)calloc(1, strlen(chip_type) + 10 + 1);
    while (chip == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        chip = (char *)calloc(1, strlen(chip_type) + 10 + 1);
    }
    sprintf(chip, "\"Chip\": \"%s\"", chip_type);
    char *app_name = (char *)calloc(1, strlen(device_data.payload_data.attributes_message.app_name) + 14 + 1);
    while (app_name == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        app_name = (char *)calloc(1, strlen(device_data.payload_data.attributes_message.app_name) + 14 + 1);
    }
    sprintf(app_name, "\"App name\": \"%s\"", device_data.payload_data.attributes_message.app_name);
    char *app_ver = (char *)calloc(1, strlen(device_data.payload_data.attributes_message.app_version) + 17 + 1);
    while (app_ver == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        app_ver = (char *)calloc(1, strlen(device_data.payload_data.attributes_message.app_version) + 17 + 1);
    }
    sprintf(app_ver, "\"App version\": \"%s\"", device_data.payload_data.attributes_message.app_version);
    secs = device_data.payload_data.attributes_message.uptime;
    mins = secs / 60;
    hours = mins / 60;
    days = hours / 24;
    char *uptime = (char *)calloc(1, 56 + 1);
    while (uptime == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        uptime = (char *)calloc(1, 56 + 1);
    }
    sprintf(uptime, "\"Uptime\": \"Days:%lu Hours:%lu Mins:%lu\"", days, hours - (days * 24), mins - (hours * 60));
    char *topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(device_type) + 30 + 1);
    while (topic == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(device_type) + 30 + 1);
    }
    sprintf(topic, "%s/%s/" MAC_STR "/attributes", mqtt_topic_prefix, device_type, MAC2STR(device_mac));
    char *data = (char *)calloc(1, strlen(type) + strlen(mac) + strlen(chip) + strlen(app_ver) + strlen(app_name) + strlen(uptime) + 14 + 1);
    while (data == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        data = (char *)calloc(1, strlen(type) + strlen(mac) + strlen(chip) + strlen(app_ver) + strlen(app_name) + strlen(uptime) + 14 + 1);
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
}

// **** Config message from espnow_switch via gateway to MQTT broker ***
// Message view as:
// {
//   "platform": "mqtt",
//   "name": "espnow_switch 34-94-54-24-A3-40",
//   "unique_id": "34-94-54-24-A3-40-1",
//   "device_class": "switch",
//   "state_topic": "zh-home/espnow_switch/34-94-54-24-A3-40/state",
//   "availability_topic": "zh-home/espnow_switch/34-94-54-24-A3-40/status",
//   "command_topic": "zh-home/espnow_switch/34-94-54-24-A3-40/set",
//   "json_attributes_topic": "zh-home/espnow_switch/34-94-54-24-A3-40/attributes",
//   "enabled_by_default": "true",
//   "optimistic": "false",
//   "payload_on": "ON",
//   "payload_off": "OFF",
//   "qos": 2,
//   "retain": "true"
// }
void zh_espnow_switch_send_mqtt_json_config_message(zh_espnow_data_t device_data, uint8_t device_mac[6])
{
    extern esp_mqtt_client_handle_t client;
    char *device_type = get_device_type_value_name(device_data.device_type);
    char *platform = "\"platform\": \"mqtt\"";
    char *name = (char *)calloc(1, strlen(device_type) + 28 + 1);
    while (name == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        name = (char *)calloc(1, strlen(device_type) + 28 + 1);
    }
    sprintf(name, "\"name\": \"%s " MAC_STR "\"", device_type, MAC2STR(device_mac));
    char *unique_id = (char *)calloc(1, 36 + 1);
    while (unique_id == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        unique_id = (char *)calloc(1, 36 + 1);
    }
    sprintf(unique_id, "\"unique_id\": \"" MAC_STR "-%d\"", MAC2STR(device_mac), device_data.payload_data.config_message.switch_config_message.unique_id);
    char *device_class = (char *)calloc(1, strlen(get_switch_device_class_value_name(device_data.payload_data.config_message.switch_config_message.device_class)) + 18 + 1);
    while (device_class == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        device_class = (char *)calloc(1, strlen(get_switch_device_class_value_name(device_data.payload_data.config_message.switch_config_message.device_class)) + 18 + 1);
    }
    sprintf(device_class, "\"device_class\": \"%s\"", get_switch_device_class_value_name(device_data.payload_data.config_message.switch_config_message.device_class));
    char *state_topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(device_type) + 42 + 1);
    while (state_topic == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        state_topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(device_type) + 42 + 1);
    }
    sprintf(state_topic, "\"state_topic\": \"%s/%s/" MAC_STR "/state\"", mqtt_topic_prefix, device_type, MAC2STR(device_mac));
    // value_template not used.
    char *availability_topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(device_type) + 50 + 1);
    while (availability_topic == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        availability_topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(device_type) + 50 + 1);
    }
    sprintf(availability_topic, "\"availability_topic\": \"%s/%s/" MAC_STR "/status\"", mqtt_topic_prefix, device_type, MAC2STR(device_mac));
    char *command_topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(device_type) + 42 + 1);
    while (command_topic == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        command_topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(device_type) + 42 + 1);
    }
    sprintf(command_topic, "\"command_topic\": \"%s/%s/" MAC_STR "/set\"", mqtt_topic_prefix, device_type, MAC2STR(device_mac));
    char *json_attributes_topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(device_type) + 57 + 1);
    while (json_attributes_topic == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        json_attributes_topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(device_type) + 57 + 1);
    }
    sprintf(json_attributes_topic, "\"json_attributes_topic\": \"%s/%s/" MAC_STR "/attributes\"", mqtt_topic_prefix, device_type, MAC2STR(device_mac));
    char *enabled_by_default = NULL;
    if (device_data.payload_data.config_message.switch_config_message.enabled_by_default == true)
    {
        enabled_by_default = "\"enabled_by_default\": \"true\"";
    }
    else if (device_data.payload_data.config_message.switch_config_message.enabled_by_default == false)
    {
        enabled_by_default = "\"enabled_by_default\": \"false\"";
    }
    else
    {
        free(name);
        free(unique_id);
        free(device_class);
        free(state_topic);
        free(availability_topic);
        free(command_topic);
        free(json_attributes_topic);
        return;
    }
    char *optimistic = NULL;
    if (device_data.payload_data.config_message.switch_config_message.optimistic == true)
    {
        optimistic = "\"optimistic\": \"true\"";
    }
    else if (device_data.payload_data.config_message.switch_config_message.optimistic == false)
    {
        optimistic = "\"optimistic\": \"false\"";
    }
    else
    {
        free(name);
        free(unique_id);
        free(device_class);
        free(state_topic);
        free(availability_topic);
        free(command_topic);
        free(json_attributes_topic);
        return;
    }
    // payload_available not used.
    // payload_not_available not used.
    char *payload_on = (char *)calloc(1, strlen(get_on_off_type_value_name(device_data.payload_data.config_message.switch_config_message.payload_on)) + 16 + 1);
    while (payload_on == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        payload_on = (char *)calloc(1, strlen(get_on_off_type_value_name(device_data.payload_data.config_message.switch_config_message.payload_on)) + 16 + 1);
    }
    sprintf(payload_on, "\"payload_on\": \"%s\"", get_on_off_type_value_name(device_data.payload_data.config_message.switch_config_message.payload_on));
    char *payload_off = (char *)calloc(1, strlen(get_on_off_type_value_name(device_data.payload_data.config_message.switch_config_message.payload_off)) + 17 + 1);
    while (payload_off == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        payload_off = (char *)calloc(1, strlen(get_on_off_type_value_name(device_data.payload_data.config_message.switch_config_message.payload_off)) + 17 + 1);
    }
    sprintf(payload_off, "\"payload_off\": \"%s\"", get_on_off_type_value_name(device_data.payload_data.config_message.switch_config_message.payload_off));
    char *qos = (char *)calloc(1, 10 + 1);
    while (qos == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        qos = (char *)calloc(1, 10 + 1);
    }
    sprintf(qos, "\"qos\": %d", device_data.payload_data.config_message.switch_config_message.qos);
    char *retain = NULL;
    if (device_data.payload_data.config_message.switch_config_message.retain == true)
    {
        retain = "\"retain\": \"true\"";
    }
    else if (device_data.payload_data.config_message.switch_config_message.retain == false)
    {
        retain = "\"retain\": \"false\"";
    }
    else
    {
        free(name);
        free(unique_id);
        free(device_class);
        free(state_topic);
        free(availability_topic);
        free(command_topic);
        free(json_attributes_topic);
        free(payload_on);
        free(payload_off);
        free(qos);
        return;
    }
    char *topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + 34 + 1);
    while (topic == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + 34 + 1);
    }
    sprintf(topic, "%s/switch/" MAC_STR "-%d/config", mqtt_topic_prefix, MAC2STR(device_mac), device_data.payload_data.config_message.switch_config_message.unique_id);
    char *data = (char *)calloc(1, strlen(platform) + strlen(name) + strlen(unique_id) + strlen(device_class) + strlen(state_topic) +
                                       strlen(availability_topic) + strlen(command_topic) + strlen(json_attributes_topic) +
                                       strlen(enabled_by_default) + strlen(optimistic) + strlen(payload_on) + strlen(payload_off) +
                                       strlen(qos) + strlen(retain) + 30 + 1);
    while (data == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        data = (char *)calloc(1, strlen(platform) + strlen(name) + strlen(unique_id) + strlen(device_class) + strlen(state_topic) +
                                     strlen(availability_topic) + strlen(command_topic) + strlen(json_attributes_topic) +
                                     strlen(enabled_by_default) + strlen(optimistic) + strlen(payload_on) + strlen(payload_off) +
                                     strlen(qos) + strlen(retain) + 30 + 1);
    }
    sprintf(data, "{ %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s }", platform, name, unique_id, device_class, state_topic, availability_topic,
            command_topic, json_attributes_topic, enabled_by_default, optimistic, payload_on, payload_off, qos, retain);
    esp_mqtt_client_publish(client, topic, data, 0, 2, true);
    free(name);
    free(unique_id);
    free(device_class);
    free(state_topic);
    free(availability_topic);
    free(command_topic);
    free(json_attributes_topic);
    free(payload_on);
    free(payload_off);
    free(qos);
    free(topic);
    free(data);
}

// **** Keep alive message from espnow_switch via gateway to MQTT broker ***
// Message view as:
// zh-home/espnow_switch/34-94-54-24-A3-40/status=online (offline)
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
    else
    {
        return;
    }
    char *topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(device_type) + 26 + 1);
    while (topic == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(device_type) + 26 + 1);
    }
    sprintf(topic, "%s/%s/" MAC_STR "/status", mqtt_topic_prefix, device_type, MAC2STR(device_mac));
    esp_mqtt_client_publish(client, topic, status, 0, 2, true);
    free(topic);
}

// **** Status message from espnow_switch via gateway to MQTT broker ***
// Message view as:
// zh-home/espnow_switch/34-94-54-24-A3-40/state=ON (OFF)
void zh_espnow_switch_send_mqtt_json_status_message(zh_espnow_data_t device_data, uint8_t device_mac[6])
{
    extern esp_mqtt_client_handle_t client;
    char *device_type = get_device_type_value_name(device_data.device_type);
    char *status = get_on_off_type_value_name(device_data.payload_data.status_message.switch_status_message.status);
    char *topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(device_type) + 30 + 1);
    while (topic == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        topic = (char *)calloc(1, strlen(mqtt_topic_prefix) + strlen(device_type) + 30 + 1);
    }
    sprintf(topic, "%s/%s/" MAC_STR "/state", mqtt_topic_prefix, device_type, MAC2STR(device_mac));
    esp_mqtt_client_publish(client, topic, status, 0, 2, true);
    free(topic);
}

// **** Setup message to espnow_switch via gateway from MQTT broker ***
// Message view as:
// zh-home/espnow_switch/34-94-54-24-A3-40/set=ON (OFF)
void zh_send_espnow_switch_setup_message(ha_on_off_type_t status, uint8_t device_mac[6])
{

    zh_switch_status_message_t switch_status_message;
    switch_status_message.status = status;
    zh_status_message_t status_message = (zh_status_message_t)switch_status_message;
    zh_espnow_data_t data;
    strcpy(data.net_name, ESPNOW_NET_NAME);
    data.device_type = ZHDT_GATEWAY;
    data.payload_type = ZHPT_SET;
    data.payload_data = (zh_payload_data_t)status_message;
    zh_espnow_send(device_mac, (uint8_t *)&data, sizeof(zh_espnow_data_t));
}