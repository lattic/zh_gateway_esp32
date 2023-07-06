#include "zh_config.h"

char *get_device_type_value_name(zh_device_type_t value)
{
    switch (value)
    {
#define DF(_value, _name) \
    case _value:          \
        return _name;
        ZH_DEVICE_TYPE
#undef DF
        break;
    default:
        break;
    }
    return "";
}

char *get_switch_device_class_value_name(ha_switch_device_class_t value)
{
    switch (value)
    {
#define DF(_value, _name) \
    case _value:          \
        return _name;
        HA_SWITCH_DEVICE_CLASS
#undef DF
        break;
    default:
        break;
    }
    return "";
}

char *get_payload_type_value_name(zh_payload_type_t value)
{
    switch (value)
    {
#define DF(_value, _name) \
    case _value:          \
        return _name;
        ZH_PAYLOAD_TYPE
#undef DF
        break;
    default:
        break;
    }
    return "";
}

char *get_on_off_type_value_name(ha_on_off_type_t value)
{
    switch (value)
    {
#define DF(_value, _name) \
    case _value:          \
        return _name;
        HA_ON_OFF_TYPE
#undef DF
        break;
    default:
        break;
    }
    return "";
}

char *get_chip_type_value_name(ha_chip_type_t value)
{
    switch (value)
    {
#define DF(_value, _name) \
    case _value:          \
        return _name;
        HA_CHIP_TYPE
#undef DF
        break;
    default:
        break;
    }
    return "";
}