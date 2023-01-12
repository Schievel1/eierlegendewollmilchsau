#include QMK_KEYBOARD_H

enum unicode_names {
DESSS,
DEBAE,
DESAE,
DEBOE,
DESOE,
DEBUE,
DESUE,
EURO
};

const uint32_t PROGMEM unicode_map[] = {
[DESSS]  = 0x00DF,  // ß
[DEBAE] = 0x00C4,  // Ä
[DESAE]  = 0x00E4, // ä
[DEBOE]  = 0x00D6,  // Ö
[DESOE] = 0x00F6,  // ö
[DEBUE]  = 0x00DC, // Ü
[DESUE]  = 0x00FC, // ü
[EURO] = 0x20AC
};

#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2

#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)

/*******************/
/*  k e y m a p s  */
/*******************/
const key_override_t   lcbr_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_LCBR, KC_LBRC); // Shift { is [
const key_override_t   rcbr_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_RCBR, KC_RBRC); // Shift } is ]
const key_override_t** key_overrides     = (const key_override_t*[]){&lcbr_key_override, &rcbr_key_override, NULL};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT_5x6(
                         KC_GRV,     KC_1,    KC_2,    KC_3 ,    KC_4,    KC_5,                  KC_6,    KC_7,       KC_8,      KC_9,      KC_0,   KC_EQL,
                         KC_TAB,     KC_Q  , KC_W  , KC_E  , KC_R  , KC_T  ,                         KC_Y  , KC_U  , KC_I  , KC_O  , KC_P  ,KC_MINS,
                         KC_ESC,     KC_A  , KC_S  , KC_D  , KC_F  , KC_G  ,                         KC_H  , KC_J  , KC_K  , KC_L  ,KC_SCLN,KC_QUOT,
                         KC_NO,   KC_Z  , KC_X  , KC_C  , KC_V  , KC_B  ,                         KC_N  , KC_M  ,KC_COMM,KC_DOT ,KC_SLSH,KC_BSLS,
                                           KC_LBRC,  KC_RBRC,                                                      KC_PGUP,     KC_PGDN,
                                                            KC_LSFT,    _______,                 _______,KC_RSFT,
                                                            KC_LCTL,    KC_SPC,                  KC_RGUI,KC_BSPC,
                                                            KC_LALT,    LOWER,                   KC_ENT, KC_RALT
                         ),

  [_LOWER] = LAYOUT_5x6(

                        KC_TILD,     KC_F1,                  KC_F2,    KC_F3,         KC_F4,     KC_F5,          KC_F6,            KC_F7,     KC_F8,                KC_F9,      KC_F10,     KC_DEL,
                        _______,    _______,               _______,  XP(EURO,EURO),       _______,   KC_LCBR,        KC_RCBR, XP(DESUE, DEBUE),   _______,     XP(DESOE, DESOE),     _______,    _______,
                        _______,XP(DESAE, DEBAE), XP(DESSS, DESSS),    RAISE,        KC_DEL,   KC_LPRN,        KC_RPRN,          KC_LEFT,     KC_UP,              KC_DOWN,     KC_RGHT,    KC_PIPE,
                        KC_CAPS ,   _______,               _______,  LCTL(KC_X), LCTL(KC_C),LCTL(KC_V),        _______,          _______,   _______,              _______,     _______,    _______,
                                                            _______,_______,                                                       _______,_______,
                                                                 _______,_______,                  _______,_______,
                                                                 _______,_______,                  _______,_______,
                                                                 _______,_______,                 _______,_______
                        ),

  [_RAISE] = LAYOUT_5x6(

                        _______, _______, _______, _______, _______, _______,                    _______,  KC_NUM, KC_PSLS, KC_PAST, KC_PMNS, KC_CALC,
                        _______, _______, _______, _______, _______, KC_LBRC,                    KC_RBRC,   KC_P7,   KC_P8,   KC_P9,   KC_PPLS, KC_MUTE,
                        _______, _______, _______, _______, _______, KC_LPRN,                    KC_RPRN,   KC_P4,   KC_P5,   KC_P6,   _______, KC_VOLU,
                        _______, _______, _______, _______, _______, _______,                    KC_P0,     KC_P1,   KC_P2,   KC_P3,   KC_PEQL, KC_VOLD,
                                           _______,_______,                                                         KC_DOT, KC_COMM,
                                                                 _______,_______,              _______,_______,
                                                                 _______,_______,              _______,_______,
                                                                 _______,_______,             _______,_______
                        ),};
// clang-format on
