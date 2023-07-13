#include QMK_KEYBOARD_H
#include "rgb_matrix_user.h"
#include "keymap.h"

// clang-format off
#ifdef RGB_MATRIX_ENABLE



led_config_t g_led_config = { {
   //left
   {0, 1, 2, 3, 4, 5 },
    { 11,10,9,8,7,6 },
    { 12,13,14,15,16,17},
    { 23,22,21,20,19,18},
    { NO_LED, NO_LED, 24, 25, 26, 27},
    { NO_LED, NO_LED, 29, 30, 28, 31},
   //right
    { 32,33,34,35,36,37} ,
    { 43,42,41,40,39,38} ,
    { 44,45,46,47,48,49} ,
    { 55,54,53,52,51,50} ,
    { NO_LED ,59,60,61,NO_LED ,NO_LED } ,
    { NO_LED ,58,56,57,NO_LED ,NO_LED }
}, {

          { 0,   0 },{ 17,   0 },{ 34 ,   0 },{   52,   0 },{ 69,    0 },{ 86,    0 },
          { 86,   8 },{ 69,   8 },{ 52,    8 },{   34,   8 },{ 17,    8 },{ 0,    8 },
          { 0,   16 },{ 17,   16 },{ 34,    16 },{   52,   16 },{ 69,   16 },{ 86,    16 },
          { 86,   24 },{ 69,   24 },{ 52,    24 },{   34,   24 },{ 17,   24 },{ 0,    24 },
                            {   34,   32 }, { 52,    32 }, { 69,   32 },  { 86,   32 },
                                                               { 86,   56 },  { 86,   64 },
                                                               { 100,   64 },  { 100,   56 },

            { 103,   0 },{ 121,   0 },{  138 ,   0 },{ 155,   0 },{ 172 ,   0 },{ 189,    0 },
            { 189,   8 },{ 172,   8 },{  155,    8 },{ 138,   8 },{ 121,   8 },{ 103,    8 },
            { 103,   16 },{ 121,   16 },{ 138,    16 },{ 155,   16 },{ 172,   16 },{ 189,    16 },
            { 189,   24 },{ 172,   24 },{ 155,    24 },{ 138,   24 },{ 121,   24 },{ 103,    24 },
            { 100,   64 },              { 103,   56 },{ 115,    48 },
            { 103,   38 },
      { 138,   32 },  { 155,    32 }
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
    static bool re_game = false;
    switch (biton32(layer_state)) {
        case _RAISE:
            if (!re_raise) {
                for (int i = 0; i < OffsLayer_2; i++) {
                    rgb_matrix_increase_hue_noeeprom();
                }
            }
            re_raise = true;
            led_t led_state = host_keyboard_led_state();
           if (led_state.num_lock){
               rgb_matrix_set_color(33, RGB_RED);
             }else{rgb_matrix_set_color(33, RGB_WHITE);}

            // set num block keys to static color
            //rgb_matrix_set_color(33, RGB_RED); // num lock
            rgb_matrix_set_color(42, RGB_GREEN); // numbers
            rgb_matrix_set_color(41, RGB_GREEN);
            rgb_matrix_set_color(40, RGB_GREEN);
            rgb_matrix_set_color(47, RGB_GREEN);
            rgb_matrix_set_color(46, RGB_GREEN);
            rgb_matrix_set_color(45, RGB_GREEN);
            rgb_matrix_set_color(55, RGB_GREEN);
            rgb_matrix_set_color(54, RGB_GREEN);
            rgb_matrix_set_color(53, RGB_GREEN);
            rgb_matrix_set_color(52, RGB_GREEN);
            rgb_matrix_set_color(36, RGB_BLUE); // operators
            rgb_matrix_set_color(35, RGB_BLUE);
            rgb_matrix_set_color(34, RGB_BLUE);
            rgb_matrix_set_color(39, RGB_BLUE);
            rgb_matrix_set_color(32, RGB_OFF);// Blank keys
            rgb_matrix_set_color(37, RGB_OFF);
            rgb_matrix_set_color(43, RGB_OFF);
            rgb_matrix_set_color(38, RGB_OFF);
            rgb_matrix_set_color(44, RGB_OFF);
            rgb_matrix_set_color(48, RGB_OFF);
            rgb_matrix_set_color(49, RGB_OFF);
            rgb_matrix_set_color(50, RGB_OFF);
            rgb_matrix_set_color(51, RGB_OFF);
            rgb_matrix_set_color(56, RGB_OFF);
            rgb_matrix_set_color(57, RGB_OFF);
            rgb_matrix_set_color(58, RGB_OFF);
            rgb_matrix_set_color(59, RGB_OFF);
            rgb_matrix_set_color(60, RGB_OFF);
            rgb_matrix_set_color(61, RGB_OFF);
		// equals
            if (re_lower) {
                for (int i = 0; i < OffsLayer_1; i++) {
                    rgb_matrix_decrease_hue_noeeprom();
                }
                re_lower = false;
            }
            if (re_game) {
                for (int i = 0; i < OffsLayer_3; i++) {
                    rgb_matrix_decrease_hue_noeeprom();
                }
                re_game = false;
            }
            break;

        case _LOWER:
            if (!re_lower) {
                for (int i = 0; i < OffsLayer_1; i++) {
                    rgb_matrix_increase_hue_noeeprom();
                }
            }
            if (re_raise) {
                for (int i = 0; i < OffsLayer_2; i++) {
                    rgb_matrix_decrease_hue_noeeprom();
                }
                re_raise = false;
            }
            if (re_game) {
                for (int i = 0; i < OffsLayer_3; i++) {
                    rgb_matrix_decrease_hue_noeeprom();
                }
                re_game = false;
            }
            re_lower = true;
            break;

        case _GAME:
            if (!re_game) {
                for (int i = 0; i < OffsLayer_3; i++) {
                    rgb_matrix_increase_hue_noeeprom();
                }
            }
            if (re_raise) {
                for (int i = 0; i < OffsLayer_2; i++) {
                    rgb_matrix_decrease_hue_noeeprom();
                }
                re_raise = false;
            }
            if (re_lower) {
                for (int i = 0; i < OffsLayer_1; i++) {
                    rgb_matrix_decrease_hue_noeeprom();
                }
                re_lower = false;
            }

            re_game = true;
            break;

        case _CONF:
            if (re_lower) {
                for (int i = 0; i < OffsLayer_1; i++) {
                    rgb_matrix_decrease_hue_noeeprom();
                }
            }
            if (re_raise) {
                for (int i = 0; i < OffsLayer_2; i++) {
                    rgb_matrix_decrease_hue_noeeprom();
                }
            }
            if (re_game) {
                for (int i = 0; i < OffsLayer_3; i++) {
                    rgb_matrix_decrease_hue_noeeprom();
                }
            }
            re_raise = false;
            re_lower = false;
            re_game = false;
            rgb_matrix_set_color_all(RGB_OFF);
            HSV      hsv      = rgb_matrix_get_hsv();
            RGB      rgb      = hsv_to_rgb(hsv);
            rgb_matrix_set_color(1,rgb.r,rgb.g,rgb.b);

             for (int i = 0; i < OffsLayer_1; i++) {
                    rgb_matrix_increase_hue_noeeprom();
                }
                  hsv      = rgb_matrix_get_hsv();
                  rgb      = hsv_to_rgb(hsv);
            rgb_matrix_set_color(2,rgb.r,rgb.g,rgb.b);
            for (int i = 0; i < OffsLayer_1; i++) {
                    rgb_matrix_decrease_hue_noeeprom();
                }

            for (int i = 0; i < OffsLayer_2; i++) {
                    rgb_matrix_increase_hue_noeeprom();
                }
                  hsv      = rgb_matrix_get_hsv();
                  rgb      = hsv_to_rgb(hsv);
            rgb_matrix_set_color(3,rgb.r,rgb.g,rgb.b);
            for (int i = 0; i < OffsLayer_2; i++) {
                    rgb_matrix_decrease_hue_noeeprom();
                }

            for (int i = 0; i < OffsLayer_3; i++) {
                    rgb_matrix_increase_hue_noeeprom();
                }
                  hsv      = rgb_matrix_get_hsv();
                  rgb      = hsv_to_rgb(hsv);
            rgb_matrix_set_color(4,rgb.r,rgb.g,rgb.b);
            for (int i = 0; i < OffsLayer_3; i++) {
                    rgb_matrix_decrease_hue_noeeprom();
                }

            break;

        case _QWERTZ:
            if (re_lower) {
                for (int i = 0; i < OffsLayer_1; i++) {
                    rgb_matrix_decrease_hue_noeeprom();
                }
            }
            if (re_raise) {
                for (int i = 0; i < OffsLayer_2; i++) {
                    rgb_matrix_decrease_hue_noeeprom();
                }
            }
            if (re_game) {
                for (int i = 0; i < OffsLayer_3; i++) {
                    rgb_matrix_decrease_hue_noeeprom();
                }
            }
            re_raise = false;
            re_lower = false;
            re_game = false;

        default:
            break;
    }
    return false;
}
#endif
