// Copyright 2024 yangzheng20003 (@yangzheng20003)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "m1_v5_us.h"

#include "module.h"
#include "wls/wls.h"

#ifdef WIRELESS_ENABLE
#    include "wireless.h"
#    include "usb_main.h"
#    include "lowpower.h"
#endif

typedef union {
    uint32_t raw;
    struct {
        uint8_t flag : 1; // used to make sure the eeprom has initialized at least once
        uint8_t current_dev : 3;
        uint8_t last_bt_dev : 3;
    };
} confinfo_t;
confinfo_t confinfo;

enum layers {
    _BL = 0,
    _FL,
    _MBL,
    _MFL,
    _FBL,
};

#define keymap_is_mac_system() ((get_highest_layer(default_layer_state) == _MBL) || (get_highest_layer(default_layer_state) == _MFL))
#define keymap_is_base_layer() ((get_highest_layer(default_layer_state) == _BL) || (get_highest_layer(default_layer_state) == _FL))

uint32_t post_init_timer   = 0x00;
bool     charging_state    = false;
bool     battery_full_flag = false;
HSV      start_hsv;
bool     lower_sleep = false;

void keyboard_post_init_kb(void) {
#ifdef CONSOLE_ENABLE
    debug_enable = true;
#endif

    confinfo.raw = eeconfig_read_kb();
    if (!confinfo.raw) {
        confinfo.flag        = true;
        confinfo.last_bt_dev = 1;

        eeconfig_init_user();
        eeconfig_update_kb(confinfo.raw);
    }

#ifdef LED_POWER_EN_PIN
    gpio_set_pin_output(LED_POWER_EN_PIN);
    gpio_write_pin_high(LED_POWER_EN_PIN);

    gpio_set_pin_output(HS_LED_BOOSTING_PIN);
    gpio_write_pin_high(HS_LED_BOOSTING_PIN);
#endif

#ifdef MM_BT_DEF_PIN
    gpio_set_pin_input_high(MM_BT_DEF_PIN);
#endif

#ifdef MM_2G4_DEF_PIN
    gpio_set_pin_input_high(MM_2G4_DEF_PIN);
#endif

#ifdef USB_POWER_EN_PIN
    gpio_write_pin_low(USB_POWER_EN_PIN);
    gpio_set_pin_output(USB_POWER_EN_PIN);
#endif

#ifdef HS_BAT_CABLE_PIN
    gpio_set_pin_input(HS_BAT_CABLE_PIN);
#endif

#ifdef BAT_FULL_PIN
    gpio_set_pin_input_high(BAT_FULL_PIN);
#endif

    gpio_set_pin_input_high(SYSTEM_WIN_PIN);
    gpio_set_pin_input_high(SYSTEM_MAC_PIN);

#ifdef WIRELESS_ENABLE
    wireless_init();
#    if (!(defined(HS_BT_DEF_PIN) && defined(HS_2G4_DEF_PIN)))
    wireless_devs_change(!confinfo.current_dev, confinfo.current_dev, false);
#    endif
    post_init_timer = timer_read32();
#endif

    keyboard_post_init_user();

    start_hsv = rgb_matrix_get_hsv();
}

#ifdef WIRELESS_ENABLE

void usb_power_connect(void) {
#    ifdef USB_POWER_EN_PIN
    gpio_write_pin_low(USB_POWER_EN_PIN);
#    endif
}

void usb_power_disconnect(void) {
#    ifdef USB_POWER_EN_PIN
    gpio_write_pin_high(USB_POWER_EN_PIN);
#    endif
}

void suspend_power_down_kb(void) {
#    ifdef LED_POWER_EN_PIN
    gpio_write_pin_low(LED_POWER_EN_PIN);
#    endif

    suspend_power_down_user();
}

void suspend_wakeup_init_kb(void) {
#    ifdef LED_POWER_EN_PIN
    gpio_write_pin_high(LED_POWER_EN_PIN);
#    endif

    wireless_devs_change(wireless_get_current_devs(), wireless_get_current_devs(), false);
    suspend_wakeup_init_user();
    hs_rgb_blink_set_timer(timer_read32());
}

bool lpwr_is_allow_timeout_hook(void) {
    if (wireless_get_current_devs() == DEVS_USB) {
        return false;
    }

    return true;
}

void wireless_post_task(void) {
    // auto switching devs
    if (post_init_timer && timer_elapsed32(post_init_timer) >= 100) {
        md_send_devctrl(MD_SND_CMD_DEVCTRL_FW_VERSION);   // get the module fw version.
        md_send_devctrl(MD_SND_CMD_DEVCTRL_SLEEP_BT_EN);  // timeout 30min to sleep in bt mode, enable
        md_send_devctrl(MD_SND_CMD_DEVCTRL_SLEEP_2G4_EN); // timeout 30min to sleep in 2.4g mode, enable
        wireless_devs_change(!confinfo.current_dev, confinfo.current_dev, false);
        post_init_timer = 0x00;
    }

    hs_mode_scan(false, confinfo.current_dev, confinfo.last_bt_dev);
}

// i don't know what this does
void m1v5_bt_test(void) {
    md_send_devctrl(0x62);
}

static uint32_t wls_process_long_press(uint32_t trigger_time, void *cb_arg) {
    uint16_t keycode = *((uint16_t *)cb_arg);

    switch (keycode) {
        case KC_BT1: {
            uint8_t mode = confinfo.current_dev;
            hs_modeio_detection(true, &mode, confinfo.last_bt_dev);
            if ((mode == hs_bt) || (mode == hs_wireless) || (mode == hs_none)) {
                wireless_devs_change(wireless_get_current_devs(), DEVS_BT1, true);
            }

        } break;
        case KC_BT2: {
            uint8_t mode = confinfo.current_dev;
            hs_modeio_detection(true, &mode, confinfo.last_bt_dev);
            if ((mode == hs_bt) || (mode == hs_wireless) || (mode == hs_none)) {
                wireless_devs_change(wireless_get_current_devs(), DEVS_BT2, true);
            }
        } break;
        case KC_BT3: {
            uint8_t mode = confinfo.current_dev;
            hs_modeio_detection(true, &mode, confinfo.last_bt_dev);
            if ((mode == hs_bt) || (mode == hs_wireless) || (mode == hs_none)) {
                wireless_devs_change(wireless_get_current_devs(), DEVS_BT3, true);
            }
        } break;
        case KC_2G4: {
            uint8_t mode = confinfo.current_dev;
            hs_modeio_detection(true, &mode, confinfo.last_bt_dev);
            if ((mode == hs_2g4) || (mode == hs_wireless) || (mode == hs_none)) {
                wireless_devs_change(wireless_get_current_devs(), DEVS_2G4, true);
            }
        } break;
        default:
            break;
    }

    return 0;
}

static bool process_record_wls(uint16_t keycode, keyrecord_t *record) {
    static uint16_t       keycode_shadow               = 0x00;
    static deferred_token wls_process_long_press_token = INVALID_DEFERRED_TOKEN;

    keycode_shadow = keycode;

#    ifndef WLS_KEYCODE_PAIR_TIME
#        define WLS_KEYCODE_PAIR_TIME 3000
#    endif

#    define WLS_KEYCODE_EXEC(wls_dev)                                                                                          \
        do {                                                                                                                   \
            if (record->event.pressed) {                                                                                       \
                if (wireless_get_current_devs() != wls_dev) wireless_devs_change(wireless_get_current_devs(), wls_dev, false); \
                if (wls_process_long_press_token == INVALID_DEFERRED_TOKEN) {                                                  \
                    wls_process_long_press_token = defer_exec(WLS_KEYCODE_PAIR_TIME, wls_process_long_press, &keycode_shadow); \
                }                                                                                                              \
            } else {                                                                                                           \
                cancel_deferred_exec(wls_process_long_press_token);                                                            \
                wls_process_long_press_token = INVALID_DEFERRED_TOKEN;                                                         \
            }                                                                                                                  \
        } while (false)

    switch (keycode) {
        case KC_BT1: {
            uint8_t mode = confinfo.current_dev;
            hs_modeio_detection(true, &mode, confinfo.last_bt_dev);
            if ((mode == hs_bt) || (mode == hs_wireless) || (mode == hs_none)) {
                WLS_KEYCODE_EXEC(DEVS_BT1);
                hs_rgb_blink_set_timer(timer_read32());
            }

        } break;
        case KC_BT2: {
            uint8_t mode = confinfo.current_dev;
            hs_modeio_detection(true, &mode, confinfo.last_bt_dev);
            if ((mode == hs_bt) || (mode == hs_wireless) || (mode == hs_none)) {
                WLS_KEYCODE_EXEC(DEVS_BT2);
                hs_rgb_blink_set_timer(timer_read32());
            }
        } break;
        case KC_BT3: {
            uint8_t mode = confinfo.current_dev;
            hs_modeio_detection(true, &mode, confinfo.last_bt_dev);
            if ((mode == hs_bt) || (mode == hs_wireless) || (mode == hs_none)) {
                WLS_KEYCODE_EXEC(DEVS_BT3);
                hs_rgb_blink_set_timer(timer_read32());
            }
        } break;
        case KC_2G4: {
            uint8_t mode = confinfo.current_dev;
            hs_modeio_detection(true, &mode, confinfo.last_bt_dev);
            if ((mode == hs_2g4) || (mode == hs_wireless) || (mode == hs_none)) {
                WLS_KEYCODE_EXEC(DEVS_2G4);
                hs_rgb_blink_set_timer(timer_read32());
            }
        } break;

        case KC_USB: {
            uint8_t mode = confinfo.current_dev;
            hs_modeio_detection(true, &mode, confinfo.last_bt_dev);
            if ((mode == hs_2g4) || (mode == hs_wireless) || (mode == hs_none)) {
                WLS_KEYCODE_EXEC(DEVS_USB);
                hs_rgb_blink_set_timer(timer_read32());
            }
        } break;
        default:
            return true;
    }

    return false;
}
#endif

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    // prevent timeout
    if (*md_getp_state() == MD_STATE_CONNECTED) {
        hs_rgb_blink_set_timer(timer_read32());
    }

    if (process_record_user(keycode, record) != true) {
        return false;
    }

#ifdef WIRELESS_ENABLE
    if (process_record_wls(keycode, record) != true) {
        return false;
    }
#endif

    return true;
}


#ifdef RGB_MATRIX_ENABLE

#    ifdef WIRELESS_ENABLE
bool     wls_rgb_indicator_reset    = false;
uint32_t wls_rgb_indicator_timer    = 0x00;
uint32_t wls_rgb_indicator_interval = 0;
// hide connecting after this amount of time
// the keyboard does this by default, i guess to save battery?
uint32_t wls_rgb_indicator_timeout  = 5000;
uint32_t wls_rgb_indicator_index    = 0;
RGB      wls_rgb_indicator_rgb      = {0};

static void rgb_matrix_wls_indicator_set(uint8_t index, RGB rgb, uint32_t interval) {
    wls_rgb_indicator_timer = timer_read32();

    wls_rgb_indicator_index    = index;
    wls_rgb_indicator_interval = interval;
    wls_rgb_indicator_rgb      = rgb;
}

static void rgb_matrix_wls_indicator_wls(uint8_t devs) {
    uint32_t interval = wls_rgb_indicator_reset ? 200 : 500;

    switch (devs) {
        case DEVS_USB: {
            rgb_matrix_wls_indicator_set(HS_RGB_BLINK_INDEX_USB, (RGB){HS_LBACK_COLOR_USB}, interval);
        } break;
        case DEVS_BT1: {
            rgb_matrix_wls_indicator_set(HS_RGB_BLINK_INDEX_BT1, (RGB){HS_LBACK_COLOR_BT1}, interval);
        } break;
        case DEVS_BT2: {
            rgb_matrix_wls_indicator_set(HS_RGB_BLINK_INDEX_BT2, (RGB){HS_LBACK_COLOR_BT2}, interval);
        } break;
        case DEVS_BT3: {
            rgb_matrix_wls_indicator_set(HS_RGB_BLINK_INDEX_BT3, (RGB){HS_LBACK_COLOR_BT3}, interval);
        } break;
        case DEVS_2G4: {
            rgb_matrix_wls_indicator_set(HS_RGB_BLINK_INDEX_2G4, (RGB){HS_LBACK_COLOR_2G4}, interval);
        } break;
    }
}

void wireless_devs_change_kb(uint8_t old_devs, uint8_t new_devs, bool reset) {
    wls_rgb_indicator_reset = reset;

    if (confinfo.current_dev != wireless_get_current_devs()) {
        confinfo.current_dev = wireless_get_current_devs();
        if (confinfo.current_dev > 0 && confinfo.current_dev < 4) confinfo.last_bt_dev = confinfo.current_dev;
        eeconfig_update_kb(confinfo.raw);
    }

    rgb_matrix_wls_indicator_wls(new_devs);
}

void notify_usb_device_state_change_kb(struct usb_device_state usb_device_state) {
    if (usb_device_state.configure_state == USB_DEVICE_STATE_CONFIGURED) {
        wls_rgb_indicator_timer = 0x00;
    }
    notify_usb_device_state_change_user(usb_device_state);
}

bool md_receive_process_kb(uint8_t *pdata, uint8_t len) {
    switch (pdata[0]) {
        case MD_REV_CMD_DEVCTRL: {
            switch (pdata[1]) {
                case MD_REV_CMD_DEVCTRL_CONNECTED: {
                    wls_rgb_indicator_timer = 0x00;
                } break;
            }
        }
    }
    md_receive_process_user(pdata, len);
    return true;
}

static void rgb_matrix_wls_indicator(void) {
    if (wls_rgb_indicator_timer) {
        if ((timer_elapsed32(wls_rgb_indicator_timer) / wls_rgb_indicator_interval) % 2 == 0) {
            rgb_matrix_set_color(wls_rgb_indicator_index, wls_rgb_indicator_rgb.r, wls_rgb_indicator_rgb.g, wls_rgb_indicator_rgb.b);
        } else {
            rgb_matrix_set_color(wls_rgb_indicator_index, 0x00, 0x00, 0x00);
        }
    }
}

#    endif

#endif

void housekeeping_task_kb(void) { // loop
#ifdef WIRELESS_ENABLE
    wireless_housekeeping_task();
#ifdef RGB_MATRIX_ENABLE
    if (wls_rgb_indicator_timer && timer_elapsed32(wls_rgb_indicator_timer) > wls_rgb_indicator_timeout) {
        wls_rgb_indicator_timer = 0x00;
    }
#endif
#endif

    static uint32_t hs_current_time;

    charging_state    = gpio_read_pin(HS_BAT_CABLE_PIN);
    battery_full_flag = gpio_read_pin(BAT_FULL_PIN);

    if (!hs_current_time || timer_elapsed32(hs_current_time) > 1000) {
        uint8_t hs_now_mode;
        if (charging_state && battery_full_flag) {
            hs_now_mode = MD_SND_CMD_DEVCTRL_CHARGING_DONE;
        } else if (charging_state) {
            hs_now_mode = MD_SND_CMD_DEVCTRL_CHARGING;
        } else {
            hs_now_mode = MD_SND_CMD_DEVCTRL_CHARGING_STOP;
        }

        hs_current_time = timer_read32();
        md_send_devctrl(hs_now_mode);
        md_send_devctrl(MD_SND_CMD_DEVCTRL_INQVOL);
    }

    if (charging_state) {
        gpio_write_pin(HS_LED_BOOSTING_PIN, 0);

    } else {
        gpio_write_pin(HS_LED_BOOSTING_PIN, 1);
    }

    if ((gpio_read_pin(SYSTEM_WIN_PIN) != 0) && (gpio_read_pin(SYSTEM_MAC_PIN) == 0)) { // mac system
        if (!keymap_is_mac_system()) {
            set_single_persistent_default_layer(_MBL);
            layer_move(0);
        }
    } else { // win system
        if (keymap_is_mac_system()) {
            set_single_persistent_default_layer(_BL);
            layer_move(0);
        }
    }

    // converts device low power mode after 60 seconds at 0% battery
    static uint32_t battery_process_time = 0;
    if (*md_getp_bat() <= BATTERY_CAPACITY_STOP) {
        if (!battery_process_time) {
            battery_process_time = timer_read32();
        }

        if (battery_process_time && timer_elapsed32(battery_process_time) > 60000) {
            battery_process_time = 0;
            lower_sleep          = true;
            lpwr_set_timeout_manual(true);
        }
    } else {
        battery_process_time = 0;
    }

    housekeeping_task_user();
}

bool rgb_matrix_indicators_kb() {
    if (!rgb_matrix_indicators_user()) {
        return false;
    }

    if (host_keyboard_led_state().caps_lock) rgb_matrix_set_color(HS_RGB_INDEX_CAPS, 0x20, 0x20, 0x20);
    if (!keymap_is_mac_system() && keymap_config.no_gui) rgb_matrix_set_color(HS_RGB_INDEX_WIN_LOCK, 0x20, 0x20, 0x20);

#ifdef WIRELESS_ENABLE
    rgb_matrix_wls_indicator();

    if (charging_state && !battery_full_flag) {
        rgb_matrix_set_color(HS_MATRIX_BLINK_INDEX_BAT, 0x00, 0xFF, 0x00);
    } else if (*md_getp_bat() <= BATTERY_CAPACITY_LOW) {
        static uint32_t battery_flash_timer;

        if (!battery_flash_timer || timer_elapsed32(battery_flash_timer) > 500) {
            battery_flash_timer = timer_read32();
        }

        if (timer_elapsed32(battery_flash_timer) <= 250) {
            rgb_matrix_set_color(HS_MATRIX_BLINK_INDEX_BAT, 0xFF, 0x00, 0x00);
        } else {
            rgb_matrix_set_color(HS_MATRIX_BLINK_INDEX_BAT, 0x00, 0x00, 0x00);
        }
    }
#endif

    return true;
}

void lpwr_wakeup_hook(void) {
    hs_mode_scan(false, confinfo.current_dev, confinfo.last_bt_dev);

    gpio_write_pin_high(LED_POWER_EN_PIN);
    gpio_write_pin_high(HS_LED_BOOSTING_PIN);
}
