#include "zh_eth.h"

extern bool sntp_is_enable;
extern bool mqtt_is_enable;

void zh_eth_init(void)
{
    esp_err_t err;
    gpio_config_t config;
    config.intr_type = GPIO_INTR_DISABLE;
    config.mode = GPIO_MODE_OUTPUT;
    config.pin_bit_mask = (1ULL << LAN_POWER_PIN);
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&config);
    gpio_set_level(LAN_POWER_PIN, HIGH);
    esp_netif_config_t esp_netif_config = ESP_NETIF_DEFAULT_ETH();
    esp_netif_t *esp_netif_eth = esp_netif_new(&esp_netif_config);
    eth_mac_config_t mac_config = ETH_MAC_DEFAULT_CONFIG();
    eth_phy_config_t phy_config = ETH_PHY_DEFAULT_CONFIG();
    eth_esp32_emac_config_t esp32_emac_config = ETH_ESP32_EMAC_DEFAULT_CONFIG();
    esp_eth_mac_t *mac = esp_eth_mac_new_esp32(&esp32_emac_config, &mac_config);
    esp_eth_phy_t *phy = esp_eth_phy_new_lan87xx(&phy_config);
    esp_eth_config_t esp_eth_config = ETH_DEFAULT_CONFIG(mac, phy);
    esp_eth_handle_t esp_eth_handle = NULL;
    err = esp_eth_driver_install(&esp_eth_config, &esp_eth_handle);
    while (err == ESP_ERR_NO_MEM)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        err = esp_eth_driver_install(&esp_eth_config, &esp_eth_handle);
    }
    esp_netif_attach(esp_netif_eth, esp_eth_new_netif_glue(esp_eth_handle));
    err = esp_event_handler_register(ETH_EVENT, ESP_EVENT_ANY_ID, &zh_eth_event_handler, NULL);
    while (err == ESP_ERR_NO_MEM)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        err = esp_event_handler_register(ETH_EVENT, ESP_EVENT_ANY_ID, &zh_eth_event_handler, NULL);
    }
    err = esp_event_handler_register(IP_EVENT, IP_EVENT_ETH_GOT_IP, &zh_eth_event_handler, NULL);
    while (err == ESP_ERR_NO_MEM)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        err = esp_event_handler_register(IP_EVENT, IP_EVENT_ETH_GOT_IP, &zh_eth_event_handler, NULL);
    }
    esp_eth_start(esp_eth_handle);
}

void zh_eth_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case ETHERNET_EVENT_DISCONNECTED:
        if (mqtt_is_enable == true)
        {
            zh_mqtt_deinit();
        }
        if (sntp_is_enable == true)
        {
            zh_sntp_time_deinit();
        }
        break;
    case IP_EVENT_ETH_GOT_IP:
        xTaskCreatePinnedToCore(&zh_self_ota_update_task, "Self OTA update", OTA_STACK_SIZE, NULL, OTA_TASK_PRIORITY, NULL, tskNO_AFFINITY);
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