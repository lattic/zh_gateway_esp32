#ifndef ZH_SNTP_H
#define ZH_SNTP_H

#include "esp_sntp.h"
#include "zh_config.h"

#define SNTP_TASK_PRIORITY 2
#define SNTP_STACK_SIZE 2048

#ifdef __cplusplus
extern "C"
{
#endif

    void zh_sntp_time_init(void);
    void zh_sntp_time_deinit(void);
    void zh_send_espnow_current_time_task(void *pvParameter);

#ifdef __cplusplus
}
#endif

#endif