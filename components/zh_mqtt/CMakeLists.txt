idf_component_register(SRCS "zh_mqtt.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_common
                        mqtt
                        zh_json
                        zh_config)