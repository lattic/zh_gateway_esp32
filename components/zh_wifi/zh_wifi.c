#include "zh_wifi.h"

extern bool sntp_is_enable;
extern bool mqtt_is_enable;

extern char *wifi_ssid;
extern char *wifi_pass;

static esp_timer_handle_t s_wifi_reconnect_timer;
static uint8_t s_wifi_reconnect_retry_num = 0;

void zh_wifi_init_sta()
{
    esp_err_t err;
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_init_sta_config = WIFI_INIT_CONFIG_DEFAULT();
    err = esp_wifi_init(&wifi_init_sta_config);
    while (err == ESP_ERR_NO_MEM)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        err = esp_wifi_init(&wifi_init_sta_config);
    }
    esp_wifi_set_storage(WIFI_STORAGE_RAM);
    wifi_config_t wifi_config;
    memcpy(wifi_config.sta.ssid, wifi_ssid, sizeof(wifi_config.sta.ssid));
    memcpy(wifi_config.sta.password, wifi_pass, sizeof(wifi_config.sta.password));
    esp_wifi_set_mode(WIFI_MODE_APSTA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_set_ps(WIFI_PS_NONE);
    esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N | WIFI_PROTOCOL_LR);
    err = esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &zh_wifi_event_handler, NULL, NULL);
    while (err == ESP_ERR_NO_MEM)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        err = esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &zh_wifi_event_handler, NULL, NULL);
    }
    err = esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &zh_wifi_event_handler, NULL, NULL);
    while (err == ESP_ERR_NO_MEM)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        err = esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &zh_wifi_event_handler, NULL, NULL);
    }
    err = esp_wifi_start();
    while (err == ESP_ERR_NO_MEM)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        err = esp_wifi_start();
    }
    esp_wifi_set_max_tx_power(84);
}

void zh_wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    esp_err_t err;
    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        if (mqtt_is_enable == true)
        {
            zh_mqtt_deinit();
        }
        if (sntp_is_enable == true)
        {
            zh_sntp_time_deinit();
        }
        if (s_wifi_reconnect_retry_num < WIFI_MAXIMUM_RETRY)
        {
            esp_wifi_connect();
            ++s_wifi_reconnect_retry_num;
        }
        else
        {
            s_wifi_reconnect_retry_num = 0;
            esp_timer_create_args_t wifi_reconnect_timer_args = {
                .callback = (void *)esp_wifi_connect};
            err = esp_timer_create(&wifi_reconnect_timer_args, &s_wifi_reconnect_timer);
            while (err == ESP_ERR_NO_MEM)
            {
                vTaskDelay(1 / portTICK_PERIOD_MS);
                err = esp_timer_create(&wifi_reconnect_timer_args, &s_wifi_reconnect_timer);
            };
            esp_timer_start_once(s_wifi_reconnect_timer, WIFI_RECONNECT_TIME * 1000);
        }
        break;
    case IP_EVENT_STA_GOT_IP:
        s_wifi_reconnect_retry_num = 0;
        if (mqtt_is_enable == false)
        {
            zh_mqtt_init();
        }
        if (sntp_is_enable == false)
        {
            zh_sntp_time_init();
        }
        break;
    default:
        break;
    }
}