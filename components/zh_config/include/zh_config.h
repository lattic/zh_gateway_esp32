#ifndef ZH_CONFIG_H
#define ZH_CONFIG_H

#include "stdint.h"
#include "stdbool.h"

#define ESPNOW_NET_NAME "ESPNOW" // Up to 9 characters.

#define HIGH true
#define LOW false

#define ON true
#define OFF false

#define ONLINE true
#define OFFLINE false

#define NOT_USED 0xFF

#define ZH_DEVICE_TYPE                             \
    DF(ZHDT_NONE, "")                              \
    DF(ZHDT_GATEWAY, "gateway")                    \
    DF(ZHDT_SWITCH, "espnow_switch")               \
    DF(ZHDT_LED, "espnow_led")                     \
    DF(ZHDT_SENSOR, "espnow_sensor")               \
    DF(ZHDT_RF_GATEWAY, "espnow_rf_gateway")       \
    DF(ZHDT_CONTROL_PANEL, "espnow_control_panel") \
    DF(ZHDT_DIMMER, "espnow_dimmer")               \
    DF(ZHDT_TERRARIUM, "espnow_terrarium")         \
    DF(ZHDT_RF_SENSOR, "rf_sensor")                \
    DF(ZHDT_MAX, "")

typedef enum zh_device_type_t
{
#define DF(_value, _name) _value,
    ZH_DEVICE_TYPE
#undef DF
} zh_device_type_t;

char *get_device_type_value_name(zh_device_type_t value);

#define ZH_PAYLOAD_TYPE               \
    DF(ZHPT_NONE, "")                 \
    DF(ZHPT_ATTRIBUTES, "attributes") \
    DF(ZHPT_KEEP_ALIVE, "status")     \
    DF(ZHPT_SET, "")                  \
    DF(ZHPT_STATE, "state")           \
    DF(ZHPT_UPDATE, "")               \
    DF(ZHPT_RESTART, "")              \
    DF(ZHPT_SYSTEM, "")               \
    DF(ZHPT_CONFIG, "config")         \
    DF(ZHPT_FORWARD, "")              \
    DF(ZHPT_MAX, "")

typedef enum zh_payload_type_t
{
#define DF(_value, _name) _value,
    ZH_PAYLOAD_TYPE
#undef DF
} zh_payload_type_t;

char *get_payload_type_value_name(zh_payload_type_t value);

#define HA_COMPONENT_TYPE                               \
    DF(HACT_NONE, "")                                   \
    DF(HACT_ALARM_CONTROL_PANEL, "alarm_control_panel") \
    DF(HACT_BINARY_SENSOR, "binary_sensor")             \
    DF(HACT_BUTTON, "button")                           \
    DF(HACT_CAMERA, "camera")                           \
    DF(HACT_COVER, "cover")                             \
    DF(HACT_DEVICE_TRACKER, "device_tracker")           \
    DF(HACT_FAN, "fan")                                 \
    DF(HACT_HUMIDIFIER, "humidifier")                   \
    DF(HACT_CLIMATE_HVAC, "climate")                    \
    DF(HACT_LIGHT, "light")                             \
    DF(HACT_LOCK, "lock")                               \
    DF(HACT_NUMBER, "number")                           \
    DF(HACT_SCENE, "scene")                             \
    DF(HACT_SELECT, "select")                           \
    DF(HACT_SENSOR, "sensor")                           \
    DF(HACT_SIREN, "siren")                             \
    DF(HACT_SWITCH, "switch")                           \
    DF(HACT_UPDATE, "update")                           \
    DF(HACT_TEXT, "text")                               \
    DF(HACT_VACUUM, "vacuum")

typedef enum ha_component_type_t
{
#define DF(_value, _name) _value,
    HA_COMPONENT_TYPE
#undef DF
} ha_component_type_t;

#define HA_BINARY_SENSOR_DEVICE_CLASS               \
    DF(HABSDC_NONE, "")                             \
    DF(HABSDC_BATTERY, "battery")                   \
    DF(HABSDC_BATTERY_CHARGING, "battery_charging") \
    DF(HABSDC_CARBON_MONOXIDE, "carbon_monoxide")   \
    DF(HABSDC_COLD, "cold")                         \
    DF(HABSDC_CONNECTIVITY, "connectivity")         \
    DF(HABSDC_DOOR, "door")                         \
    DF(HABSDC_GARAGE_DOOR, "garage_door")           \
    DF(HABSDC_GAS, "gas")                           \
    DF(HABSDC_HEAT, "heat")                         \
    DF(HABSDC_LIGHT, "light")                       \
    DF(HABSDC_LOCK, "lock")                         \
    DF(HABSDC_MOISTURE, "moisture")                 \
    DF(HABSDC_MOTION, "motion")                     \
    DF(HABSDC_MOVING, "moving")                     \
    DF(HABSDC_OCCUPANCY, "occupancy")               \
    DF(HABSDC_OPENING, "opening")                   \
    DF(HABSDC_PLUG, "plug")                         \
    DF(HABSDC_POWER, "power")                       \
    DF(HABSDC_PRESENCE, "presence")                 \
    DF(HABSDC_PROBLEM, "problem")                   \
    DF(HABSDC_RUNNING, "running")                   \
    DF(HABSDC_SAFETY, "safety")                     \
    DF(HABSDC_SMOKE, "smoke")                       \
    DF(HABSDC_SOUND, "sound")                       \
    DF(HABSDC_TAMPER, "tamper")                     \
    DF(HABSDC_UPDATE, "update")                     \
    DF(HABSDC_VIBRATION, "vibration")               \
    DF(HABSDC_WINDOW, "window")

typedef enum ha_binary_sensor_device_class_t
{
#define DF(_value, _name) _value,
    HA_BINARY_SENSOR_DEVICE_CLASS
#undef DF
} ha_binary_sensor_device_class_t;

#define HA_COVER_DEVICE_CLASS    \
    DF(HACDC_NONE, "")           \
    DF(HACDC_AWNING, "awning")   \
    DF(HACDC_BLIND, "blind")     \
    DF(HACDC_CURTAIN, "curtain") \
    DF(HACDC_DAMPER, "damper")   \
    DF(HACDC_DOOR, "door")       \
    DF(HACDC_GARAGE, "garage")   \
    DF(HACDC_GATE, "gate")       \
    DF(HACDC_SHADE, "shade")     \
    DF(HACDC_SHUTTER, "shutter") \
    DF(HACDC_WINDOW, "window")

typedef enum ha_cover_device_class_t
{
#define DF(_value, _name) _value,
    HA_COVER_DEVICE_CLASS
#undef DF
} ha_cover_device_class_t;

#define HA_SENSOR_DEVICE_CLASS                                         \
    DF(HASDC_NONE, "")                                                 \
    DF(HASDC_APPARENT_POWER, "apparent_power")                         \
    DF(HASDC_AQI, "aqi")                                               \
    DF(HASDC_BATTERY, "battery")                                       \
    DF(HASDC_CARBON_DIOXIDE, "carbon_dioxide")                         \
    DF(HASDC_CARBON_MONOXIDE, "carbon_monoxide")                       \
    DF(HASDC_CURRENT, "current")                                       \
    DF(HASDC_DATE, "date")                                             \
    DF(HASDC_DISTANCE, "distance")                                     \
    DF(HASDC_DURATION, "duration")                                     \
    DF(HASDC_ENERGY, "energy")                                         \
    DF(HASDC_FREQUENCY, "frequency")                                   \
    DF(HASDC_GAS, "gas")                                               \
    DF(HASDC_HUMIDITY, "humidity")                                     \
    DF(HASDC_ILLUMINANCE, "illuminance")                               \
    DF(HASDC_MOISTURE, "moisture")                                     \
    DF(HASDC_MONETARY, "monetar")                                      \
    DF(HASDC_NITROGEN_DIOXIDE, "nitrogen_dioxide")                     \
    DF(HASDC_NITROGEN_MONOXIDE, "nitrogen_monoxide")                   \
    DF(HASDC_NITROUS_OXIDE, "nitrous_oxide")                           \
    DF(HASDC_OZONE, "ozone")                                           \
    DF(HASDC_PM1, "pm1")                                               \
    DF(HASDC_PM10, "pm10")                                             \
    DF(HASDC_PM25, "pm25")                                             \
    DF(HASDC_POWER_FACTOR, "power_factor")                             \
    DF(HASDC_POWER, "power")                                           \
    DF(HASDC_PRECIPITATION, "precipitation")                           \
    DF(HASDC_PRECIPITATION_INTENSITY, "precipitation_intensity")       \
    DF(HASDC_PRESSURE, "pressure")                                     \
    DF(HASDC_REACTIVE_POWER, "reactive_power")                         \
    DF(HASDC_SIGNAL_STRENGTH, "signal_strength")                       \
    DF(HASDC_SPEED, "speed")                                           \
    DF(HASDC_SULPHUR_DIOXIDE, "sulphur_dioxide")                       \
    DF(HASDC_TEMPERATURE, "temperature")                               \
    DF(HASDC_TIMESTAMP, "timestamp")                                   \
    DF(HASDC_VOLATILE_ORGANIC_COMPOUNDS, "volatile_organic_compounds") \
    DF(HASDC_VOLTAGE, "voltage")                                       \
    DF(HASDC_VOLUME, "volume")                                         \
    DF(HASDC_WATER, "water")                                           \
    DF(HASDC_WEIGHT, "weight")                                         \
    DF(HASDC_WIND_SPEED, "wind_speed")

typedef enum ha_sensor_device_class_t
{
#define DF(_value, _name) _value,
    HA_SENSOR_DEVICE_CLASS
#undef DF
} ha_sensor_device_class_t;

#define HA_SWITCH_DEVICE_CLASS  \
    DF(HASWDC_NONE, "")         \
    DF(HASWDC_OUTLET, "outlet") \
    DF(HASWDC_SWITCH, "switch")

typedef enum ha_switch_device_class_t
{
#define DF(_value, _name) _value,
    HA_SWITCH_DEVICE_CLASS
#undef DF
} ha_switch_device_class_t;

#define HA_PAYLOAD_TYPE \
    DF(HAPT_NONE, "")   \
    DF(HAPT_ON, "ON")   \
    DF(HAPT_OFF, "OFF")

typedef enum ha_payload_type_t
{
#define DF(_value, _name) _value,
    HA_PAYLOAD_TYPE
#undef DF
} ha_payload_type_t;

#define HA_CHIP_TYPE         \
    DF(HACHT_NONE, "")       \
    DF(HACHT_ESP32, "ESP32") \
    DF(HACHT_ESP8266, "ESP8266")

typedef enum ha_chip_type_t
{
#define DF(_value, _name) _value,
    HA_CHIP_TYPE
#undef DF
} ha_chip_type_t;

char *get_chip_type_value_name(ha_chip_type_t value);
//***********************************************************************************//
typedef struct zh_switch_config_message_t
{
    uint8_t unique_id;
    ha_switch_device_class_t device_class;
    ha_payload_type_t payload_on;
    ha_payload_type_t payload_off;
    bool enabled_by_default;
    bool optimistic;
    uint8_t qos;
    bool retain;
} __attribute__((packed)) zh_switch_config_message_t;
//***********************************************************************************//
typedef struct zh_led_status_message_t
{
    bool status;
    uint8_t led_brightness;
    uint16_t color_temperature;
    uint8_t rgb_colors[3];
} __attribute__((packed)) zh_led_status_message_t;

typedef struct zh_switch_status_message_t
{
    bool status;
} __attribute__((packed)) zh_switch_status_message_t;
//***********************************************************************************//
typedef struct zh_attributes_message_t
{
    ha_chip_type_t chip_type;
    char app_name[32];
    char app_version[32];
    uint32_t uptime;
} __attribute__((packed)) zh_attributes_message_t;

typedef struct zh_keep_alive_message_t
{
    bool online_status;
    uint8_t message_frequency;
} __attribute__((packed)) zh_keep_alive_message_t;

typedef union
{
    zh_switch_config_message_t switch_config_message;
} zh_config_message_t;

typedef union zh_status_message_t
{
    zh_led_status_message_t led_status_message;
    zh_switch_status_message_t switch_status_message;
} zh_status_message_t;

typedef union
{
    zh_attributes_message_t attributes_message;
    zh_keep_alive_message_t keep_alive_message;
    zh_config_message_t config_message;
    zh_status_message_t status_message;
} zh_payload_data_t;

typedef struct zh_espnow_data_t
{
    char net_name[10];
    zh_device_type_t device_type;
    zh_payload_type_t payload_type;
    zh_payload_data_t payload_data;
} __attribute__((packed)) zh_espnow_data_t;

#endif