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

#include "f411.h"

void keyboard_pre_init_sub(void) { setPinInputHigh(A0); }

void matrix_scan_sub_kb(void) {
    if (!readPin(A0)) {
        reset_keyboard();
    }
}

__attribute__((weak)) void bootmagic_lite(void) {
    // We need multiple scans because debouncing can't be turned off.
    matrix_scan();
#if defined(DEBOUNCE) && DEBOUNCE > 0
    wait_ms(DEBOUNCE * 2);
#else
    wait_ms(30);
#endif
    matrix_scan();

    uint8_t row = BOOTMAGIC_LITE_ROW;
    uint8_t col = BOOTMAGIC_LITE_COLUMN;

#if defined(SPLIT_KEYBOARD) && defined(BOOTMAGIC_LITE_ROW_RIGHT) && defined(BOOTMAGIC_LITE_COLUMN_RIGHT)
    if (!is_keyboard_left()) {
        row = BOOTMAGIC_LITE_ROW_RIGHT;
        col = BOOTMAGIC_LITE_COLUMN_RIGHT;
    }
#endif

    if (matrix_get_row(row) & (1 << col) || !readPin(A0)) {
        eeconfig_disable();
        bootloader_jump();
    }
}


#ifdef USB_VBUS_PIN
bool usb_vbus_state(void) {
    setPinInputLow(USB_VBUS_PIN);
    wait_us(5);
    return readPin(USB_VBUS_PIN);
}
#endif

void matrix_output_unselect_delay(uint8_t line, bool key_pressed) {
    for (int32_t i = 0; i < 40; i++) {
        __asm__ volatile("nop" ::: "memory");
    }
}

// clang-format off
#ifdef RGB_MATRIX_ENABLE
led_config_t g_led_config = { {
    { 5, 4, 3, 2, 1, 0 },
    { 11,10,9,8,7,6 },
    { 17,16,15,14,13,12},
    { 23,22,21,20,19,18},
    { NO_LED, NO_LED, 24, 25, 26, 27},
    { NO_LED, NO_LED, 30, 31, 29, 28},

    { 37,36,35,34,33,32} ,
    { 43,42,41,40,39,38} ,
    { 49,48,47,46,45,44} ,
    { 55,54,53,52,51,50} ,
    { NO_LED ,58,57,56,NO_LED ,NO_LED } ,
    { NO_LED ,59,61,60,NO_LED ,NO_LED }
}, {

          { 86,   0 },{ 69,   0 },{ 52 ,   0 },{   34,   0 },{ 17,    0 },{ 0,    0 },
          { 86,   8 },{ 69,   8 },{ 52,    8 },{   34,   8 },{ 17,    8 },{ 0,    8 },
          { 86,   16 },{ 69,   16 },{ 52,    16 },{   34,   16 },{ 17,   16 },{ 0,    16 },
          { 86,   24 },{ 69,   24 },{ 52,    24 },{   34,   24 },{ 17,   24 },{ 0,    24 },
                            {   34,   32 }, { 52,    32 }, { 69,   32 },  { 86,   32 },
                                                               { 95,   38 },  { 84,   40 },
                                                               { 100,   46 },  { 86,   48 },

            { 189,   0 },{ 172,   0 },{  155 ,   0 },{ 138,   0 },{ 121 ,   0 },{ 103,    0 },
            { 189,   8 },{ 172,   8 },{  155,    8 },{ 138,   8 },{ 121,   8 },{ 103,    8 },
            { 189,   16 },{ 172,   16 },{ 155,    16 },{ 138,   16 },{ 121,   16 },{ 103,    16 },
            { 189,   24 },{ 172,   24 },{ 155,    24 },{ 138,   24 },{ 121,   24 },{ 103,    24 },
            { 189,   32 },              { 155,   32 },{ 138,    32 },
            { 189,   40 },
      { 224,   48 },  { 206,   48 }
}, {
     1, 1, 1 , 1 , 1, 1,
     1 ,1 ,1 ,1 ,1 , 1,
     1 ,1 ,1 ,1 ,1 , 1,
     1 ,1 ,1 ,1 ,1 , 1,
     1 ,1 ,1 ,1 ,
     1 ,1 ,1 ,1,

     1 , 1 , 1 , 1 , 1 , 1  ,
     1 ,1 ,1 ,1 ,1 , 1,
     1 ,1 ,1 ,1 ,1 , 1,
     1 ,1 ,1 ,1 ,1 , 1,
     1 , 1 , 1,
     1 , 1 , 1
}
};
#endif
// clang-format on
