#ifndef ZH_ESPNOW_H
#define ZH_ESPNOW_H

#include <string.h>
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_now.h"
#include "esp_mac.h"
#include "zh_json.h"
#include "zh_config.h"

#define ESPNOW_TASK_PRIORITY 4
#define ESPNOW_STACK_SIZE 3072
#define ESPNOW_QUEUE_SIZE 32
#define MAX_NUMBER_OF_ATTEMPTS_TO_SEND 5

#define MAC_STR "%02X%02X%02X%02X%02X%02X"

#define DATA_SEND_SUCCESS BIT0
#define DATA_SEND_FAIL BIT1

#ifdef __cplusplus
extern "C"
{
#endif

    ESP_EVENT_DECLARE_BASE(ESPNOW);
    enum
    {
        ESPNOW_ON_RECV_EVENT,
        ESPNOW_ON_SEND_EVENT
    };

    typedef struct
    {
        uint8_t mac_addr[ESP_NOW_ETH_ALEN];
        esp_now_send_status_t status;
    } espnow_event_on_send_t;

    typedef struct
    {
        uint8_t mac_addr[ESP_NOW_ETH_ALEN];
        uint8_t *data;
        uint8_t data_len;
    } espnow_event_on_recv_t;

    typedef enum
    {
        ESP_NOW_RECV,
        ESP_NOW_SEND,
    } espnow_queue_id_t;

    typedef struct
    {
        uint8_t mac_addr[ESP_NOW_ETH_ALEN];
        esp_now_send_status_t status;
    } espnow_queue_status_data_t;

    typedef struct
    {
        uint8_t mac_addr[ESP_NOW_ETH_ALEN];
        uint8_t *data;
        uint8_t data_len;
    } espnow_queue_send_data_t;

    typedef struct
    {
        uint8_t mac_addr[ESP_NOW_ETH_ALEN];
        uint8_t *data;
        uint8_t data_len;
    } espnow_queue_recv_data_t;

    typedef union
    {
        espnow_queue_send_data_t send_data;
        espnow_queue_recv_data_t recv_data;
    } espnow_queue_data_t;

    typedef struct
    {
        espnow_queue_id_t id;
        espnow_queue_data_t data;
    } espnow_queue_t;

    void zh_espnow_init(void);
    void zh_espnow_send(const uint8_t *target, const uint8_t *data, const uint8_t data_len);
    void zh_espnow_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status);
    void zh_espnow_recv_cb(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len);
    void zh_espnow_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
    void zh_espnow_processing_task(void *pvParameter);

#ifdef __cplusplus
}
#endif

#endif