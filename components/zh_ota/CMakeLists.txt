idf_component_register(SRCS "zh_ota.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_common
                        bootloader_support
                        app_update
                        esp_http_client
                        zh_config
                    EMBED_TXTFILES ${project_dir}/certificate.pem)