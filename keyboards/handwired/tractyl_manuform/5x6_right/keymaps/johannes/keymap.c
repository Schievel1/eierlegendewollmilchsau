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

//EEPROM structure definition



user_config_t user_config;
user_config_t1 user_config1;
user_config_t2 user_config2;
user_config_t3 user_config3;

//static void normalize_keymap(void);
// global declarations for idle mode
bool     idle_mode = false;
bool     sleep_mode = false;

//global WPM declarations
bool isSneaking = false;
bool isJumping  = false;
bool isJiggle = false;
uint8_t ANIM_FRAME_DURATION1_OLD = 1;
/* status variables */
 wpm_state_t   current_wpms = 0;
 led_t led_usb_state;

//Config mode declarations
bool oneShot = 0;

uint8_t LayerEFF = 0;


bool zoom=false;
bool troughtTime=false;
bool snipe=false;
bool drag=false;
bool dragc=false;

/* Smart Backspace Delete */
bool            shift_held = false;
static uint16_t held_shift = 0;

int      old_rgb_mode;
uint32_t idle_timer = 0;
uint32_t sleep_timer = 0;
void     idle_function(void);
void     sleep_function(void);
void     jiggle_function(void);
static report_mouse_t last_mouse_report   = {0};
static report_mouse_t new_mouse_report   = {0};

void housekeeping_task_user(void) {
        current_wpms   = get_current_wpm();

        master_slave_com();
    // draw display every 33 ms
    static uint32_t last_draw = 0;
    if (timer_elapsed32(last_draw) > 150) { // Throttle to 30fps
        last_draw = timer_read32();
   //     ili9341_draw_display(big_display);
    }

new_mouse_report = pointing_device_get_report();
dprint("maus Update");
    if (has_mouse_report_changed(last_mouse_report, new_mouse_report)) {
        idle_timer = timer_read32();
        sleep_timer = timer_read32();
        idle_mode  = false;
        sleep_mode  = false;
        last_mouse_report = pointing_device_get_report();
        dprint("maus Aktuell");
    }


    // enable sniping mode with lower layer
    charybdis_set_pointer_sniping_enabled((biton32(layer_state) == _LOWER)||(snipe));
    // enable dragscroll mode when left shift key is pressed
    charybdis_set_pointer_dragscroll_enabled(((biton32(layer_state) == _RAISE)||(drag))&&!(snipe));

    charybdis_set_pointer_dragcurser_enabled(((biton32(layer_state) == _PROG)||(dragc))&&!(snipe));

    charybdis_set_pointer_timetravel_enabled((biton32(layer_state) == _PROG)&&(troughtTime));
}

  /***********/
 /* i n i t */
/***********/
void keyboard_post_init_user(void) {
    // Debug: Customise these values to desired behaviour
    debug_enable   = true;
    debug_matrix   = true;
    debug_mouse    = true;

    user_config.raw = eeconfig_read_user();
    user_config1.raw = eeconfig_read_user1();
    user_config2.raw = eeconfig_read_user2();
    user_config3.raw = eeconfig_read_user3();

        last_mouse_report = pointing_device_get_report();

    rgb_matrix_mode_noeeprom(user_config1.EE_EffectL1);
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
    //use old RGB mode if it was disconnected in idle or sleep mode

	//normalize_keymap();
}

void eeconfig_init_user(void) {  // EEPROM is getting reset!
    user_config.raw = 0;
    user_config1.raw = 0;
    user_config2.raw = 0;
    user_config3.raw = 0;

    user_config.EE_OffsLayer_1 = 10;
    user_config.EE_OffsLayer_2 = 15;
    user_config.EE_OffsLayer_3 = 20;
    user_config.EE_OffsLayer_4 = 25;
    user_config1.EE_EffectL1= 31;
    user_config1.EE_EffectL2= 31;
    user_config1.EE_EffectL3= 31;
    user_config1.EE_EffectL4= 31;
    user_config2.EE_EffectL5= 31;
    user_config2.EE_EffectIdle= 25;
    user_config2.EE_TimeSleep = 30;
    user_config2.EE_TimeIdle = 10;
    user_config3.EE_DragCurser = 6;
    user_config3.EE_DragScroll = 6;
 // We want this enabled by default
  eeconfig_update_user(user_config.raw,user_config1.raw,user_config2.raw,user_config3.raw); // Write default value to EEPROM now
}

// HACK terrible hack to UNmagic the keymap
// somehow this gets mixed up all over the place sometimes
// static void normalize_keymap(void) {
//     keymap_config.raw                      = eeconfig_read_keymap();
//     keymap_config.swap_control_capslock    = false;
//     keymap_config.swap_escape_capslock     = false;
//     keymap_config.capslock_to_control      = false;
//     keymap_config.swap_lalt_lgui           = false;
//     keymap_config.swap_ralt_rgui           = false;
//     keymap_config.swap_lctl_lgui           = false;
//     keymap_config.swap_rctl_rgui           = false;
//     keymap_config.no_gui                   = false;
//     keymap_config.swap_grave_esc           = false;
//     keymap_config.swap_backslash_backspace = false;
//     keymap_config.swap_lalt_lgui = keymap_config.swap_ralt_rgui = false;
//     keymap_config.swap_lctl_lgui = keymap_config.swap_rctl_rgui = false;
//     eeconfig_update_keymap(keymap_config.raw);



//     clear_keyboard();
// }
/*******************/
/*  k e y m a p s  */
/*******************/
const key_override_t   lcbr_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_PGUP, KC_LPRN); // Shift PGUP is (
const key_override_t   rcbr_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_PGDN, KC_RPRN); // Shift PGDWN is )
const key_override_t** key_overrides     = (const key_override_t*[]){&lcbr_key_override, &rcbr_key_override, NULL};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
 [_QWERTZ] = LAYOUT_5x6_right(
                         QK_GESC,     KC_1,        KC_2,        KC_3,        KC_4,        KC_5,                          KC_6,        KC_7,        KC_8,        KC_9,        KC_0,        KC_EQL,
                         KC_LSFT,     KC_Q,        KC_W,        KC_E,        KC_R,        KC_T,                          KC_Z,        KC_U,        KC_I,        KC_O,        KC_P,        KC_MINS,
                         KC_TAB,      KC_A,        KC_S,        KC_D,        KC_F,        KC_G,                          KC_H,        KC_J,        KC_K,        KC_L,        KC_SCLN,     KC_QUOT,
                         KC_LCTL,     KC_Y,        KC_X,        KC_C,        KC_V,        KC_B,                          KC_N,        KC_M,        KC_COMM,     KC_DOT,      KC_SLSH,     KC_BSLS,
                                                   KC_LBRC,     KC_RBRC,                                                                           KC_PGUP,     KC_PGDN,
                                                                             KC_LSFT,     SC_LSPO,                         KC_LSFT,
                                                                                KC_LCTL,        TT(LOWER),                               SC_LSPO,
                                                                                KC_LALT,        KC_LGUI,              KC_RALT,       SC_SENT
                         ),

  [_GAME] = LAYOUT_5x6_right(

                         KC_ESC,      KC_ESC,      KC_1,        KC_2,        KC_3,        KC_4,                          XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX,     LALT(KC_TAB),
                         XXXXXXX,     KC_TAB,      KC_Q,        KC_W,        KC_E,        KC_R,                          KC_RBRC,     KC_1,        KC_2,        KC_3,        KC_4,        KC_MUTE,
                         XXXXXXX,     KC_LSFT,     KC_A,        KC_S,        KC_D,        KC_F,                          KC_RPRN,     KC_BTN1,     KC_BTN2,     KC_B,        KC_PPLS,     KC_VOLU,
                         XXXXXXX,      KC_LCTL,     KC_Y,        KC_X,        KC_C,        KC_V,                          KC_G,        KC_R,        KC_Q,        XXXXXXX,     KC_PMNS,     KC_VOLD,
                                                   XXXXXXX,     XXXXXXX,                                                                           XXXXXXX,     XXXXXXX,
                                                                             KC_LSFT,     KC_SPC,                          KC_V,
                                                                                KC_ENT,         KC_SPC,                            KC_C,
                                                                                TO(_PROG),        KC_LCTL,              TG(_GAME),  LALT(KC_TAB)
                        ),

  [_PROG] = LAYOUT_5x6_right(

                         QK_GESC,     KC_1,        KC_2,        KC_3,        KC_4,        KC_5,                          KC_6,        KC_7,        KC_8,        KC_9,        KC_0,        KC_EQL,
                         KC_LSFT,     KC_Q,        KC_W,        KC_E,        KC_R,        KC_T,                          KC_Z,        KC_U,        KC_I,        KC_O,        KC_P,        KC_MINS,
                         KC_TAB,      KC_A,        KC_S,        KC_D,        KC_F,        KC_G,                          KC_H,        KC_J,        KC_K,        KC_L,        KC_SCLN,     KC_QUOT,
                         KC_LCTL,     KC_Y,        KC_X,        KC_C,        KC_V,        KC_B,                          KC_N,        KC_M,        KC_COMM,     KC_DOT,      KC_SLSH,     KC_BSLS,
                                                   KC_LBRC,     KC_RBRC,                                                                           KC_PGUP,     KC_PGDN,
                                                                             KC_LSFT,     SC_LSPO,                           LCTL(KC_LBRC),
                                                                                UNREDO,         LOWER,                            SC_LSPO,
                                                                                TO(_GAME),        XXXXXXX,              TG(_PROG),  SC_SENT
                        ),

  [_LOWER] = LAYOUT_5x6_right(

                         KC_TILD,     KC_F1,       KC_F2,       KC_F3,       KC_F4,       KC_F5,                         KC_F6,       KC_F7,       KC_F8,       KC_F9,       KC_F10,      KC_F11,
                         XXXXXXX,     XXXXXXX,     LCTL(KC_W),  RCS(KC_J),   LSG(KC_S),   RCS(KC_K),                     LCTL(KC_Z),  KC_LEFT,     KC_UP,       KC_DOWN,     KC_RGHT,     KC_F12,
                         LGUI(KC_L),  LCTL(KC_A),  UC(0x00DF),  RAISE,       KC_LSFT,     KC_LCBR,                       KC_RCBR,     KC_BTN1,     KC_BTN2,     KC_LEFT,     KC_RGHT,     KC_PIPE,
                         KC_CAPS ,    LCTL(KC_Y),  LCTL(KC_X),  LCTL(KC_C),  LSFT(KC_INS),KC_LPRN,                       KC_RPRN,     LSFT(KC_INS),XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX,
                                                   XXXXXXX,     XXXXXXX,                                                                          XXXXXXX,     XXXXXXX,
                                                                             KC_LSFT,    XXXXXXX,                          LCTL(KC_LBRC),
                                                                                XXXXXXX,       XXXXXXX,                              BSPCDEL,
                                                                                XXXXXXX,       XXXXXXX,               TG(_LOWER),      LGUI(KC_V)
                        ),


  [_RAISE] = LAYOUT_5x6_right(

                         KC_ESC,      KC_ESC,      KC_1,        KC_2,        KC_3,        KC_4,                          TT(CONF),     KC_NUM,      KC_PSLS,     KC_PAST,     KC_PMNS,     KC_CALC,
                         XXXXXXX,     KC_TAB,      KC_Q,        KC_W,        KC_E,        KC_R,                          KC_RBRC,      KC_P7,       KC_P8,       KC_P9,       KC_PPLS,     KC_MUTE,
                         LGUI(KC_L),  LCTL(KC_A),  UC(0x00DF),  XXXXXXX,     KC_LSFT,     KC_LCBR,                      KC_RBRC,      P4BTN1,      P5BTN2,      KC_P6,       COMMDOT,     KC_VOLU,
                         XXXXXXX,     LCTL(KC_Y),  LCTL(KC_X),  LCTL(KC_C),  LSFT(KC_INS),KC_LPRN,                       KC_RPRN,      KC_P1,       KC_P2,       KC_P3,       KC_PEQL,     KC_VOLD,
                                                   XXXXXXX,     XXXXXXX,                                                                            KC_P0,       COMMDOT,
                                                                             KC_LSFT,     KC_LSFT,                          KC_LSFT,
                                                                                KC_LCTL,        ZOOM,                            BSPCDEL,
                                                                                SNIPE,        KC_LCTL,              TG(_RAISE),  LGUI(KC_V)
                        ),


  [_CONF] = LAYOUT_5x6_right(

                         TO(_QWERTZ),  HUELAY1,      HUELAY2,        HUELAY3,        HUELAY4,        HUELAY5,            TimeIdle,    TimeSleep,     XXXXXXX,       XXXXXXX,       XXXXXXX,     EE_CLR,
                         DB_TOGG,      EFFLAY1,      EFFLAY2,        EFFLAY3,        EFFLAY4,        EFFLAY5,            EFFIdle,     XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,     QK_BOOT,
                         RGB_TOG,      XXXXXXX,      SNIPE,          DRAG,           DRAGC,          XXXXXXX,            XXXXXXX,     XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,     XXXXXXX,
                         EESave,       DPIUP,        DPISPUP,        DragScroll,     DragCurser,     XXXXXXX,            XXXXXXX,     XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,     XXXXXXX,
                                                     RGB_SAI,        RGB_VAI,                                                                        XXXXXXX,       XXXXXXX,
                                                                             KC_LSFT,     KC_LSFT,                          XXXXXXX,
                                                                                KC_LCTL,         XXXXXXX,                            XXXXXXX,
                                                                                XXXXXXX,        XXXXXXX,                      JiggleTg,  XXXXXXX
                        ),};
// clang-format onß

/*****************************/
/*  f o r   e n c o d e r s  */
/*****************************/
bool encoder_update_user(uint8_t index, bool clockwise) {
    dprintf("encoder index: %d\n", index);

   switch (biton32(layer_state)) {
            case _QWERTZ:
/////////////////////////////////////////////
                if (index == 1) // master side
    {
        if (shift_held){
            if (clockwise) {
                layer_move(_LOWER);
            } else {
                layer_move(_GAME);
            }
        } else {
            if (clockwise) {
	        charybdis_cycle_pointer_default_dpi_noeeprom(true);
        } else {
            charybdis_cycle_pointer_default_dpi_noeeprom(false);
            }
             }
    }
    if (index == 0) // slave side
    {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    return true;

///////////////////////
                break;
            case _LOWER:
/////////////////////////////////////////////
                if (index == 1) // master side
    {
        if (shift_held){
            if (clockwise) {
                layer_move(_RAISE);
            } else {
                layer_move(_QWERTZ);
            }
        } else {
            if (clockwise) {
	        charybdis_cycle_pointer_sniping_dpi_noeeprom(true);
        } else {
            charybdis_cycle_pointer_sniping_dpi_noeeprom(false);
            }
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
        if (shift_held){
            if (clockwise) {
                layer_move(_PROG);
            } else {
                layer_move(_LOWER);
            }
        } else {
            if (clockwise) {
                //scroll up
            } else {
                //scroll down
            }
        }
    }
    if (index == 0) // slave side
    {
        if (clockwise) {
                //scroll up
            } else {
                //scroll down
        }
    }
    return true;

//////////////////////
    break;
    case _PROG:
//////////////////////////////////////////////
     if (index == 1) // master side
    {
        if (shift_held){
            if (clockwise) {
                layer_move(_GAME);
            } else {
                layer_move(_RAISE);
            }
        } else {
            if (clockwise) {
                //scroll up
                if(user_config3.EE_DragScroll>=15)
                    {
                        user_config3.EE_DragScroll = 0;
                    }else
                    {
                        user_config3.EE_DragScroll = user_config3.EE_DragScroll+1;
                    }

            } else {
                //scroll down
                if (user_config3.EE_DragScroll<=0)
                    {
                        user_config3.EE_DragScroll = 15;
                    }else
                    {
                        user_config3.EE_DragScroll = user_config3.EE_DragScroll-1;
                    }
            }
        }

    }
    if (index == 0) // slave side
    {
        if (clockwise) {
            register_code(KC_LCTL);
			tap_code(KC_PPLS);
			unregister_code(KC_LCTL);
        } else {
			register_code(KC_LCTL);
            tap_code(KC_PMNS);
			unregister_code(KC_LCTL);
        }
    }
    return true;

//////////////////////
                break;
            case _GAME:
//////////////////////////////////////////////
                if (index == 1) // master side
    {
        if (shift_held){
            if (clockwise) {
                layer_move(_QWERTZ);
            } else {
                layer_move(_PROG);
            }
        } else {
            if (clockwise) {
	        charybdis_cycle_pointer_default_dpi_noeeprom(true);
        } else {
            charybdis_cycle_pointer_default_dpi_noeeprom(false);
            }
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

//////////////////////
                break;
            case _CONF:
//////////////////////////////////////////////
                if (index == 1) // master side
    {
        if (shift_held){
            if (clockwise) {

            } else {

            }
        } else {
            if (clockwise) {
	        charybdis_cycle_pointer_default_dpi_noeeprom(true);
        } else {
            charybdis_cycle_pointer_default_dpi_noeeprom(false);
            }
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

//////////////////////
                break;
            default:
//////////////////////////////////////////////
                if (index == 1) // master side
    {
        if (shift_held){
            if (clockwise) {
                layer_move(_QWERTZ);
            } else {
                layer_move(_QWERTZ);
            }
        } else {
            if (clockwise) {
                //scroll up
            } else {
                //scroll down
            }
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

    if (record->event.pressed ) {
        idle_timer = timer_read32();
        sleep_timer = timer_read32();
        idle_mode  = false;
        sleep_mode  = false;

    }

    switch (keycode) {
        // case KC_ESC:
        //     if (record->event.pressed) {
        //         SEND_STRING("Hello, world!\n");ys
        //     }
        //      /* KEYBOARD PET STATUS START */

        case KC_LCTL:
        case KC_RCTL:
            if (record->event.pressed) {
                isSneaking = true;
               // DragScrollX = 255;
            } else {
                isSneaking = false;
               // DragScrollX = 6;
            }
            return true;
        case KC_SPC:
            if (record->event.pressed) {
                isJumping  = true;
            } else {
                isJumping = false;
            }
            return true;
            case SC_LSPO:
            if (record->event.pressed&&current_wpms>MIN_WALK_SPEED) {
                isJumping  = true;
            } else {
                isJumping = false;
            }
            return true;

            /* KEYBOARD PET STATUS END */

        case KC_LSFT:
        //hack to exclude shift from delete keypress
            shift_held = record->event.pressed;
            held_shift = keycode;
            return true;
        case BSPCDEL:
            if (record->event.pressed) {
                if (shift_held) {
                    unregister_code(held_shift);
                    register_code(KC_DEL);
                } else {
                    register_code(KC_BSPC);
                }
            } else {
                unregister_code(KC_DEL);
                unregister_code(KC_BSPC);
                if (shift_held) {
                    register_code(held_shift);
                }
            }
            return false;

            case ZOOM:
            if (record->event.pressed) {
                zoom = true;
                register_code(KC_LCTL);
            } else {
                zoom = false;
                unregister_code(KC_LCTL);
            }
            return false;

            case UNREDO:
            if (record->event.pressed) {
                troughtTime = true;
                register_code(KC_LCTL);
            } else {
                troughtTime = false;
                unregister_code(KC_LCTL);
            }
            return false;

             case P4BTN1:
            if (record->event.pressed) {
                if (shift_held) {
                    unregister_code(held_shift);
                    register_code(KC_BTN1);
                } else {
                    register_code(KC_P4);
                }
            } else {
                unregister_code(KC_BTN1);
                unregister_code(KC_P4);
                if (shift_held) {
                    register_code(held_shift);
                }
            }
            return false;

             case P5BTN2:
            if (record->event.pressed) {
                if (shift_held) {
                    unregister_code(held_shift);
                    register_code(KC_BTN2);
                } else {
                    register_code(KC_P5);
                }
            } else {
                unregister_code(KC_BTN2);
                unregister_code(KC_P5);
                if (shift_held) {
                    register_code(held_shift);
                }
            }
            return false;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////   Config Layer Keycodes ////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            case DPIDWN:
            if (record->event.pressed) {
            if (oneShot==false){

            charybdis_cycle_pointer_default_dpi(false);
            oneShot=true;

            }

            } else {

            oneShot=false;

            }
            return false;

            case DPIUP:
            if (record->event.pressed) {
            if (oneShot==false){

            charybdis_cycle_pointer_default_dpi(true);
            oneShot=true;

            }

            } else {

            oneShot=false;

            }
            return false;

            case DPISPDWN:
            if (record->event.pressed) {
            if (oneShot==false){

            charybdis_cycle_pointer_sniping_dpi(false);
            oneShot=true;

            }

            } else {

            oneShot=false;

            }
            return false;
            case DPISPUP:
            if (record->event.pressed) {
            if (oneShot==false){

            charybdis_cycle_pointer_sniping_dpi(true);
            oneShot=true;

            }

            } else {

            oneShot=false;

            }
            return false;

            case HUELAY1:
            if (record->event.pressed) {
            if (oneShot==false){

                if (shift_held) {
                 rgb_matrix_decrease_hue();
                }else{
                 rgb_matrix_increase_hue();
                }

                oneShot=true;
            }

                } else {

                oneShot=false;

                }
            return false;

            case HUELAY2:
            if (record->event.pressed) {
            if (oneShot==false){

                if (shift_held) {

                    if (user_config.EE_OffsLayer_1<=0)
                    {
                        user_config.EE_OffsLayer_1 = 32;
                    }else
                    {
                        user_config.EE_OffsLayer_1 = user_config.EE_OffsLayer_1-1;
                    }
                }else{
                    if(user_config.EE_OffsLayer_1>=32)
                    {
                        user_config.EE_OffsLayer_1 = 0;
                    }else
                    {
                        user_config.EE_OffsLayer_1 = user_config.EE_OffsLayer_1+1;
                    }
                }
                oneShot=true;
            }

                } else {

                oneShot=false;

                }
            return false;

            case HUELAY3:
            if (record->event.pressed) {
            if (oneShot==false){

                if (shift_held) {

                    if (user_config.EE_OffsLayer_2<=0)
                    {
                        user_config.EE_OffsLayer_2 = 32;
                    }else
                    {
                        user_config.EE_OffsLayer_2 = user_config.EE_OffsLayer_2-1;
                    }
                }else{
                    if(user_config.EE_OffsLayer_2>=32)
                    {
                        user_config.EE_OffsLayer_2 = 0;
                    }else
                    {
                        user_config.EE_OffsLayer_2 = user_config.EE_OffsLayer_2+1;
                    }
                }
                oneShot=true;
            }

                } else {

                oneShot=false;

                }
            return false;

            case HUELAY4:
            if (record->event.pressed) {
            if (oneShot==false){

                if (shift_held) {
                    if (user_config.EE_OffsLayer_3<=0)
                    {
                        user_config.EE_OffsLayer_3 = 32;
                    }else
                    {
                        user_config.EE_OffsLayer_3 = user_config.EE_OffsLayer_3-1;
                    }
                }else{
                    if(user_config.EE_OffsLayer_3>=32)
                    {
                        user_config.EE_OffsLayer_3 = 0;
                    }else
                    {
                        user_config.EE_OffsLayer_3 = user_config.EE_OffsLayer_3+1;
                    }

                }
                oneShot=true;
            }

                } else {

                oneShot=false;

                }
            return false;

            case HUELAY5:
            if (record->event.pressed) {
            if (oneShot==false){

                if (shift_held) {
                    if (user_config.EE_OffsLayer_4<=0)
                    {
                        user_config.EE_OffsLayer_4 = 32;
                    }else
                    {
                        user_config.EE_OffsLayer_4 = user_config.EE_OffsLayer_4-1;
                    }
                }else{
                    if(user_config.EE_OffsLayer_4>=32)
                    {
                        user_config.EE_OffsLayer_4 = 0;
                    }else
                    {
                        user_config.EE_OffsLayer_4 = user_config.EE_OffsLayer_4+1;
                    }

                }
                oneShot=true;
            }

                } else {

                oneShot=false;

                }
            return false;

            case EFFLAY1:
            if (record->event.pressed) {
            if (oneShot==false){
            if (isSneaking){
                LayerEFF=user_config1.EE_EffectL1;
                rgb_matrix_mode_noeeprom(LayerEFF);
            } else {
                if (shift_held) {
                    if (user_config1.EE_EffectL1<=0)
                    {
                        user_config1.EE_EffectL1 = 42;
                    }else
                    {
                        user_config1.EE_EffectL1 = user_config1.EE_EffectL1-1;
                    }
                }else{
                    if(user_config1.EE_EffectL1>=42)
                    {
                        user_config1.EE_EffectL1 = 0;
                    }else
                    {
                        user_config1.EE_EffectL1 = user_config1.EE_EffectL1+1;
                    }

                }
                LayerEFF=user_config1.EE_EffectL1;
                rgb_matrix_mode_noeeprom(LayerEFF);
                oneShot=true;
            }}

                } else {
                oneShot=false;

                }
            return false;

            case EFFLAY2:
            if (record->event.pressed) {
            if (oneShot==false){
            if (isSneaking){
                LayerEFF=user_config1.EE_EffectL2;
                rgb_matrix_mode_noeeprom(LayerEFF);
            } else {
                if (shift_held) {
                    if (user_config1.EE_EffectL2<=0)
                    {
                        user_config1.EE_EffectL2 = 42;
                    }else
                    {
                        user_config1.EE_EffectL2 = user_config1.EE_EffectL2-1;
                    }
                }else{
                    if(user_config1.EE_EffectL2>=42)
                    {
                        user_config1.EE_EffectL2 = 0;
                    }else
                    {
                        user_config1.EE_EffectL2 = user_config1.EE_EffectL2+1;
                    }

                }
                LayerEFF=user_config1.EE_EffectL2;
                rgb_matrix_mode_noeeprom(LayerEFF);
                oneShot=true;
            }}

                } else {

                oneShot=false;

                }
            return false;

            case EFFLAY3:
            if (record->event.pressed) {
            if (oneShot==false){
            if (isSneaking){
                LayerEFF=user_config1.EE_EffectL3;
                rgb_matrix_mode_noeeprom(LayerEFF);
            } else {
                if (shift_held) {
                    if (user_config1.EE_EffectL3<=0)
                    {
                        user_config1.EE_EffectL3 = 42;
                    }else
                    {
                        user_config1.EE_EffectL3 = user_config1.EE_EffectL3-1;
                    }
                }else{
                    if(user_config1.EE_EffectL3>=42)
                    {
                        user_config1.EE_EffectL3 = 0;
                    }else
                    {
                        user_config1.EE_EffectL3 = user_config1.EE_EffectL3+1;
                    }

                }
                LayerEFF=user_config1.EE_EffectL3;
                rgb_matrix_mode_noeeprom(LayerEFF);
                oneShot=true;
            }}

                } else {

                oneShot=false;

                }
            return false;

            case EFFLAY4:
            if (record->event.pressed) {
            if (oneShot==false){
            if (isSneaking){
                LayerEFF=user_config1.EE_EffectL4;
                rgb_matrix_mode_noeeprom(LayerEFF);
            } else {
                if (shift_held) {
                    if (user_config1.EE_EffectL4<=0)
                    {
                        user_config1.EE_EffectL4 = 42;
                    }else
                    {
                        user_config1.EE_EffectL4 = user_config1.EE_EffectL4-1;
                    }
                }else{
                    if(user_config1.EE_EffectL4>=42)
                    {
                        user_config1.EE_EffectL4 = 0;
                    }else
                    {
                        user_config1.EE_EffectL4 = user_config1.EE_EffectL4+1;
                    }

                }
                LayerEFF=user_config1.EE_EffectL4;
                rgb_matrix_mode_noeeprom(LayerEFF);
                oneShot=true;
            }}

                } else {

                oneShot=false;

                }
            return false;

            case EFFLAY5:
            if (record->event.pressed) {
            if (oneShot==false){
            if (isSneaking){
                LayerEFF=user_config2.EE_EffectL5;
                rgb_matrix_mode_noeeprom(LayerEFF);
            } else {
                if (shift_held) {
                    if (user_config2.EE_EffectL5<=0)
                    {
                        user_config2.EE_EffectL5 = 42;
                    }else
                    {
                        user_config2.EE_EffectL5 = user_config2.EE_EffectL5-1;
                    }
                }else{
                    if(user_config2.EE_EffectL5>=42)
                    {
                        user_config2.EE_EffectL5 = 0;
                    }else
                    {
                        user_config2.EE_EffectL5 = user_config2.EE_EffectL5+1;
                    }

                }
                LayerEFF=user_config2.EE_EffectL5;
                rgb_matrix_mode_noeeprom(LayerEFF);
                oneShot=true;
            }}

                } else {

                oneShot=false;

                }
            return false;

            case EFFIdle:
            if (record->event.pressed) {
            if (oneShot==false){
            if (isSneaking){
                LayerEFF=user_config2.EE_EffectIdle;
                rgb_matrix_mode_noeeprom(LayerEFF);
            } else {
                if (shift_held) {
                    if (user_config2.EE_EffectIdle<=0)
                    {
                        user_config2.EE_EffectIdle = 42;
                    }else
                    {
                        user_config2.EE_EffectIdle = user_config2.EE_EffectIdle-1;
                    }
                }else{
                    if(user_config2.EE_EffectIdle>=42)
                    {
                        user_config2.EE_EffectIdle = 0;
                    }else
                    {
                        user_config2.EE_EffectIdle = user_config2.EE_EffectIdle+1;
                    }

                }
                LayerEFF=user_config2.EE_EffectIdle;
                rgb_matrix_mode_noeeprom(LayerEFF);
                oneShot=true;
            }}

                } else {

                oneShot=false;

                }
            return false;




            case SNIPE:
            if (record->event.pressed) {
                snipe = true;
                drag = false;
            } else {
                snipe = false;
            }
            return false;

            case DRAG:
            if (record->event.pressed) {
                drag = true;
                snipe = false;
            } else {
                drag = false;
            }
            return false;

            case DRAGC:
            if (record->event.pressed) {
                dragc = true;
                snipe = false;
                drag = false;
            } else {
                dragc = false;
            }
            return false;

            case DragScroll:
            if (record->event.pressed) {
            if (oneShot==false){

                if (shift_held) {
                    if (user_config3.EE_DragScroll<=0)
                    {
                        user_config3.EE_DragScroll = 15;
                    }else
                    {
                        user_config3.EE_DragScroll = user_config3.EE_DragScroll-1;
                    }
                }else{
                    if(user_config3.EE_DragScroll>=15)
                    {
                        user_config3.EE_DragScroll = 0;
                    }else
                    {
                        user_config3.EE_DragScroll = user_config3.EE_DragScroll+1;
                    }

                }
                oneShot=true;
            }

                } else {

                oneShot=false;

                }
            return false;

            case DragCurser:
            if (record->event.pressed) {
            if (oneShot==false){

                if (shift_held) {
                    if (user_config3.EE_DragCurser<=0)
                    {
                        user_config3.EE_DragCurser = 15;
                    }else
                    {
                        user_config3.EE_DragCurser = user_config3.EE_DragCurser-1;
                    }
                }else{
                    if(user_config3.EE_DragCurser>=15)
                    {
                        user_config3.EE_DragCurser = 0;
                    }else
                    {
                        user_config3.EE_DragCurser = user_config3.EE_DragCurser+1;
                    }

                }
                oneShot=true;
            }

                } else {

                oneShot=false;

                }
            return false;


            case COMMDOT:
            if (record->event.pressed) {
             if (shift_held) {
                    unregister_code(held_shift);
                    register_code(KC_DOT);
                } else {
                    register_code(KC_COMM);
                }
            } else {
                unregister_code(KC_DOT);
                unregister_code(KC_COMM);
                if (shift_held) {
                    register_code(held_shift);
                }
            }
            return false;

            case  EESave:
            if (record->event.pressed) {
            if (oneShot==false){
               eeconfig_update_user(user_config.raw,user_config1.raw,user_config2.raw,user_config3.raw);
               oneShot=true;
                }
            } else {
                oneShot=false;


            }
            return false;

            case TimeIdle:
            if (record->event.pressed) {
            if (oneShot==false){
            if (isSneaking){
                LayerEFF=user_config2.EE_TimeIdle;
                rgb_matrix_mode_noeeprom(LayerEFF);
            } else {
                if (shift_held) {
                    if (user_config2.EE_TimeIdle<=0)
                    {
                        user_config2.EE_TimeIdle = 99;
                    }else
                    {
                        user_config2.EE_TimeIdle = user_config2.EE_TimeIdle-1;
                    }
                }else{
                    if(user_config2.EE_TimeIdle>=99)
                    {
                        user_config2.EE_TimeIdle = 0;
                    }else
                    {
                        user_config2.EE_TimeIdle = user_config2.EE_TimeIdle+1;
                    }

                }
                oneShot=true;
            }}

                } else {

                oneShot=false;

                }
            return false;

            case TimeSleep:
            if (record->event.pressed) {
            if (oneShot==false){
            if (isSneaking){
                LayerEFF=user_config2.EE_TimeSleep;
                rgb_matrix_mode_noeeprom(LayerEFF);
            } else {
                if (shift_held) {
                    if (user_config2.EE_TimeSleep<=0)
                    {
                        user_config2.EE_TimeSleep = 180;
                    }else
                    {
                        user_config2.EE_TimeSleep = user_config2.EE_TimeSleep-1;
                    }
                }else{
                    if(user_config2.EE_TimeSleep>=180)
                    {
                        user_config2.EE_TimeSleep = 0;
                    }else
                    {
                        user_config2.EE_TimeSleep = user_config2.EE_TimeSleep+1;
                    }

                }
                oneShot=true;
            }}

                } else {

                oneShot=false;

                }
            return false;

            case JiggleTg:
            if (record->event.pressed) {
            if (oneShot==false){
            if (isJiggle){
                isJiggle = false;
            } else {
                isJiggle = true;
                oneShot=true;
            }}

                } else {

                oneShot=false;

                }
            return false;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////   End Config Layer Keycodes ////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
default:
      return true; // Process all other keycodes normally
    }
    // If console is enabled, it will print the matrix position and status of each key pressed
#ifdef CONSOLE_ENABLE
    dprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif
    return true;

}





void matrix_scan_user(void) {

    //read chip timer to generate random number for cat idle blinks
    ANIM_FRAME_DURATION1_OLD=timer_read32()%10;

    // idle_timer needs to be set one time
    if (idle_timer == 0) idle_timer = timer_read32();
    if (sleep_timer == 0) sleep_timer = timer_read32();
    if (timer_elapsed32(idle_timer) > user_config2.EE_TimeIdle * 1000 && !idle_mode && user_config2.EE_TimeIdle > 0) {
        idle_mode  = true;
        idle_timer = timer_read32();
    }
    //Bc I want to use not a new timer and only want to prevent the oled from burning in I used this dirty hack its rou
    if (timer_elapsed32(sleep_timer) > (user_config2.EE_TimeIdle + (user_config2.EE_TimeSleep * 10)) * 1000 && !sleep_mode && user_config2.EE_TimeSleep > 0) {
        print("sleep 1\n");
        sleep_mode  = true;
        sleep_timer = timer_read32();
    }
    idle_function();
    sleep_function();
    jiggle_function();

}

void idle_function(void) {
    static bool last_state_idle = false;
    if (idle_mode && !last_state_idle) { // rising edge of idle mode
        old_rgb_mode = rgb_matrix_get_mode();
                    print("sleep 2\n");
        dprintf("%i Status sleep\n",sleep_mode);
        rgb_matrix_mode_noeeprom(user_config2.EE_EffectIdle);
    }
    if (!idle_mode && last_state_idle) { // falling edge of idle mode
        rgb_matrix_mode_noeeprom(old_rgb_mode);

    }
    last_state_idle = idle_mode;
}

//To power down my oled when my pc is in sleep mode.
void sleep_function(void) {

    static bool last_state_sleep = false;
    if (sleep_mode && !last_state_sleep) { // rising edge of idle mode
        rgb_matrix_mode_noeeprom(RGB_MATRIX_SLEEP_MODE);

    }
    if (!sleep_mode && last_state_sleep) { // falling edge of idle mode
        rgb_matrix_mode_noeeprom(old_rgb_mode);
    }
    last_state_sleep = sleep_mode;
}
void jiggle_function(void) {
uint8_t jiggle = timer_read32()%10;
    if (isJiggle && jiggle == 5) { // rising edge of idle mode

  tap_code(KC_MS_UP);
    }

    if (isJiggle && jiggle == 6) { // rising edge of idle mode
  tap_code(KC_MS_DOWN);
    }

        if (isJiggle && jiggle == 8) { // rising edge of idle mode
  tap_code(KC_MS_LEFT);
    }

        if (isJiggle && jiggle == 3) { // rising edge of idle mode
  tap_code(KC_MS_RIGHT);
    }

}
