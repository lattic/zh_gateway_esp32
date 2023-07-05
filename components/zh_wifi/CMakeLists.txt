idf_component_register(SRCS "zh_wifi.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_wifi
                        esp_timer
                        zh_sntp
                        zh_mqtt
                        zh_ota
                        zh_espnow
                        zh_config)