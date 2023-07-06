idf_component_register(SRCS "zh_espnow.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_common
                        esp_wifi
                        esp_hw_support
                        zh_json
                        zh_config)