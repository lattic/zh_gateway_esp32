#ifndef ZH_OTA_H
#define ZH_OTA_H

#include "esp_err.h"
#include "esp_app_format.h"
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "zh_config.h"

#define OTA_TASK_PRIORITY 3
#define OTA_STACK_SIZE 8192
#define HTTP_BUFF_SIZE 1024

#ifdef __cplusplus
extern "C"
{
#endif

    void zh_self_ota_update_task(void *pvParameter);

#ifdef __cplusplus
}
#endif

#endif