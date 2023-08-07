#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "eeprom.h"
#include "eeconfig.h"
#include "action_layer.h"

#if defined(EEPROM_DRIVER)
#    include "eeprom_driver.h"
#endif

#if defined(HAPTIC_ENABLE)
#    include "haptic.h"
#endif

#if defined(VIA_ENABLE)
bool via_eeprom_is_valid(void);
void via_eeprom_set_valid(bool valid);
void eeconfig_init_via(void);
#endif

/** \brief eeconfig enable
 *
 * FIXME: needs doc
 */
__attribute__((weak)) void eeconfig_init_user(void) {
#if (EECONFIG_USER_DATA_SIZE) == 0
    // Reset user EEPROM value to blank, rather than to a set value
    eeconfig_update_user(0,0,0);
#endif
}

__attribute__((weak)) void eeconfig_init_kb(void) {
#if (EECONFIG_KB_DATA_SIZE) == 0
    // Reset Keyboard EEPROM value to blank, rather than to a set value
    eeconfig_update_kb(0);
#endif

    eeconfig_init_user();
}

/*
 * FIXME: needs doc
 */
void eeconfig_init_quantum(void) {
#if defined(EEPROM_DRIVER)
    eeprom_driver_erase();
#endif
    eeprom_update_word(EECONFIG_MAGIC, EECONFIG_MAGIC_NUMBER);
    eeprom_update_byte(EECONFIG_DEBUG, 0);
    eeprom_update_byte(EECONFIG_DEFAULT_LAYER, 0);
    default_layer_state = 0;
    // Enable oneshot and autocorrect by default: 0b0001 0100 0000 0000
    eeprom_update_word(EECONFIG_KEYMAP, 0x1400);
    eeprom_update_byte(EECONFIG_BACKLIGHT, 0);
    eeprom_update_byte(EECONFIG_AUDIO, 0xFF); // On by default
    eeprom_update_dword(EECONFIG_RGBLIGHT, 0);
    eeprom_update_byte(EECONFIG_STENOMODE, 0);
    eeprom_update_dword(EECONFIG_HAPTIC, 0);
    eeprom_update_byte(EECONFIG_VELOCIKEY, 0);
    eeprom_update_dword(EECONFIG_RGB_MATRIX, 0);
    eeprom_update_word(EECONFIG_RGB_MATRIX_EXTENDED, 0);

#if defined(HAPTIC_ENABLE)
    haptic_reset();
#else
    // this is used in case haptic is disabled, but we still want sane defaults
    // in the haptic configuration eeprom. All zero will trigger a haptic_reset
    // when a haptic-enabled firmware is loaded onto the keyboard.
    eeprom_update_dword(EECONFIG_HAPTIC, 0);
#endif

#if (EECONFIG_KB_DATA_SIZE) > 0
    eeconfig_init_kb_datablock();
#endif

#if (EECONFIG_USER_DATA_SIZE) > 0
    eeconfig_init_user_datablock();
#endif

#if defined(VIA_ENABLE)
    // Invalidate VIA eeprom config, and then reset.
    // Just in case if power is lost mid init, this makes sure that it pets
    // properly re-initialized.
    via_eeprom_set_valid(false);
    eeconfig_init_via();
#endif

    eeconfig_init_kb();
}

/** \brief eeconfig initialization
 *
 * FIXME: needs doc
 */
void eeconfig_init(void) {
    eeconfig_init_quantum();
}

/** \brief eeconfig enable
 *
 * FIXME: needs doc
 */
void eeconfig_enable(void) {
    eeprom_update_word(EECONFIG_MAGIC, EECONFIG_MAGIC_NUMBER);
}

/** \brief eeconfig disable
 *
 * FIXME: needs doc
 */
void eeconfig_disable(void) {
#if defined(EEPROM_DRIVER)
    eeprom_driver_erase();
#endif
    eeprom_update_word(EECONFIG_MAGIC, EECONFIG_MAGIC_NUMBER_OFF);
}

/** \brief eeconfig is enabled
 *
 * FIXME: needs doc
 */
bool eeconfig_is_enabled(void) {
    bool is_eeprom_enabled = (eeprom_read_word(EECONFIG_MAGIC) == EECONFIG_MAGIC_NUMBER);
#ifdef VIA_ENABLE
    if (is_eeprom_enabled) {
        is_eeprom_enabled = via_eeprom_is_valid();
    }
#endif
    return is_eeprom_enabled;
}

/** \brief eeconfig is disabled
 *
 * FIXME: needs doc
 */
bool eeconfig_is_disabled(void) {
    bool is_eeprom_disabled = (eeprom_read_word(EECONFIG_MAGIC) == EECONFIG_MAGIC_NUMBER_OFF);
#ifdef VIA_ENABLE
    if (!is_eeprom_disabled) {
        is_eeprom_disabled = !via_eeprom_is_valid();
    }
#endif
    return is_eeprom_disabled;
}

/** \brief eeconfig read debug
 *
 * FIXME: needs doc
 */
uint8_t eeconfig_read_debug(void) {
    return eeprom_read_byte(EECONFIG_DEBUG);
}
/** \brief eeconfig update debug
 *
 * FIXME: needs doc
 */
void eeconfig_update_debug(uint8_t val) {
    eeprom_update_byte(EECONFIG_DEBUG, val);
}

/** \brief eeconfig read default layer
 *
 * FIXME: needs doc
 */
uint8_t eeconfig_read_default_layer(void) {
    return eeprom_read_byte(EECONFIG_DEFAULT_LAYER);
}
/** \brief eeconfig update default layer
 *
 * FIXME: needs doc
 */
void eeconfig_update_default_layer(uint8_t val) {
    eeprom_update_byte(EECONFIG_DEFAULT_LAYER, val);
}

/** \brief eeconfig read keymap
 *
 * FIXME: needs doc
 */
uint16_t eeconfig_read_keymap(void) {
    return eeprom_read_word(EECONFIG_KEYMAP);
}
/** \brief eeconfig update keymap
 *
 * FIXME: needs doc
 */
void eeconfig_update_keymap(uint16_t val) {
    eeprom_update_word(EECONFIG_KEYMAP, val);
}

/** \brief eeconfig read audio
 *
 * FIXME: needs doc
 */
uint8_t eeconfig_read_audio(void) {
    return eeprom_read_byte(EECONFIG_AUDIO);
}
/** \brief eeconfig update audio
 *
 * FIXME: needs doc
 */
void eeconfig_update_audio(uint8_t val) {
    eeprom_update_byte(EECONFIG_AUDIO, val);
}

#if (EECONFIG_KB_DATA_SIZE) == 0
/** \brief eeconfig read kb
 *
 * FIXME: needs doc
 */
uint32_t eeconfig_read_kb(void) {
    return eeprom_read_dword(EECONFIG_KEYBOARD);
}
/** \brief eeconfig update kb
 *
 * FIXME: needs doc
 */
void eeconfig_update_kb(uint32_t val) {
    eeprom_update_dword(EECONFIG_KEYBOARD, val);
}
#endif // (EECONFIG_KB_DATA_SIZE) == 0

#if (EECONFIG_USER_DATA_SIZE) == 0
/** \brief eeconfig read user
 *
 * FIXME: needs doc
 */
uint32_t eeconfig_read_user(void) {
    return eeprom_read_dword(EECONFIG_USER);
}
uint32_t eeconfig_read_user1(void) {
    return eeprom_read_dword(EECONFIG_USER1);
}
uint32_t eeconfig_read_user2(void) {
    return eeprom_read_dword(EECONFIG_USER2);
}
uint32_t eeconfig_read_user3(void) {
    return eeprom_read_dword(EECONFIG_USER3);
}
/** \brief eeconfig update user
 *
 * FIXME: needs doc
 */
void eeconfig_update_user(uint32_t val,uint32_t val1,uint32_t val2,uint32_t val3) {
    eeprom_update_dword(EECONFIG_USER, val);
    eeprom_update_dword(EECONFIG_USER1, val1);
    eeprom_update_dword(EECONFIG_USER2, val2);
    eeprom_update_dword(EECONFIG_USER3, val3);
}
#endif // (EECONFIG_USER_DATA_SIZE) == 0

/** \brief eeconfig read haptic
 *
 * FIXME: needs doc
 */
uint32_t eeconfig_read_haptic(void) {
    return eeprom_read_dword(EECONFIG_HAPTIC);
}
/** \brief eeconfig update haptic
 *
 * FIXME: needs doc
 */
void eeconfig_update_haptic(uint32_t val) {
    eeprom_update_dword(EECONFIG_HAPTIC, val);
}

/** \brief eeconfig read split handedness
 *
 * FIXME: needs doc
 */
bool eeconfig_read_handedness(void) {
    return !!eeprom_read_byte(EECONFIG_HANDEDNESS);
}
/** \brief eeconfig update split handedness
 *
 * FIXME: needs doc
 */
void eeconfig_update_handedness(bool val) {
    eeprom_update_byte(EECONFIG_HANDEDNESS, !!val);
}

#if (EECONFIG_KB_DATA_SIZE) > 0
/** \brief eeconfig assert keyboard data block version
 *
 * FIXME: needs doc
 */
bool eeconfig_is_kb_datablock_valid(void) {
    return eeprom_read_dword(EECONFIG_KEYBOARD) == (EECONFIG_KB_DATA_VERSION);
}
/** \brief eeconfig read keyboard data block
 *
 * FIXME: needs doc
 */
void eeconfig_read_kb_datablock(void *data) {
    if (eeconfig_is_kb_datablock_valid()) {
        eeprom_read_block(data, EECONFIG_KB_DATABLOCK, (EECONFIG_KB_DATA_SIZE));
    } else {
        memset(data, 0, (EECONFIG_KB_DATA_SIZE));
    }
}
/** \brief eeconfig update keyboard data block
 *
 * FIXME: needs doc
 */
void eeconfig_update_kb_datablock(const void *data) {
    eeprom_update_dword(EECONFIG_KEYBOARD, (EECONFIG_KB_DATA_VERSION));
    eeprom_update_block(data, EECONFIG_KB_DATABLOCK, (EECONFIG_KB_DATA_SIZE));
}
/** \brief eeconfig init keyboard data block
 *
 * FIXME: needs doc
 */
__attribute__((weak)) void eeconfig_init_kb_datablock(void) {
    uint8_t dummy_kb[(EECONFIG_KB_DATA_SIZE)] = {0};
    eeconfig_update_kb_datablock(dummy_kb);
}
#endif // (EECONFIG_KB_DATA_SIZE) > 0

#if (EECONFIG_USER_DATA_SIZE) > 0
/** \brief eeconfig assert user data block version
 *
 * FIXME: needs doc
 */
bool eeconfig_is_user_datablock_valid(void) {
    return eeprom_read_dword(EECONFIG_USER) == (EECONFIG_USER_DATA_VERSION);
}
/** \brief eeconfig read user data block
 *
 * FIXME: needs doc
 */
void eeconfig_read_user_datablock(void *data) {
    if (eeconfig_is_user_datablock_valid()) {
        eeprom_read_block(data, EECONFIG_USER_DATABLOCK, (EECONFIG_USER_DATA_SIZE));
    } else {
        memset(data, 0, (EECONFIG_USER_DATA_SIZE));
    }
}
/** \brief eeconfig update user data block
 *
 * FIXME: needs doc
 */
void eeconfig_update_user_datablock(const void *data) {
    eeprom_update_dword(EECONFIG_USER, (EECONFIG_USER_DATA_VERSION));
    eeprom_update_block(data, EECONFIG_USER_DATABLOCK, (EECONFIG_USER_DATA_SIZE));
}
/** \brief eeconfig init user data block
 *
 * FIXME: needs doc
 */
__attribute__((weak)) void eeconfig_init_user_datablock(void) {
    uint8_t dummy_user[(EECONFIG_USER_DATA_SIZE)] = {0};
    eeconfig_update_user_datablock(dummy_user);
}
#endif // (EECONFIG_USER_DATA_SIZE) > 0
