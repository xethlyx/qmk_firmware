#include "eeprom_settings.h"
#include "eeprom.h"
#include "raw_hid.h"
#include "print.h"

user_config_t user_config; // Global configuration struct

enum via_value {
    id_capslock      = 1,
    id_layer1        = 2,
    id_layer2        = 3,
    id_capslock_br   = 4,
    id_layer1_br     = 5,
    id_layer2_br     = 6,
    id_capslock_ind  = 7,
    id_layer1_ind    = 8,
    id_layer2_ind    = 9
};

void via_custom_value_command_kb(uint8_t *data, uint8_t length) {
    // data = [ command_id, channel_id, value_id, value_data ]
    uint8_t *command_id        = &(data[0]);
    uint8_t *channel_id        = &(data[1]);
    uint8_t *value_id_and_data = &(data[2]);

    if ( *channel_id == id_custom_channel ) {
        switch (*command_id) {
            case id_custom_set_value:
            {
                user_config_set_value(value_id_and_data);
                break;
            }
            case id_custom_get_value:
            {
                user_config_get_value(value_id_and_data);
                break;
            }
            case id_custom_save:
            {
                eeconfig_update_user_datablock(&user_config); // Save to EEPROM
                break;
            }
            default:
            {
                // Unhandled message.
                *command_id = id_unhandled;
                break;
            }
        }
        return;
    }

    *command_id = id_unhandled;
}


// VIA Command Handling
void  user_config_set_value(uint8_t *data) {

    uint8_t *value_id = &(data[0]);  // Get the value ID
    HSV *hsv_data   = (HSV *)&(data[1]);  // Point to HSV structure

    switch (*value_id) {
        case id_capslock:
            user_config.caps_lock_hs = *hsv_data;
            break;
        case id_layer1:
            user_config.layer1_hs = *hsv_data;
            break;
        case id_layer2:
            user_config.layer2_hs = *hsv_data;
            break;
        case id_capslock_br:
            user_config.caps_lock_br = data[1];
            break;
        case id_layer1_br:
            user_config.layer1_br = data[1];
            break;
        case id_layer2_br:
            user_config.layer2_br = data[1];
            break;
        case id_capslock_ind:
            user_config.caps_lock_ind = data[1];
            break;
        case id_layer1_ind:
            user_config.layer1_ind = data[1];
            break;
        case id_layer2_ind:
            user_config.layer2_ind = data[1];
            break;
        default:
            return; // Unknown value ID, do nothing
    }
}

void user_config_get_value(uint8_t *data) {
    // data = [ value_id, R, G, B ]
    uint8_t *value_id = &(data[0]);
    HSV *hsv_data   = (HSV *)&(data[1]);

    switch (*value_id) {
        case id_capslock:
            *hsv_data = user_config.caps_lock_hs;
            break;
        case id_layer1:
            *hsv_data = user_config.layer1_hs;
            break;
        case id_layer2:
            *hsv_data = user_config.layer2_hs;
            break;
        case id_capslock_br:
            data[1] = user_config.caps_lock_br;
            break;
        case id_layer1_br:
            data[1] = user_config.layer1_br;
            break;
        case id_layer2_br:
            data[1] = user_config.layer2_br;
            break;
        case id_capslock_ind:
            data[1] = user_config.caps_lock_ind;
            break;
        case id_layer1_ind:
            data[1] = user_config.layer1_ind;
            break;
        case id_layer2_ind:
            data[1] = user_config.layer2_ind;
            break;
    }
}

void load_user_config(void) {
    eeprom_read_block(&user_config, (void *)USER_CONFIG_EEPROM_ADDR, sizeof(user_config));
}

void save_user_config(void) {
    eeprom_update_block(&user_config, (void *)USER_CONFIG_EEPROM_ADDR, sizeof(user_config));
}
