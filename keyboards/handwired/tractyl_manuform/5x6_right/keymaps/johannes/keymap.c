/* Copyright 2020 Pascal Jaeger <pascal.jaeger@leimstift.de>
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
//#include "qp.h"

// user level imports
#include "keymap.h"
#include "transport.h"
//#include "ili9341_display.h"
#include "rgb_matrix_user.h"

// debugging:
//#ifdef CONSOLE_ENABLE
#include "print.h"
//#endif

static void normalize_keymap(void);
// global declarations for idle mode
bool     idle_mode = false;
bool     sleep_mode = false;
int      old_rgb_mode;
uint32_t idle_timer = 0;
void     idle_function(void); 
void     sleep_function(void);
void suspend_power_down_kb(void) { oled_off(); } 
void housekeeping_task_user(void) {
    master_slave_com();
    // draw display every 33 ms
    static uint32_t last_draw = 0;
    if (timer_elapsed32(last_draw) > 33) { // Throttle to 30fps
        last_draw = timer_read32();
   //     ili9341_draw_display(big_display);
    }
    // enable sniping mode with lower layer
    charybdis_set_pointer_sniping_enabled(biton32(layer_state) == _LOWER);
    // enable dragscroll mode when left shift key is pressed
    charybdis_set_pointer_dragscroll_enabled(biton32(layer_state) == _RAISE);
 
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
 print("1");
    user_sync_init();
    // init ili9341 display
   // big_display = ili9341_init();
    // backlight of the big LED screen on on startup
    print("2");
    //backlight_enable();
    print("3");
    //backlight_level(4);
    // turn numlock on on startup
    print("4");
    if (!(host_keyboard_leds() & (1 << USB_LED_NUM_LOCK))) {
        register_code(KC_NUM_LOCK);
        unregister_code(KC_NUM_LOCK);
    }
	normalize_keymap();
}

// HACK terrible hack to UNmagic the keymap
// somehow this gets mixed up all over the place sometimes
static void normalize_keymap(void) {
    keymap_config.raw                      = eeconfig_read_keymap();
    keymap_config.swap_control_capslock    = false;
    keymap_config.swap_escape_capslock     = false;
    keymap_config.capslock_to_control      = false;
    keymap_config.swap_lalt_lgui           = false;
    keymap_config.swap_ralt_rgui           = false;
    keymap_config.swap_lctl_lgui           = false;
    keymap_config.swap_rctl_rgui           = false;
    keymap_config.no_gui                   = false;
    keymap_config.swap_grave_esc           = false;
    keymap_config.swap_backslash_backspace = false;
    keymap_config.swap_lalt_lgui = keymap_config.swap_ralt_rgui = false;
    keymap_config.swap_lctl_lgui = keymap_config.swap_rctl_rgui = false;
    eeconfig_update_keymap(keymap_config.raw);
    clear_keyboard();
}
/*******************/
/*  k e y m a p s  */
/*******************/
const key_override_t   lcbr_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_LCBR, KC_LBRC); // Shift { is [
const key_override_t   rcbr_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_RCBR, KC_RBRC); // Shift } is ]
const key_override_t** key_overrides     = (const key_override_t*[]){&lcbr_key_override, &rcbr_key_override, NULL};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_DVORAK] = LAYOUT_5x6_right(
                         KC_ESC,     KC_1,    KC_2,     KC_3 ,   KC_4,    KC_5,                  KC_6,    KC_7,       KC_8,      KC_9,      KC_0,    KC_EQL,
                         KC_TAB,     KC_Q,    KC_W,     KC_E,    KC_R,    KC_T,                  KC_Z,    KC_U,       KC_I,      KC_O,      KC_P,    KC_MINS,
                         KC_LSFT,     KC_A,    KC_S,     KC_D,    KC_F,    KC_G,                  KC_H,    KC_J,       KC_K,      KC_L,      KC_SCLN, KC_QUOT,
                         TG(RAISE),      KC_Y,    KC_X,     KC_C,    KC_V,    KC_B,                  KC_N,    KC_M,       KC_COMM,   KC_DOT,    KC_SLSH, KC_BSLS,
                                           KC_LBRC,  KC_RBRC,                                                      KC_PGUP,     LOWER,
                                                            KC_LSFT,    KC_SPC,                 KC_RSFT,
                                                            KC_LCTL,    LOWER,                     KC_SPC,
                                                            KC_LALT,    KC_LGUI,        KC_RALT, KC_ENT
                         ),

  [_LOWER] = LAYOUT_5x6_right(

                        KC_TILD,     KC_F1,    KC_F2,    KC_F3,         KC_F4,     KC_F5,          KC_F6,   KC_F7,     KC_F8,       KC_F9,      KC_F10,     EE_CLR,
                        _______,    _______, _______,  _______,       _______,   KC_LCBR,        KC_RCBR, KC_LEFT,     KC_UP,     KC_DOWN,     KC_RGHT,    QK_BOOT,
                        _______,    _______, _______,    RAISE,        KC_DEL,   KC_LPRN,        KC_RPRN, KC_BTN1,   KC_BTN2,     KC_DOWN,     KC_RGHT,    KC_PIPE,
                        KC_CAPS ,   RGB_TOG, _______,  LCTL(KC_X), LCTL(KC_C),LCTL(KC_V),        KC_DEL, LSFT(KC_INS),   BL_TOGG,       BL_UP,     BL_DOWN,    DB_TOGG,
                                           RGB_MOD,RGB_RMOD,                                                       RGB_HUI,RGB_SAI,
                                                                 KC_LSFT,_______,                         _______,
                                                                 S_D_RMOD,S_D_MOD,                        KC_BSPC,
                                                                 DPI_RMOD,DPI_MOD,                 _______,_______
                        ),

  [_RAISE] = LAYOUT_5x6_right(

                        _______, KC_ESC,     KC_1,    KC_2,     KC_3 ,   KC_4,           _______,  KC_NUM, KC_PSLS, KC_PAST, KC_PMNS, KC_CALC,
                        _______, KC_TAB,     KC_Q,    KC_W,     KC_E,    KC_R,                    KC_RBRC,   KC_P7,   KC_P8,   KC_P9,   KC_PPLS, KC_MUTE,
                        _______, KC_LSFT,     KC_A,    KC_S,     KC_D,    KC_F,                    KC_RPRN,   KC_P4,   KC_P5,   KC_P6,   _______, KC_VOLU,
                        KC_NO, KC_LCTL,      KC_Y,    KC_X,     KC_C,    KC_V,                    KC_P0,     KC_P1,   KC_P2,   KC_P3,   KC_PEQL, KC_VOLD,
                                           _______,_______,                                                         KC_DOT, KC_COMM,
                                                                 KC_LSFT,    KC_SPC,                     _______,
                                                                 KC_LCTL,_______,                     _______,
                                                                 _______,_______,             TG(RAISE),_______
                        ),};
// clang-format on

/*****************************/
/*  f o r   e n c o d e r s  */
/*****************************/
bool encoder_update_user(uint8_t index, bool clockwise) {
    dprintf("encoder index: %d\n", index);
   switch (biton32(layer_state)) {
            case _DVORAK:
/////////////////////////////////////////////		
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
			tap_code(KC_WH_L);
        } else {
            tap_code(KC_WH_R);
        }
    }
    return true;

///////////////////////
                break;
            case _LOWER:
/////////////////////////////////////////////	
                if (index == 1) // master side
    {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    if (index == 0) // slave side
    {
        if (clockwise) {
            register_code(KC_LCTL);
			tap_code(KC_WH_U);
			unregister_code(KC_LCTL);
        } else {
			register_code(KC_LCTL);  
            tap_code(KC_WH_D);
			unregister_code(KC_LCTL);
        }
    }
    return true;

///////////////////////
                break;
            case _RAISE:
//////////////////////////////////////////////			
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
            register_code(KC_LCTL);
			tap_code(KC_WH_U);
			unregister_code(KC_LCTL);
        } else {
			register_code(KC_LCTL);  
            tap_code(KC_WH_D);
			unregister_code(KC_LCTL);
        }
    }
    return true;

//////////////////////
                break;
            default:
//////////////////////////////////////////////	
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
            register_code(KC_LCTL);
			tap_code(KC_WH_U);
			unregister_code(KC_LCTL);
        } else {
			register_code(KC_LCTL);  
            tap_code(KC_WH_D);
			unregister_code(KC_LCTL);
        }
    }
    return true;

//////////////////////
}
}
  


/*******************************************/
/*  k e y b o a r d   i d l e   t i m e r  */
/*******************************************/
bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    if (record->event.pressed) {
        idle_timer = timer_read32();
        idle_mode  = false;
        sleep_mode  = false;
    }
    // If console is enabled, it will print the matrix position and status of each key pressed
#ifdef CONSOLE_ENABLE
    dprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif
    return true;
}

void matrix_scan_user(void) {
    // idle_timer needs to be set one time
    if (idle_timer == 0) idle_timer = timer_read32();
    if (timer_elapsed32(idle_timer) > IDLE_TIMEOUT_SECS * 1000 && !idle_mode) {
        idle_mode  = true;
        idle_timer = timer_read32();
    }
    if (timer_elapsed32(idle_timer) > IDLE_TIMEOUT_SECS * 2000 && !sleep_mode) {
        print("sleep 1\n");
        sleep_mode  = true;
        idle_timer = timer_read32();
    }
    idle_function();
    sleep_function();

}

void idle_function(void) {
    static bool last_state_idle = false;
    if (idle_mode && !last_state_idle) { // rising edge of idle mode
        old_rgb_mode = rgb_matrix_get_mode();
                    print("sleep 2\n");
        dprintf("%i Status sleep\n",sleep_mode);
        rgb_matrix_mode(RGB_MATRIX_IDLE_MODE);
    }
    if (!idle_mode && last_state_idle) { // falling edge of idle mode
        rgb_matrix_mode(old_rgb_mode);

    }
    last_state_idle = idle_mode;
}

void sleep_function(void) {
 
    static bool last_state_sleep = false;
    if (sleep_mode && !last_state_sleep) { // rising edge of idle mode
        rgb_matrix_mode(RGB_MATRIX_SLEEP_MODE);

    }
    if (!sleep_mode && last_state_sleep) { // falling edge of idle mode
        rgb_matrix_mode(old_rgb_mode);
    }
    last_state_sleep = sleep_mode;
}


/*******************************************/
/*  oled sleep mode */
/*******************************************/
//void suspend_power_down_user(void) { oled_off(); }  