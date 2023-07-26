# Gateway

Gateway for data exchange between ESP-NOW devices and MQTT broker via WiFi/LAN.

## Features

1. Automatically adds gateway configuration to Home Assistan via MQTT discovery as a binary_sensor.
2. Automatically adds supported devices configurations to Home Assistan via MQTT discovery.
3. Update firmware from HTTPS server over OTA.

## Notes

1. Written in C using Espressif ESP-IDF v5.0 for and tested on the WT32-ETH01 (ESP32+LAN8720) module.
2. To restart the device, send the "restart" command to the root topic of the device (example - "homeassistant/gateway/70-03-9F-44-BE-F7").
3. To check/update the device firmware, send the "update" command to the root topic of the device (example - "homeassistant/gateway/70-03-9F-44-BE-F7"). The best way is to create a Home Assistant automation to periodically check for firmware updates.

## Attention

1. ESP-NOW network name must be set same of all another ESP-NOW devices in network (can be changed in zh_config.h).
2. If using a WiFi connection, the WiFi router must be set to channel 1.

## Supported devices

1. [zh_espnow_switch_esp32](https://github.com/aZholtikov/zh_espnow_switch_esp32)
2. [zh_espnow_switch_esp8266](https://github.com/aZholtikov/zh_espnow_switch_esp8266)
