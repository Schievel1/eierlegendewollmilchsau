#include QMK_KEYBOARD_H

#include "rgb_matrix_user.h"
#include "keymap.h"

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
// clang-format on

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
#endif
