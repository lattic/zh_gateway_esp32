idf_component_register(SRCS "zh_eth.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_common
                        driver
                        esp_netif
                        esp_eth
                        zh_sntp
                        zh_mqtt
                        zh_ota
                        zh_config)