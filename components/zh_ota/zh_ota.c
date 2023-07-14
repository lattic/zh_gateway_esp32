#include "zh_ota.h"

extern const uint8_t server_certificate_pem_start[] asm("_binary_certificate_pem_start");
extern const uint8_t server_certificate_pem_end[] asm("_binary_certificate_pem_end");

static char s_ota_write_data[HTTP_BUFF_SIZE + 1] = {0};

extern char *firmware_upgrade_url;

void zh_self_ota_update_task(void *pvParameter)
{
    esp_err_t err;
    esp_ota_handle_t update_handle = 0;
    const esp_partition_t *update_partition = NULL;
    const esp_partition_t *running = esp_ota_get_running_partition();
    if (running == NULL)
    {
        vTaskDelete(NULL);
    }
    const esp_app_desc_t *app_info = esp_app_get_description();
    char *app_name = (char *)calloc(1, strlen(firmware_upgrade_url) + strlen(app_info->project_name) + 5 + 1);
    while (app_name == NULL)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        app_name = (char *)calloc(1, strlen(firmware_upgrade_url) + strlen(app_info->project_name) + 5 + 1);
    }
    sprintf(app_name, "%s/%s.bin", firmware_upgrade_url, app_info->project_name);
    esp_http_client_config_t config = {
        .url = app_name,
        .cert_pem = (char *)server_certificate_pem_start,
        .timeout_ms = 5000,
        .keep_alive_enable = true,
        .skip_cert_common_name_check = true,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    free(app_name);
    if (client == NULL)
    {
        vTaskDelete(NULL);
    }
    err = esp_http_client_open(client, 0);
    if (err != ESP_OK)
    {
        esp_http_client_cleanup(client);
        vTaskDelete(NULL);
    }
    esp_http_client_fetch_headers(client);
    update_partition = esp_ota_get_next_update_partition(NULL);
    if (update_partition == NULL)
    {
        vTaskDelete(NULL);
    }
    bool image_header_was_checked = false;
    for (;;)
    {
        int data_read = esp_http_client_read(client, s_ota_write_data, HTTP_BUFF_SIZE);
        if (data_read < 0)
        {
            esp_http_client_close(client);
            esp_http_client_cleanup(client);
            vTaskDelete(NULL);
        }
        else if (data_read > 0)
        {
            if (image_header_was_checked == false)
            {
                esp_app_desc_t new_app_info;
                if (data_read > sizeof(esp_image_header_t) + sizeof(esp_image_segment_header_t) + sizeof(esp_app_desc_t))
                {
                    memcpy(&new_app_info, &s_ota_write_data[sizeof(esp_image_header_t) + sizeof(esp_image_segment_header_t)], sizeof(esp_app_desc_t));
                    esp_app_desc_t running_app_info;
                    esp_ota_get_partition_description(running, &running_app_info);
                    const esp_partition_t *last_invalid_app = esp_ota_get_last_invalid_partition();
                    if (last_invalid_app != NULL)
                    {
                        esp_app_desc_t invalid_app_info;
                        esp_ota_get_partition_description(last_invalid_app, &invalid_app_info);
                        if (memcmp(invalid_app_info.version, new_app_info.version, sizeof(new_app_info.version)) == 0)
                        {
                            esp_http_client_close(client);
                            esp_http_client_cleanup(client);
                            vTaskDelete(NULL);
                        }
                    }
                    if (memcmp(new_app_info.version, running_app_info.version, sizeof(new_app_info.version)) == 0)
                    {
                        esp_http_client_close(client);
                        esp_http_client_cleanup(client);
                        vTaskDelete(NULL);
                    }
                    if (memcmp(new_app_info.version, running_app_info.version, sizeof(new_app_info.version)) < 0)
                    {
                        esp_http_client_close(client);
                        esp_http_client_cleanup(client);
                        vTaskDelete(NULL);
                    }
                    image_header_was_checked = true;
                    err = esp_ota_begin(update_partition, OTA_WITH_SEQUENTIAL_WRITES, &update_handle);
                    while (err != ESP_OK)
                    {
                        if (err == ESP_ERR_NO_MEM)
                        {
                            vTaskDelay(1 / portTICK_PERIOD_MS);
                            err = esp_ota_begin(update_partition, OTA_WITH_SEQUENTIAL_WRITES, &update_handle);
                        }
                        else
                        {
                            esp_http_client_close(client);
                            esp_http_client_cleanup(client);
                            esp_ota_abort(update_handle);
                            vTaskDelete(NULL);
                        }
                    }
                }
                else
                {
                    esp_http_client_close(client);
                    esp_http_client_cleanup(client);
                    vTaskDelete(NULL);
                }
            }
            err = esp_ota_write(update_handle, (const void *)s_ota_write_data, data_read);
            if (err != ESP_OK)
            {
                esp_http_client_close(client);
                esp_http_client_cleanup(client);
                esp_ota_abort(update_handle);
                vTaskDelete(NULL);
            }
        }
        else
        {
            break;
        }
    }
    if (esp_http_client_is_complete_data_received(client) == false)
    {
        esp_http_client_close(client);
        esp_http_client_cleanup(client);
        esp_ota_abort(update_handle);
        vTaskDelete(NULL);
    }
    err = esp_ota_end(update_handle);
    if (err != ESP_OK)
    {
        esp_http_client_close(client);
        esp_http_client_cleanup(client);
        vTaskDelete(NULL);
    }
    err = esp_ota_set_boot_partition(update_partition);
    if (err != ESP_OK)
    {
        esp_http_client_close(client);
        esp_http_client_cleanup(client);
        vTaskDelete(NULL);
    }
    esp_restart();
}