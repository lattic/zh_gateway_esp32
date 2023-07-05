#include "zh_sntp.h"

bool sntp_is_enable = false;

static TaskHandle_t espnow_current_time_task;

extern char *ntp_server_url;
extern char *ntp_time_zone;

void zh_sntp_time_init()
{
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);
    sntp_setservername(0, ntp_server_url);
    sntp_init();
    sntp_is_enable = true;
    xTaskCreatePinnedToCore(&zh_send_espnow_current_time_task, "Send current time", SNTP_STACK_SIZE, NULL, SNTP_TASK_PRIORITY, &espnow_current_time_task, tskNO_AFFINITY);
}

void zh_sntp_time_deinit()
{
    sntp_stop();
    sntp_is_enable = false;
    vTaskDelete(espnow_current_time_task);
}

void zh_send_espnow_current_time_task(void *pvParameter)
{
    while (sntp_get_sync_status() != SNTP_SYNC_STATUS_COMPLETED)
    {
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
    time_t now;
    setenv("TZ", ntp_time_zone, 1);
    tzset();
    for (;;)
    {
        time(&now);
        // To Do.
        vTaskDelay(86400000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}