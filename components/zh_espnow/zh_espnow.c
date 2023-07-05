#include "zh_espnow.h"

extern bool is_lan_connection;
extern bool mqtt_is_connected;

static EventGroupHandle_t s_espnow_send_cb_status;
static QueueHandle_t s_espnow_queue;

ESP_EVENT_DEFINE_BASE(ESPNOW);

void zh_espnow_init()
{
    esp_err_t err;
    s_espnow_send_cb_status = xEventGroupCreate();
    s_espnow_queue = xQueueCreate(ESPNOW_QUEUE_SIZE, sizeof(espnow_queue_t));
    while (s_espnow_queue == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        s_espnow_queue = xQueueCreate(ESPNOW_QUEUE_SIZE, sizeof(espnow_queue_t));
    }
    if (is_lan_connection == true)
    {
        wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
        err = esp_wifi_init(&wifi_init_config);
        while (err == ESP_ERR_NO_MEM)
        {
            vTaskDelay(1 / portTICK_PERIOD_MS);
            err = esp_wifi_init(&wifi_init_config);
        }
        esp_wifi_set_storage(WIFI_STORAGE_RAM);
        esp_wifi_set_mode(WIFI_MODE_AP);
        err = esp_wifi_start();
        while (err == ESP_ERR_NO_MEM)
        {
            vTaskDelay(1 / portTICK_PERIOD_MS);
            err = esp_wifi_start();
        }
        esp_wifi_set_max_tx_power(84);
    }
    esp_wifi_set_ps(WIFI_PS_NONE);
    esp_wifi_set_protocol(WIFI_IF_AP, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_LR);
    esp_wifi_config_espnow_rate(WIFI_IF_AP, WIFI_PHY_RATE_1M_L);
    esp_now_init();
    esp_now_set_wake_window(65535);
    esp_now_register_send_cb(zh_espnow_send_cb);
    esp_now_register_recv_cb(zh_espnow_recv_cb);
    err = esp_event_handler_instance_register(ESPNOW, ESP_EVENT_ANY_ID, &zh_espnow_event_handler, NULL, NULL);
    while (err == ESP_ERR_NO_MEM)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        err = esp_event_handler_instance_register(ESPNOW, ESP_EVENT_ANY_ID, &zh_espnow_event_handler, NULL, NULL);
    }
    xTaskCreatePinnedToCore(&zh_espnow_processing_task, "ESPNOW processing task", ESPNOW_STACK_SIZE, NULL, ESPNOW_TASK_PRIORITY, NULL, tskNO_AFFINITY);
}

void zh_espnow_send(const uint8_t *target, const uint8_t *data, const uint8_t data_len)
{
    espnow_queue_t espnow_queue;
    espnow_queue.id = ESP_NOW_SEND;
    espnow_queue_send_data_t *send_data = &espnow_queue.data.send_data;
    memcpy(send_data->mac_addr, target, ESP_NOW_ETH_ALEN);
    send_data->data = malloc(data_len);
    while (send_data->data == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        send_data->data = malloc(data_len);
    }
    memcpy(send_data->data, data, data_len);
    send_data->data_len = data_len;
    xQueueSend(s_espnow_queue, &espnow_queue, 0);
}

void zh_espnow_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    if (status == ESP_NOW_SEND_SUCCESS)
    {
        xEventGroupSetBits(s_espnow_send_cb_status, DATA_SEND_SUCCESS);
    }
    else
    {
        xEventGroupSetBits(s_espnow_send_cb_status, DATA_SEND_FAIL);
    }
}

void zh_espnow_recv_cb(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len)
{
    espnow_queue_t espnow_queue;
    espnow_queue_recv_data_t *recv_data = &espnow_queue.data.recv_data;
    espnow_queue.id = ESP_NOW_RECV;
    memcpy(recv_data->mac_addr, esp_now_info->src_addr, ESP_NOW_ETH_ALEN);
    recv_data->data = malloc(data_len);
    while (recv_data->data == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        recv_data->data = malloc(data_len);
    }
    memcpy(recv_data->data, data, data_len);
    recv_data->data_len = data_len;
    xQueueSend(s_espnow_queue, &espnow_queue, 0);
}

void zh_espnow_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case ESPNOW_ON_RECV_EVENT:
        espnow_event_on_recv_t *recv_data = event_data;
        if (recv_data->data_len != sizeof(zh_espnow_data_t))
        {
            goto ESPNOW_EVENT_HANDLER_EXIT;
        }
        zh_espnow_data_t data;
        memcpy(&data, recv_data->data, recv_data->data_len);
        if (memcmp(&data.net_name, ESPNOW_NET_NAME, sizeof(ESPNOW_NET_NAME)) != 0)
        {
            goto ESPNOW_EVENT_HANDLER_EXIT;
        }
    ESPNOW_EVENT_HANDLER_EXIT:
        free(recv_data->data); // Do not delete for prevent of memory leakage!!!
        break;
    case ESPNOW_ON_SEND_EVENT:
        espnow_event_on_send_t *send_data = event_data;
        if (send_data->status == ESP_NOW_SEND_SUCCESS)
        {
            // Reserved for future developments.
        }
        else
        {
            // Reserved for future developments.
        }
        break;
    default:
        break;
    }
}

void zh_espnow_processing_task(void *pvParameter)
{
    espnow_queue_t espnow_queue;
    while (xQueueReceive(s_espnow_queue, &espnow_queue, portMAX_DELAY) == pdTRUE)
    {
        switch (espnow_queue.id)
        {
        case ESP_NOW_SEND:
            esp_now_peer_info_t *peer = malloc(sizeof(esp_now_peer_info_t));
            while (peer == NULL)
            {
                vTaskDelay(1 / portTICK_PERIOD_MS);
                peer = malloc(sizeof(esp_now_peer_info_t));
            }
            memset(peer, 0, sizeof(esp_now_peer_info_t));
            peer->ifidx = WIFI_IF_AP;
            espnow_queue_send_data_t *send_data = &espnow_queue.data.send_data;
            memcpy(peer->peer_addr, send_data->mac_addr, ESP_NOW_ETH_ALEN);
            esp_now_add_peer(peer);
            uint8_t attempted_transmission = 0;
            espnow_event_on_send_t *on_send = malloc(sizeof(espnow_event_on_send_t));
            while (on_send == NULL)
            {
                vTaskDelay(1 / portTICK_PERIOD_MS);
                on_send = malloc(sizeof(espnow_event_on_send_t));
            }
            memcpy(on_send->mac_addr, send_data->mac_addr, ESP_NOW_ETH_ALEN);
        RESEND_ESPNOW_MESSAGE:
            esp_now_send(send_data->mac_addr, send_data->data, send_data->data_len);
            EventBits_t bit = xEventGroupWaitBits(s_espnow_send_cb_status, DATA_SEND_SUCCESS | DATA_SEND_FAIL, pdTRUE, pdFALSE, 50 / portTICK_PERIOD_MS);
            if (bit & DATA_SEND_SUCCESS)
            {
                on_send->status = ESP_NOW_SEND_SUCCESS;
                esp_event_post(ESPNOW, ESPNOW_ON_SEND_EVENT, on_send, sizeof(espnow_event_on_send_t), portMAX_DELAY);
            }
            else if (bit & DATA_SEND_FAIL)
            {
                if (attempted_transmission < MAX_NUMBER_OF_ATTEMPTS_TO_SEND)
                {
                    ++attempted_transmission;
                    goto RESEND_ESPNOW_MESSAGE;
                }
                on_send->status = ESP_NOW_SEND_FAIL;
                esp_event_post(ESPNOW, ESPNOW_ON_SEND_EVENT, on_send, sizeof(espnow_event_on_send_t), portMAX_DELAY);
            }
            free(send_data->data);
            esp_now_del_peer(send_data->mac_addr);
            free(peer);
            free(on_send);
            break;
        case ESP_NOW_RECV:
            espnow_queue_recv_data_t *recv_data = &espnow_queue.data.recv_data;
            esp_event_post(ESPNOW, ESPNOW_ON_RECV_EVENT, recv_data, recv_data->data_len + ESP_NOW_ETH_ALEN + 1, portMAX_DELAY);
            break;
        default:
            break;
        }
    }
    vTaskDelete(NULL);
}