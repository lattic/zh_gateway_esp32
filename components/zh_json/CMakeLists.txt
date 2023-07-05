idf_component_register(SRCS "zh_json.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_app_format
                        mqtt
                        esp_timer
                        esp_hw_support
                        zh_espnow
                        zh_config)