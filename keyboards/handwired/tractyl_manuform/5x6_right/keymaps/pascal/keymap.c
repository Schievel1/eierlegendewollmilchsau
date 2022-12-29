/* Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "rgb_matrix_user.h"
#include "transactions.h"
#include "qp.h"

// quantum painter images
#include "graphics/dickbutt.qgf.h"

//debugging:
#include "print.h"

// for the big oled display
painter_device_t big_display;
painter_image_handle_t dickbutt;

// global variables for idle mode
bool idle_mode = false;
int old_rgb_mode;
uint32_t idle_timer = 0;
void idle_function(void);

/*******************/
/*  k e y m a p s  */
/*******************/
enum custom_layers {
    _DVORAK,
    _LOWER,
    _RAISE,
};

#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)

/*****************************************************/
/*  T r a n s p o r t   l a y e r   t o   s l a v e  */
/*****************************************************/
typedef struct _master_to_slave_t {
    layer_state_t current_layer_state;
} master_to_slave_t;

typedef struct _slave_to_master_t {
    layer_state_t current_layer_state;
} slave_to_master_t;

void user_sync_a_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    const master_to_slave_t *m2s = (const master_to_slave_t*)in_data;
    slave_to_master_t *s2m = (slave_to_master_t*)out_data;
    if (layer_state != m2s->current_layer_state) {
        layer_state = m2s->current_layer_state;
    }
    s2m->current_layer_state = m2s->current_layer_state;  // just mirror it back for debug purposes
}

void housekeeping_task_user(void) {
    if (is_keyboard_master()) {
        static uint32_t last_sync = 0;
        if (timer_elapsed32(last_sync) > USER_COM_POLL_TIME_MS) {
            /* dprintf("current layer state: %d\n", layer_state); */
            master_to_slave_t m2s = {layer_state};
            slave_to_master_t s2m = {0};
            if (transaction_rpc_exec(USER_SYNC_A, sizeof(m2s), &m2s, sizeof(s2m), &s2m)) {
                last_sync = timer_read32();
                /* dprintf("Slave current layer value: %d\n", s2m.current_layer_state); */
            } else {
                dprint("Slave sync failed!\n");
            }
        }
    }
    // draw display every 33 ms
    static uint32_t last_draw = 0;
	static uint32_t clear_timer = 0;
    if (timer_elapsed32(last_draw) > 33) { // Throttle to 30fps
        last_draw = timer_read32();
		/* qp_drawimage(big_display, 0, 0, dickbutt); */
		qp_circle(big_display, last_draw%240, last_draw%320, 25, last_draw%255, 255, 255, false);
        qp_rect(big_display, 0, 0, 10, 10, HSV_BLUE, true);
        qp_rect(big_display, 10, 10, 20, 20, HSV_GREEN, true);
        if (timer_elapsed32(clear_timer) > 10000 && timer_elapsed32(clear_timer) < 10500) {
                dprint("Slave sync failed`\n");
            qp_clear(big_display);
			qp_init(big_display, QP_ROTATION_270);
            qp_power(big_display, true);
            qp_rect(big_display, 0, 0, 319, 239, HSV_BLACK, true);
            clear_timer = timer_read32();
        }
        qp_flush(big_display);
    }
    // enable sniping mode with lower layer
    charybdis_set_pointer_sniping_enabled((biton32(layer_state)) == _LOWER);
}

/***********/
/*  i n i t */
/***********/
void keyboard_post_init_user(void) {
    // Debug: Customise these values to desired behaviour
    debug_enable   = true;
    debug_matrix   = true;
    debug_keyboard = true;
    debug_mouse    = true;
	// user comms
    transaction_register_rpc(USER_SYNC_A, user_sync_a_slave_handler);
	// quantum painter
    big_display = qp_ili9341_make_spi_device(320, 240, ILI9341_CS_PIN, ILI9341_DCRS_PIN, ILI9341_RESET_PIN, 2, 0);
    qp_init(big_display, QP_ROTATION_270);
    qp_power(big_display, true);
    qp_rect(big_display, 0, 0, 319, 239, HSV_BLACK, true);
	/* dickbutt =  qp_load_image_mem(dickbutt); */
	backlight_enable();
	backlight_level(4);
}

/*******************/
/*  k e y m a p s  */
/*******************/
const key_override_t lcbr_key_override =
    ko_make_basic(MOD_MASK_SHIFT, KC_LCBR, KC_LBRC);  // Shift { is [
const key_override_t rcbr_key_override =
    ko_make_basic(MOD_MASK_SHIFT, KC_RCBR, KC_RBRC); // Shift } is ]
const key_override_t** key_overrides = (const key_override_t*[]){
    &lcbr_key_override,
    &rcbr_key_override,
    NULL
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_DVORAK] = LAYOUT_5x6_right(
                         KC_GRV,     KC_1,    KC_2,    KC_3 ,    KC_4,    KC_5,                  KC_6,    KC_7,       KC_8,      KC_9,      KC_0,   KC_EQL,
                         KC_TAB,  KC_QUOT, KC_COMM,   KC_DOT,    KC_P,    KC_Y,                  KC_F,    KC_G,       KC_C,      KC_R,      KC_L,  KC_SLSH,
                         KC_ESC,     KC_A,    KC_O,     KC_E,    KC_U,    KC_I,                  KC_D,    KC_H,       KC_T,      KC_N,      KC_S,  KC_MINS,
                         KC_NO,   KC_SCLN,    KC_Q,     KC_J,    KC_K,    KC_X,                  KC_B,    KC_M,       KC_W,      KC_V,      KC_Z,  KC_BSLS,
                                           KC_LBRC,  KC_RBRC,                                                      KC_PGUP,     LOWER,
                                                            KC_LSFT,    KC_SPC,                 KC_RSFT,
                                                            KC_LCTL,    LOWER,                     KC_BSPC,
                                                            KC_LALT,    KC_LGUI,        KC_ENT, KC_RALT
                         ),

  [_LOWER] = LAYOUT_5x6_right(

                        KC_TILD,     KC_F1,    KC_F2,    KC_F3,         KC_F4,     KC_F5,          KC_F6,   KC_F7,     KC_F8,       KC_F9,      KC_F10,     KC_DEL,
                        _______,    _______, _______,  _______,       _______,   KC_LCBR,        KC_RCBR, KC_BTN2,   _______,     _______,     _______,    QK_BOOT,
                        _______,    _______, _______,    RAISE,        KC_DEL,   KC_LPRN,        KC_RPRN, KC_LEFT,     KC_UP,     KC_DOWN,     KC_RGHT,    KC_PIPE,
                        KC_CAPS ,   RGB_TOG, _______,  LCTL(KC_X), LCTL(KC_C),LCTL(KC_V),        _______, KC_BTN1,   BL_TOGG,       BL_UP,     BL_DOWN,    _______,
                                           RGB_MOD,RGB_RMOD,                                                       RGB_HUI,RGB_SAI,
                                                                 _______,_______,                         _______,
                                                                 _______,_______,                         _______,
                                                                 _______,_______,                 _______,_______
                        ),

  [_RAISE] = LAYOUT_5x6_right(

                        _______, _______, _______, _______, _______, _______,                    _______, _______, KC_PSLS, KC_PAST, KC_PMNS, KC_CALC,
                        _______, _______, _______, _______, _______, KC_LBRC,                    KC_RBRC, KC_P7,   KC_P8,   KC_P9,   KC_PPLS, KC_MUTE,
                        _______, _______, _______, _______, _______, KC_LPRN,                    KC_RPRN, KC_P4,   KC_P5,   KC_P6,   _______, KC_VOLU,
                        _______, _______, _______, _______, _______, _______,                    KC_P0,   KC_P1,   KC_P2,   KC_P3,   KC_PEQL, KC_VOLD,
                                           _______,_______,                                                         KC_DOT, KC_COMM,
                                                                 _______,_______,                     _______,
                                                                 _______,_______,                     _______,
                                                                 _______,_______,             _______,_______
                        ),};// clang-format on

/*****************************/
/*  f o r   e n c o d e r s  */
/*****************************/
bool encoder_update_user(uint8_t index, bool clockwise) {
    dprintf("encoder index: %d\n", index);
    if (index == 1) // master side
    {
        if (clockwise) {
            tap_code(KC_WH_U);
        } else {
            tap_code(KC_WH_D);
        }
    }
    if (index == 0) // slave side
    {
        if (clockwise) {
            tap_code(KC_WH_U);
        } else {
            tap_code(KC_WH_D);
        }
    }
    return true;
}

/*************************************/
/*  f o r   O L E D   d i s p l a y  */
/*************************************/
#ifdef OLED_ENABLE
bool oled_task_user(void) {
    /* Host Keyboard Layer Status */
    oled_write_P(PSTR("Layer: "), false);
    switch (biton32(layer_state)) {
        case _DVORAK:
            oled_write_P(PSTR("Dvorak\n"), false);
            break;
        case _LOWER:
            oled_write_P(PSTR("LOWER\n"), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("RAISE\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }

    oled_write_P(PSTR("RGB: "), false);
    if (!(rgb_matrix_is_enabled())) {
        oled_write_P(PSTR("Off\n"), false);
    } else {
        switch (rgb_matrix_get_mode()) {
            case MY_RGB_MATRIX_SOLID_COLOR:
                oled_write_P(PSTR("Solid Color\n"), false);
                break;
            case MY_RGB_MATRIX_ALPHAS_MODS:
                oled_write_P(PSTR("Alphas Mods\n"), false);
                break;
            case MY_RGB_MATRIX_GRADIENT_UP_DOWN:
                oled_write_P(PSTR("Gradient Up Down\n"), false);
                break;
            case MY_RGB_MATRIX_GRADIENT_LEFT_RIGHT:
                oled_write_P(PSTR("Gradient Left Right\n"), false);
                break;
            case MY_RGB_MATRIX_BREATHING:
                oled_write_P(PSTR("Breathing\n"), false);
                break;
            case MY_RGB_MATRIX_BAND_SAT:
                oled_write_P(PSTR("Band Sat\n"), false);
                break;
            case MY_RGB_MATRIX_BAND_VAL:
                oled_write_P(PSTR("Band VAL\n"), false);
                break;
            case MY_RGB_MATRIX_BAND_PINWHEEL_SAT:
                oled_write_P(PSTR("Band Pinwheel Sat\n"), false);
                break;
            case MY_RGB_MATRIX_BAND_PINWHEEL_VAL:
                oled_write_P(PSTR("Band Pinwheel Val\n"), false);
                break;
            case MY_RGB_MATRIX_BAND_SPIRAL_SAT:
                oled_write_P(PSTR("Band Spiral Sat\n"), false);
                break;
            case MY_RGB_MATRIX_BAND_SPIRAL_VAL:
                oled_write_P(PSTR("Band Spiral Val\n"), false);
                break;
            case MY_RGB_MATRIX_CYCLE_ALL:
                oled_write_P(PSTR("Cycle All\n"), false);
                break;
            case MY_RGB_MATRIX_CYCLE_LEFT_RIGHT:
                oled_write_P(PSTR("Cycle Left Right\n"), false);
                break;
            case MY_RGB_MATRIX_CYCLE_UP_DOWN:
                oled_write_P(PSTR("Cycle Up Down\n"), false);
                break;
            case MY_RGB_MATRIX_CYCLE_OUT_IN:
                oled_write_P(PSTR("Cycle Out In\n"), false);
                break;
            case MY_RGB_MATRIX_CYCLE_OUT_IN_DUAL:
                oled_write_P(PSTR("Cycle Out IN Dual\n"), false);
                break;
            case MY_RGB_MATRIX_RAINBOW_MOVING_CHEVRON:
                oled_write_P(PSTR("Rainbow Moving Chevron\n"), false);
                break;
            case MY_RGB_MATRIX_CYCLE_PINWHEEL:
                oled_write_P(PSTR("Cycle Pinwheel\n"), false);
                break;
            case MY_RGB_MATRIX_CYCLE_SPIRAL:
                oled_write_P(PSTR("Cycle Spiral\n"), false);
                break;
            case MY_RGB_MATRIX_DUAL_BEACON:
                oled_write_P(PSTR("Dual Beacon\n"), false);
                break;
            case MY_RGB_MATRIX_RAINBOW_BEACON:
                oled_write_P(PSTR("Rainbow Beacon\n"), false);
                break;
            case MY_RGB_MATRIX_RAINBOW_PINWHEELS:
                oled_write_P(PSTR("Rainbow Pinwheels\n"), false);
                break;
            case MY_RGB_MATRIX_RAINDROPS:
                oled_write_P(PSTR("Raindrops\n"), false);
                break;
            case MY_RGB_MATRIX_JELLYBEAN_RAINDROPS:
                oled_write_P(PSTR("Jellybean Raindrops\n"), false);
                break;
            case MY_RGB_MATRIX_HUE_BREATHING:
                oled_write_P(PSTR("Hue Breathing\n"), false);
                break;
            case MY_RGB_MATRIX_HUE_PENDULUM:
                oled_write_P(PSTR("Hue Pendulum\n"), false);
                break;
            case MY_RGB_MATRIX_HUE_WAVE:
                oled_write_P(PSTR("Hue Wave\n"), false);
                break;
#    if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS)
            case MY_RGB_MATRIX_TYPING_HEATMAP:
                oled_write_P(PSTR("Typing Heatmap\n"), false);
                break;
            case MY_RGB_MATRIX_DIGITAL_RAIN:
                oled_write_P(PSTR("Digital Rain\n"), false);
                break;
#    endif
#    if defined(RGB_MATRIX_KEYPRESSES) || defined(RGb_MATRIX_KEYRELEASES)
            case MY_RGB_MATRIX_SOLID_REACTIVE_SIMPLE:
                oled_write_P(PSTR("Solid Reactive Simple\n"), false);
                break;
            case MY_RGB_MATRIX_SOLID_REACTIVE:
                oled_write_P(PSTR("Solid Reactive\n"), false);
                break;
            case MY_RGB_MATRIX_SOLID_REACTIVE_WIDE:
                oled_write_P(PSTR("Solid Reactive Wide\n"), false);
                break;
            case MY_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE:
                oled_write_P(PSTR("Solid Reactive Multiwide\n"), false);
                break;
            case MY_RGB_MATRIX_SOLID_REACTIVE_CROSS:
                oled_write_P(PSTR("Solid Reactive Cross\n"), false);
                break;
            case MY_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS:
                oled_write_P(PSTR("Solid Reactive Multicross\n"), false);
                break;
            case MY_RGB_MATRIX_SOLID_REACTIVE_NEXUS:
                oled_write_P(PSTR("Solid Reactive NEXUS\n"), false);
                break;
            case MY_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS:
                oled_write_P(PSTR("Solid Reactive Multinexus\n"), false);
                break;
            case MY_RGB_MATRIX_SPLASH:
                oled_write_P(PSTR("Splash\n"), false);
                break;
            case MY_RGB_MATRIX_MULTISPLASH:
                oled_write_P(PSTR("Multisplash\n"), false);
                break;
            case MY_RGB_MATRIX_SOLID_SPLASH:
                oled_write_P(PSTR("Solid Splash\n"), false);
                break;
            case MY_RGB_MATRIX_SOLID_MULTISPLASH:
                oled_write_P(PSTR("Solid Multisplash\n"), false);
                break;
#    endif
            case MY_RGB_MATRIX_EFFECT_MAX:
                break;
                oled_write_P(PSTR("Matrix Effect Max\n"), false);
            default:
                oled_write_P(PSTR("Undefined\n"), false);
        }
    }

    /* Host Keyboard LED Status */
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
    return false;
}
#endif

/*************************************************************/
/*  R G B   m a t r i x   r e s p o n d s   t o   l a y e r  */
/*************************************************************/
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    static bool re_raise = false;
    static bool re_lower = false;
    switch (biton32(layer_state)) {
        case _RAISE:
            if (!re_raise) {
                for (int i = 0; i < 10; i++) {
                    rgb_matrix_increase_hue_noeeprom();
                }
            }
            re_raise = true;
            // set num block keys to static color
            rgb_matrix_set_color(36, RGB_YELLOW); // num lock
            rgb_matrix_set_color(42, RGB_GREEN); // numbers
            rgb_matrix_set_color(41, RGB_GREEN);
            rgb_matrix_set_color(40, RGB_GREEN);
            rgb_matrix_set_color(48, RGB_GREEN);
            rgb_matrix_set_color(47, RGB_GREEN);
            rgb_matrix_set_color(46, RGB_GREEN);
            rgb_matrix_set_color(55, RGB_GREEN);
            rgb_matrix_set_color(54, RGB_GREEN);
            rgb_matrix_set_color(53, RGB_GREEN);
            rgb_matrix_set_color(52, RGB_GREEN);
            rgb_matrix_set_color(35, RGB_BLUE); // operators
            rgb_matrix_set_color(34, RGB_BLUE);
            rgb_matrix_set_color(33, RGB_BLUE);
            rgb_matrix_set_color(39, RGB_BLUE);
            rgb_matrix_set_color(35, RGB_BLUE);
            rgb_matrix_set_color(51, RGB_RED); // equals
            break;
        case _LOWER:
            if (!re_lower) {
                for (int i = 0; i < 15; i++) {
                    rgb_matrix_increase_hue_noeeprom();
                }
            }
            if (re_raise) {
                for (int i = 0; i < 10; i++) {
                    rgb_matrix_decrease_hue_noeeprom();
                }
                re_raise = false;
            }
            re_lower = true;
            break;
        case _DVORAK:
            if (re_lower) {
                for (int i = 0; i < 15; i++) {
                    rgb_matrix_decrease_hue_noeeprom();
                }
            }
            if (re_raise) {
                for (int i = 0; i < 10; i++) {
                    rgb_matrix_decrease_hue_noeeprom();
                }
            }
            re_raise = false;
            re_lower = false;
        default:
            break;
    }
    return false;
}

/*******************************************/
/*  k e y b o a r d   i d l e   t i m e r  */
/*******************************************/
bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    if (record->event.pressed) {
        idle_timer = timer_read32();
        idle_mode  = false;
    }
    return true;
}

void matrix_scan_user(void) {
    // idle_timer needs to be set one time
    if (idle_timer == 0) idle_timer = timer_read32();
    if (timer_elapsed32(idle_timer) > IDLE_TIMEOUT_SECS * 1000 && !idle_mode) {
		idle_mode = true;
		idle_timer = timer_read32();
    }
	idle_function();
}

void idle_function() {
	static bool last_state_idle = false;
	if (idle_mode && !last_state_idle) { // rising edge of idle mode
		old_rgb_mode = rgb_matrix_get_mode();
		rgb_matrix_mode(RGB_MATRIX_IDLE_MODE);
	}
	if (!idle_mode && last_state_idle) { // falling edge of idle mode
		rgb_matrix_mode(old_rgb_mode);
	}
	last_state_idle = idle_mode;
}
