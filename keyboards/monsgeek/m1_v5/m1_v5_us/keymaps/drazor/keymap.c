// Copyright 2024 yangzheng20003 (@yangzheng20003)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "config.h"
#include "eeprom_settings.h"
#include "eeprom.h"
#include "rgb_matrix.h"
#include "module.h"
#include "wls/wls.h"

enum layers {
    _BL = 0,
    _FL,
    _MBL,
    _MFL,
    _FBL,
    _NL,
};

enum custom_keycodes {
    HS_BATQ = SAFE_RANGE,
    KEEP_AWAKE,
};

enum combos {
    NLAYER_TOGGLE_COMBO,
    // other combos
};


// These macros must be defined in a shared file or directly here
uint8_t caps_lock_letter_list[] = CAPS_LOCK_LETTER_LIST;
uint8_t number_list[] = NUMBER_LIST;
uint8_t func_list[]   = FUNC_LIST;
#define CAPS_LOCK_LETTER_COUNT (sizeof(caps_lock_letter_list)/sizeof(caps_lock_letter_list[0]))
#define NUMBER_LIST_COUNT (sizeof(number_list)/sizeof(number_list[0]))
#define FUNC_LIST_COUNT   (sizeof(func_list)/sizeof(func_list[0]))


#define ______ HS_BLACK

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_BL] = LAYOUT( /* Base */
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_DEL,   KC_MUTE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_HOME,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_PGUP,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,   KC_PGDN,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,  KC_UP,    KC_END,
        KC_LCTL,  KC_LCMD,  KC_LALT,                      KC_SPC,                                 KC_RALT,  MO(_FL),  KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [_FL] = LAYOUT( /* Function Layer */
        QK_BOOT,  KC_MYCM,  KC_MAIL,  KC_WSCH,  KC_WHOM,  KC_MSEL,  KC_MPLY,  KC_MPRV,  KC_MNXT,  AC_TOGG,  KC_TRNS,  KC_TRNS,  KC_TRNS,  RGB_MOD,  KC_TRNS,
        EE_CLR,   KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  RGB_SPD,  RGB_SPI,  KC_TRNS,  KC_TRNS,
        QK_RBT,  KC_TRNS,  KC_TRNS,   KC_BT1,   KC_BT2,   KC_BT3,   KC_2G4,   KC_USB,   KC_INS,   KC_TRNS,  KC_PSCR,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
        KC_CAPS,  KC_TRNS, KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,  KEEP_AWAKE,  KC_TRNS,  RGB_TOG,  KC_TRNS,  KC_TRNS,            KC_TRNS,  KC_TRNS,
        KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_CALC,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_TRNS,  KC_TRNS, RGB_VAI,  KC_TRNS,
        KC_TRNS,   GU_TOGG,  KC_TRNS,                     HS_BATQ,                                KC_TRNS,  KC_TRNS,  KC_TRNS,  RGB_SAI,  RGB_VAD,  RGB_SAD),

    [_MBL] = LAYOUT( /* Base (Mac) */
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_DEL,   KC_MUTE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_HOME,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_PGUP,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,   KC_PGDN,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,  KC_UP,    KC_END,
        KC_LCTL,  KC_LALT,  KC_LCMD,                      KC_SPC,                                 KC_RCMD,  MO(_MFL), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [_MFL] = LAYOUT( /* Function Layer (Mac) */
        KC_TRNS,  KC_BRID,  KC_BRIU,  KC_MCTL,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  RGB_MOD,  KC_TRNS,
        EE_CLR,   KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  RGB_SPD,  RGB_SPI,  KC_TRNS,  KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_BT1,   KC_BT2,   KC_BT3,   KC_2G4,   KC_USB,   KC_INS,   KC_TRNS,  KC_PSCR,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
        KC_CAPS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KEEP_AWAKE,  KC_TRNS,  RGB_TOG,  KC_TRNS,  KC_TRNS,            KC_TRNS,  KC_TRNS,
        KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_CALC,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_TRNS,  MO(_FBL), RGB_VAI,  KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,                      HS_BATQ,                                KC_TRNS,  KC_TRNS,  KC_TRNS,  RGB_SAI,  RGB_VAD,  RGB_SAD),

    [_NL] = LAYOUT( /* Number Layer */
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
        KC_TRNS,  KC_KP_1,  KC_KP_2,  KC_KP_3,  KC_KP_4,  KC_KP_5,  KC_KP_6,  KC_KP_7,  KC_KP_8,  KC_KP_9,  KC_KP_0,  KC_KP_MINUS, KC_KP_PLUS, KC_BSPC,  KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_KP_SLASH, KC_KP_ASTERISK, KC_TRNS,  KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,  KC_TRNS,
        KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,                      KC_TRNS,                                KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS)

};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [1] = {ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    [2] = {ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    [3] = {ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    [4] = {ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    [5] = {ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
};
#endif

// clang-format on

bool rk_bat_req_flag;

#define KEEP_AWAKE_KEY KC_LSFT
#define KEEP_AWAKE_INTERVAL 5000
uint32_t keep_awake_timer;
bool     keep_awake_pressed = false;

// Define the combo: RALT + HOME
const uint16_t PROGMEM nl_combo[] = {KC_RALT, KC_HOME, COMBO_END};

combo_t key_combos[] = {
    [NLAYER_TOGGLE_COMBO] = COMBO_ACTION(nl_combo),
};

void process_combo_event(uint16_t combo_index, bool pressed) {
    switch(combo_index) {
        case NLAYER_TOGGLE_COMBO:
            if (pressed) {
                layer_invert(_NL);
            }
            break;
    }
}

uint32_t keep_awake_callback(uint32_t trigger_time, void *cb_arg) {
    if (keep_awake_pressed) {
        unregister_code(KEEP_AWAKE_KEY);
    } else {
        register_code(KEEP_AWAKE_KEY);
    }

    // prevent timeout
    if (*md_getp_state() == MD_STATE_CONNECTED) {
        hs_rgb_blink_set_timer(timer_read32());
    }

    keep_awake_pressed = !keep_awake_pressed;
    return 5000;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case AC_TOGG:
                autocorrect_toggle();
                return false;

            case KEEP_AWAKE: {
                static deferred_token delayed_exec = INVALID_DEFERRED_TOKEN;

                if (record->event.pressed) {
                    if (keep_awake_timer) {
                        cancel_deferred_exec(delayed_exec);
                        if (keep_awake_pressed) {
                            unregister_code(KEEP_AWAKE_KEY);
                            keep_awake_pressed = false;
                        }
                        keep_awake_timer = 0;
                        delayed_exec     = INVALID_DEFERRED_TOKEN;
                    } else {
                        delayed_exec     = defer_exec(KEEP_AWAKE_INTERVAL, keep_awake_callback, NULL);
                        if (delayed_exec != INVALID_DEFERRED_TOKEN) keep_awake_timer = timer_read();
                    }
                    return false;
                }

                return false;
            } break;

            case HS_BATQ: {
                if (record->event.pressed) {
                    rk_bat_req_flag = !rk_bat_req_flag;
                }
                return false;
            } break;
        }
    }

    return true;
}

bool rgb_matrix_indicators_user() {
    if (rk_bat_req_flag) {
        rgb_matrix_set_color_all(0x00, 0x00, 0x00);
        for (uint8_t i = 0; i < 10; i++) {
            uint8_t mi_index[10] = RGB_MATRIX_BAT_INDEX_MAP;
            if ((i < (*md_getp_bat() / 10)) || (i < 1)) {
                if (*md_getp_bat() >= (IM_BAT_REQ_LEVEL1_VAL)) {
                    rgb_matrix_set_color(mi_index[i], IM_BAT_REQ_LEVEL1_COLOR);
                } else if (*md_getp_bat() >= (IM_BAT_REQ_LEVEL2_VAL)) {
                    rgb_matrix_set_color(mi_index[i], IM_BAT_REQ_LEVEL2_COLOR);
                } else {
                    rgb_matrix_set_color(mi_index[i], IM_BAT_REQ_LEVEL3_COLOR);
                }
            } else {
                rgb_matrix_set_color(mi_index[i], 0x00, 0x00, 0x00);
            }
        }
    }

    if (keep_awake_timer) {
        if (timer_elapsed(keep_awake_timer) / 500 % 2 == 0) {
            rgb_matrix_set_color(59, 0xFF, 0xFF, 0xFF);
        } else {
            rgb_matrix_set_color(59, 0x00, 0x00, 0x00);
        }
    }

    if (host_keyboard_led_state().caps_lock && user_config.caps_lock_ind) {
        HSV hsv = user_config.caps_lock_hs;
        hsv.v = user_config.caps_lock_br;
        RGB rgb = hsv_to_rgb(hsv);

        for (uint8_t i = 0; i < ARRAY_SIZE(caps_lock_letter_list); i++) {
            rgb_matrix_set_color(caps_lock_letter_list[i], rgb.r, rgb.g, rgb.b);
        }
    }

    if ((IS_LAYER_ON(_FL) || IS_LAYER_ON(_MFL)) && user_config.layer1_ind) {
        HSV hsv = user_config.layer1_hs;
        hsv.v = user_config.layer1_br;
        RGB rgb = hsv_to_rgb(hsv);

        for (uint8_t j = 0; j < ARRAY_SIZE(func_list); j++) {
            rgb_matrix_set_color(func_list[j], rgb.r, rgb.g, rgb.b);
        }
    }

    if ((IS_LAYER_ON(_NL)) && user_config.layer2_ind) {
        HSV hsv = user_config.layer2_hs;
        hsv.v = user_config.layer2_br;
        RGB rgb = hsv_to_rgb(hsv);

        for (uint8_t j = 0; j < ARRAY_SIZE(number_list); j++) {
            rgb_matrix_set_color(number_list[j], rgb.r, rgb.g, rgb.b);
        }
    }

    return true;
}

void keyboard_post_init_user(void) {
    eeconfig_read_user_datablock(&user_config);
}

