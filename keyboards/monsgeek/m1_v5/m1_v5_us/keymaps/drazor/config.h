// -------------------------
// EEPROM Config Definitions
// -------------------------

// Total size your config struct uses — adjust if fields are added
#define USER_CONFIG_SIZE 16
#define EECONFIG_USER_DATA_SIZE 16

// Start your block *after* all manufacturer-defined data
#define EECONFIG_USER_CONFIG_OFFSET (EECONFIG_USER_DATA_SIZE)

// Final EEPROM address offset
#define USER_CONFIG_EEPROM_ADDR ((uint32_t)EECONFIG_USER_DATABLOCK + EECONFIG_USER_CONFIG_OFFSET)

// -------------------------
// LED Index Lists (customize as needed)
// -------------------------

// DELETE = 10
// ESC    = 23
// NUM 1  = 25
// +      = 36
// P      = 41
// Q      = 50
// A      = 53
// L      = 61
// Z      = 76
// M      = 70
#define FUNC_LIST             { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 }
#define NUMBER_LIST           { 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 40, 39 }

#define CAPS_LOCK_LETTER_LIST { 50,49,48,47,46,45,44,43,42,41, \
                                53,54,55,56,57,58,59,60,61, \
                                76,75,74,73,72,71,70 }




#undef DYNAMIC_KEYMAP_LAYER_COUNT
#define DYNAMIC_KEYMAP_LAYER_COUNT 6
