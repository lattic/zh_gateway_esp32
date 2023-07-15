#include "nvs_flash.h"
#include "esp_ota_ops.h"
#include "esp_netif.h"
#include "zh_eth.h"
#include "zh_wifi.h"
#include "zh_espnow.h"
#include "secrets.h"

bool is_lan_connection = true; // If "true", the LAN connection will be used. If "false", the WiFi connection will be used. In case of using WiFi router must be set on channel 1.

// Create file "secrets.h" in the "main" folder with definitions below.
char *wifi_ssid = WIFI_SSID;
char *wifi_pass = WIFI_PASS;
char *mqtt_broker_url = MQTT_BROKER_URL;           // For example - "mqtt://mqttbroker.com".
char *mqtt_topic_prefix = MQTT_TOPIC_PREFIX;       // For example - "homeassistant".
char *ntp_server_url = NTP_SERVER_URL;             // For example - "ntpserver.com".
char *ntp_time_zone = NTP_TIME_ZONE;               // For example - "UTC-3".
char *firmware_upgrade_url = FIRMWARE_UPGRADE_URL; // For example - "https://yourserver.com". The certificate (certificate.pem) must match the upgrade server!

void app_main()
{
    esp_err_t err;
    const esp_partition_t *running = esp_ota_get_running_partition();
    esp_ota_img_states_t ota_state;
    esp_ota_get_state_partition(running, &ota_state);
    err = nvs_flash_init();
    while (err == ESP_ERR_NO_MEM)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        err = nvs_flash_init();
    }
    esp_netif_init();
    err = esp_event_loop_create_default();
    while (err == ESP_ERR_NO_MEM)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        err = esp_event_loop_create_default();
    };
    if (is_lan_connection == true)
    {
        zh_eth_init();
    }
    else if (is_lan_connection == false)
    {
        zh_wifi_init_sta();
    }
    else
    {
        vTaskDelete(NULL);
    }
    zh_espnow_init();
    if (ota_state == ESP_OTA_IMG_PENDING_VERIFY)
    {
        vTaskDelay(60000 / portTICK_PERIOD_MS);
        esp_ota_mark_app_valid_cancel_rollback();
    }
}