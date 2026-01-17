#ifndef EEPROM_SETTINGS_H
#define EEPROM_SETTINGS_H

#include "quantum.h"

// Define the User Config Union
typedef union {
    uint8_t raw[USER_CONFIG_SIZE];
    struct {
        HSV caps_lock_hs; // Hue & Saturation for Caps Lock
        HSV layer1_hs;    // Hue & Saturation for Layer 1
        HSV layer2_hs;    // Hue & Saturation for Layer 2
        uint8_t caps_lock_br;
        uint8_t layer1_br;
        uint8_t layer2_br;
        bool caps_lock_ind;
        bool layer1_ind;
        bool layer2_ind;
    } __attribute__((packed)); // Ensure no padding
} user_config_t;

// Global Variable
extern user_config_t user_config;

// VIA Command Handling
void via_custom_value_command_kb(uint8_t *data, uint8_t length);
void user_config_set_value(uint8_t *data);
void user_config_get_value(uint8_t *data);
void load_user_config(void);
void save_user_config(void);

#endif // EEPROM_SETTINGS_H
