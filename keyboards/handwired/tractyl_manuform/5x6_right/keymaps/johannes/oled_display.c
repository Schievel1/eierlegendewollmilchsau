#include QMK_KEYBOARD_H

#include "rgb_matrix_user.h"
#include "keymap.h"

void oled_idle_task0(void);
void oled_idle_task1(void);
void oled_idle_task2(void);
void oled_idle_task3(void);
void oled_idle_task4(void);
void oled_idle_task5(void);


/* timers */
uint32_t anim_timer = 0;

/* current frame */
uint8_t current_frame = 0;



#ifdef OLED_ENABLE
#define ANIM_FRAME_DURATION 120
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

bool oled_task_user(void) {
    if (!idle_mode) {
        /* Host Keyboard Layer Status */
		oled_clear();
	    oled_set_cursor(0, 0);
        oled_write_P(PSTR("\nLayer"), false);
        switch (biton32(layer_state)) {
            case _DVORAK:
                oled_write_P(PSTR("QWRTZ"), false);
                break;
            case _LOWER:
                oled_write_P(PSTR("LOWER"), false);
                break;
            case _RAISE:
                oled_write_P(PSTR("RAISE"), false);
                break;
            default:
                // Or use the write_ln shortcut over adding '\n' to the end of your string
                oled_write_ln_P(PSTR("Undef"), false);
        }
        oled_write_P(PSTR("\nRGB:\n"), false);
        if (!(rgb_matrix_is_enabled())) {
            oled_write_P(PSTR("Off\n"), false);
        } else {
            switch (rgb_matrix_get_mode()) {
                case MY_RGB_MATRIX_SOLID_COLOR:
                    oled_write_P(PSTR("Solid\n"), false);
                    break;
                case MY_RGB_MATRIX_ALPHAS_MODS:
                    oled_write_P(PSTR("AlphsMods\n"), false);
                    break;
                case MY_RGB_MATRIX_GRADIENT_UP_DOWN:
                    oled_write_P(PSTR("GrdntUpDwn"), false);
                    break;
                case MY_RGB_MATRIX_GRADIENT_LEFT_RIGHT:
                    oled_write_P(PSTR("GrdntLtRt\n"), false);
                    break;
                case MY_RGB_MATRIX_BREATHING:
                    oled_write_P(PSTR("Brthg\n"), false);
                    break;
                case MY_RGB_MATRIX_BAND_SAT:
                    oled_write_P(PSTR("BaSat\n"), false);
                    break;
                case MY_RGB_MATRIX_BAND_VAL:
                    oled_write_P(PSTR("BaVAL\n"), false);
                    break;
                case MY_RGB_MATRIX_BAND_PINWHEEL_SAT:
                    oled_write_P(PSTR("BaSatPinwl"), false);
                    break;
                case MY_RGB_MATRIX_BAND_PINWHEEL_VAL:
                    oled_write_P(PSTR("BaVALPinwl"), false);
                    break;
                case MY_RGB_MATRIX_BAND_SPIRAL_SAT:
                    oled_write_P(PSTR("BaSatSpral"), false);
                    break;
                case MY_RGB_MATRIX_BAND_SPIRAL_VAL:
                    oled_write_P(PSTR("BaVALSpral"), false);
                    break;
                case MY_RGB_MATRIX_CYCLE_ALL:
                    oled_write_P(PSTR("CycleAll\n"), false);
                    break;
                case MY_RGB_MATRIX_CYCLE_LEFT_RIGHT:
                    oled_write_P(PSTR("CycleLtRt\n"), false);
                    break;
                case MY_RGB_MATRIX_CYCLE_UP_DOWN:
                    oled_write_P(PSTR("CycleUpDwn"), false);
                    break;
                case MY_RGB_MATRIX_CYCLE_OUT_IN:
                    oled_write_P(PSTR("CycleOutIn"), false);
                    break;
                case MY_RGB_MATRIX_CYCLE_OUT_IN_DUAL:
                    oled_write_P(PSTR("CycleOtIN2"), false);
                    break;
                case MY_RGB_MATRIX_RAINBOW_MOVING_CHEVRON:
                    oled_write_P(PSTR("RnbowMvChvn"), false);
                    break;
                case MY_RGB_MATRIX_CYCLE_PINWHEEL:
                    oled_write_P(PSTR("CyclePinwl"), false);
                    break;
                case MY_RGB_MATRIX_CYCLE_SPIRAL:
                    oled_write_P(PSTR("CycleSpral"), false);
                    break;
                case MY_RGB_MATRIX_DUAL_BEACON:
                    oled_write_P(PSTR("Dual\nBeacn"), false);
                    break;
                case MY_RGB_MATRIX_RAINBOW_BEACON:
                    oled_write_P(PSTR("RnbowBeacn"), false);
                    break;
                case MY_RGB_MATRIX_RAINBOW_PINWHEELS:
                    oled_write_P(PSTR("RnbowPinwl"), false);
                    break;
                case MY_RGB_MATRIX_RAINDROPS:
                    oled_write_P(PSTR("Rain\ndrops"), false);
                    break;
                case MY_RGB_MATRIX_JELLYBEAN_RAINDROPS:
                    oled_write_P(PSTR("JlybnRndps"), false);
                    break;
                case MY_RGB_MATRIX_HUE_BREATHING:
                    oled_write_P(PSTR("Hue\nBrthg"), false);
                    break;
                case MY_RGB_MATRIX_HUE_PENDULUM:
                    oled_write_P(PSTR("Hue\nPend\n"), false);
                    break;
                case MY_RGB_MATRIX_HUE_WAVE:
                    oled_write_P(PSTR("Hue\nWave\n"), false);
                    break;
#    if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS)
                case MY_RGB_MATRIX_TYPING_HEATMAP:
                    oled_write_P(PSTR("TypngHtmap"), false);
                    break;
                case MY_RGB_MATRIX_DIGITAL_RAIN:
                    oled_write_P(PSTR("DgtalRain\n"), false);
                    break;
#    endif
#    if defined(RGB_MATRIX_KEYPRESSES) || defined(RGb_MATRIX_KEYRELEASES)
                case MY_RGB_MATRIX_SOLID_REACTIVE_SIMPLE:
                    oled_write_P(PSTR("SolidRtSmp"), false);
                    break;
                case MY_RGB_MATRIX_SOLID_REACTIVE:
                    oled_write_P(PSTR("SolidReact"), false);
                    break;
                case MY_RGB_MATRIX_SOLID_REACTIVE_WIDE:
                    oled_write_P(PSTR("SolidRtWde"), false);
                    break;
                case MY_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE:
                    oled_write_P(PSTR("SolidRtMtw"), false);
                    break;
                case MY_RGB_MATRIX_SOLID_REACTIVE_CROSS:
                    oled_write_P(PSTR("SolidRtCrs"), false);
                    break;
                case MY_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS:
                    oled_write_P(PSTR("SolidRtMtc"), false);
                    break;
                case MY_RGB_MATRIX_SOLID_REACTIVE_NEXUS:
                    oled_write_P(PSTR("SolidRtNEX"), false);
                    break;
                case MY_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS:
                    oled_write_P(PSTR("SolidRtMtx"), false);
                    break;
                case MY_RGB_MATRIX_SPLASH:
                    oled_write_P(PSTR("Splsh\n"), false);
                    break;
                case MY_RGB_MATRIX_MULTISPLASH:
                    oled_write_P(PSTR("MultiSplsh"), false);
                    break;
                case MY_RGB_MATRIX_SOLID_SPLASH:
                    oled_write_P(PSTR("SolidSplsh"), false);
                    break;
                case MY_RGB_MATRIX_SOLID_MULTISPLASH:
                    oled_write_P(PSTR("SolidMtSph"), false);
                    break;
#    endif
                case MY_RGB_MATRIX_EFFECT_MAX:
                    break;
                    oled_write_P(PSTR("MtrixEffMx"), false);
                default:
                    oled_write_P(PSTR("Undef\n"), false);
            }
        }
        /* Host Keyboard LED Status */
        led_t led_state = host_keyboard_led_state();
        oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR(""), false);
        oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR(""), false);
        oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR(""), false);
    } else {

        /***********************/
       /*  i d l e   m o d e oled  */
      /***********************/

if (timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
        anim_timer = timer_read32();
        if (current_frame < 5) {
        current_frame = current_frame + 1;
    }else {
        current_frame = 0;
        }

            switch (current_frame) {
                case 0:    
        oled_idle_task0();
    break;
 case 1:    
        oled_idle_task1();
    break;
 case 2:    
        oled_idle_task2();
    break;
 case 3:    
        oled_idle_task3();
    break;
 case 4:    
        oled_idle_task4();
    break;
 case 5:    
        oled_idle_task5();
    break;
default:
                    oled_off();
            }
        }
    }
    return false;
}

/***********************/
/*  i d l e   a n i m a t i o n frames*/
/***********************/

void oled_idle_task0() {

 
     static const char PROGMEM raw_logo[] = {
        0,  0,  0,  8,  0, 64,  0,  8,  0,  0,  0, 64,  0, 10,  0,128,  0,  0,  0,  0, 16,  0,  0, 32,  0,  0,  0,128,  0,  8,  0,  0,  0,  0,  0, 32,  0,  0,  8,  0,  0,  0,130,  0,  0,  0, 16,  0,  0,  0,  0, 32,  0,128,  0,  0,  0,  0,  0, 32,  0,128,  0,  0,  0,  0,  0,  8,  0,  0,  0,  0,  0,  8,  0,  0, 32,  0,  2,  0,  0,  0,  8,  0,  0,  0,  0,  0,  0,  0, 32,  0,  0,  0,132,  0,  0,  0, 64,  0, 32,  0,  1,  0,  2,  0,  0,  0, 16,  0,  0,  1,  0,  0, 64,  0,  0,  8,  0,  0,  0,  2,  0,128,  8,  0,  0,  0,
        0,  0,  0,  0,  8,  0,  0,114, 32, 32,112, 14,  4,  4, 14,  0,  0,  0,  0, 32,  0,  2,  0,  0,  0,  0,128, 16,  0,  2,  0, 32,  0,  0,  0,126, 24, 24, 60, 60, 24, 24,126,  0,  0,  4,  0,  0,130,  0,  0,  8,  0,129,  0,  0,  0, 32,  0,  0,  0, 32,  0,  0,  0,  0,  0,  8,128,192, 97, 48, 16, 24, 56,120,248,248,248,240,208, 64,  0,  0,  0,  0, 32,  0,  0,  0,  8,  0,  0, 64,  1,  0,  0,  0,216,223,223,223,222,220,216,216,220,222,223,223,223,223,223,221,221,220,216,216,216,  0,  0,  2,128, 16,  0,  0,  8, 32, 
        0,  0,  0,  7, 15, 31, 63,127,127,127,255,255,255,255,191, 63, 59, 27, 25,  9, 17,  0,  0,  0, 16,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0, 64,  2,  0,  0,  0,  0, 36,  0,  0,  0, 16,  0, 64,  2,  0,  0,  0,  0,136,128,  0,  0,  0,  8,  0,  0,  0, 33,  0,  0,  0,  0,  0,224, 48,224,  0,  0,  4,  0,  0,224, 16,144,144,144,144,144, 30,239,  3,  0,  0,  0,224,144,224,  0,  4,  0,  0,  0,  0,128,  0,  3, 13, 48, 67,172, 16, 32,240,127, 48, 33, 33, 33,161, 33, 48,127,240, 16,  8,150, 97, 56,  7,  0, 32,  0,  0, 
        0,  0,  0,  0,  0, 32,  0,  0,  0,  1,  2,255,192,194,153,175,142,158,147,200,192,255,130,  1, 32,  0,  0,  0,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8, 22,243, 32, 32, 17, 17,201, 17, 17, 32, 32,241, 18, 13,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,135,252,162,162,255,  0,255,162,162,252,131,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    };
    oled_clear();
    
//oled_write_P(qmk_logo, false);
    oled_write_raw_P(raw_logo,sizeof(raw_logo));


}

void oled_idle_task1() {
   
 
     static const char PROGMEM raw_logo[] = {
         0,  0,  0,  8,  0, 64,  0,  8,  0,  0,  0, 64,  0, 10,  0,128,  0,  0,  0,  0, 16,  0,  0, 32,  0,  0,  0,128,  0,  8,  0,  0,  0,  0,  0, 32,  0,  0,  8,  0,  0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0,128,  0,  0,  0,  0,  0, 32,  0,128,  0,  0,  0,  0,  0,  8,  0,  0,  0,  0,  0,  8,  0,  0, 32,  0,  2,  0,  0,  0,  8,  0,  0,  0,  0,  0,  0,  0, 32,  0,  0,  0,132,  0,  0,  0,  0,  0, 32,  0,  1,  0,  0,  0,  0,  0, 16,128,  0,  0,128,  0, 64,  0,  0,  8,  0,  0,  0,  2,  0,128,  0,  0,  0,  0,
        0,  0,  0,  0,  8,  0,  0,  2,  0,  0,224, 64, 64,227,  1,  1,  3,  0,  0,  0,  0,  2,  0,  0,  0,  0,  0, 16,  0,  0,  0, 32,  0,  0,  0,  0,  0,  0,126, 24, 24, 60, 60, 24, 24,126,  0,  0,130,  0,  0,  8,  0,129,  0,  0,  0,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8,128,192,225,240,240,120, 56, 24, 24, 56,120,240,208, 64,  0,  0,  0,  0,  0,  0,  0,  0,  8,  0,  0, 64,  1,  0,  0,  0,216,223,223,223,223,223,223,222,220,216,216,220,222,223,223,221,221,220,216,216,216,  0,  0,  2,128, 16,  0,  0,  0, 32, 
        0,  0,  0,  7, 15, 31, 63,127,127,127,255,255,255,255,191, 63, 59, 27, 25,  9, 17,  0,  0,  0, 16,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0, 64,  2,  0,  0,  0,  0, 36,  0,  0,  0,  0,  0, 64,  2,  0,  0,  0,128,200,  0,  0,  0,  0, 16,  0,  0,  0, 33,  0,  0,  0,  0,  0, 64,  2,  0,  0,  0,  4,  0,  0,224, 16,144,144,144,144,144, 30,239,  3,  0,  0, 32,  0,  0,  0,  0,  4,  0,  0,  0,  0,128,  0, 48,104,144, 32,192,128,128,240,127, 48, 33, 32, 32,160, 33, 48,127,240,128, 64,160, 24,212, 56,  0, 32,  0,  0, 
        0,  0,  0,  0,  0, 32,  0,  3,  2,  4,  8,255,192,194,153,175,142,158,147,200,192,255,  4,  2, 34,  1,  0,  0,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8, 22,243, 32, 32, 17, 17,201, 17, 17, 32, 32,241, 18, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,135,252,162,162,255,  0,255,162,162,252,131,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    };
    oled_clear();
    
//oled_write_P(qmk_logo, false);
    oled_write_raw_P(raw_logo,sizeof(raw_logo));


}

void oled_idle_task2() {

 
     static const char PROGMEM raw_logo[] = {
       0,  0,  0,  8,  0, 64,  0,  8,  0,  0,  0, 64,  0, 10,  0,128,  0,  0,  0,  0, 16,  0,  0, 32,  0,  0,  0,128,  0,  8,  0,  0,  0,  0,  0, 32,  0,  0,  8,  0,  0,  0,130,  0,  0,  0, 16,  0,  0,  0,  0, 32,  0,128,  0,  0,  0,  0,  0, 32,  0,128,  0,  0,  0,  0,  0,  8,  0,  0,  0,  0,  0,  8,  0,  0, 32,  0,  2,  0,  0,  0,  8,  0,  0,  0,  0,  0,  0,  0, 32,  0,  0,  0,132,  0,  0,  0, 64,  0, 32,  0,  1,  0,  2,  0,  0,  0, 16,  0,  0,  1,  0,  0, 64,  0,  0,  8,  0,  0,  0,  2,  0,128,  8,  0,  0,  0,
        0,  0,  0,  0,  8,  0,  0,226, 64, 64,224,  0,  7,  2,  2,  7,  0,  0,  0, 32,  0,  2,  0,  0,  0,  0,128, 16,  0,  2,  0, 32,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,252, 48, 48,122,120, 48, 48,252,129,  0,  0,  0, 32,  0,  0,  0, 32,  0,  0,  0,  0,  0,  8,128,192,225,240,240,248,248,248,248,120, 56, 16, 16, 64,  0,  0,  0,  0, 32,  0,  0,  0,  8,  0,  0, 64,  1,  0,  0,  0,216,223,223,223,223,223,223,223,223,223,223,222,220,216,216,221,221,220,216,216,216,  0,  0,  2,128, 16,  0,  0,  8, 32, 
        0,  0,  0,  7, 15, 31, 63,127,127,127,255,255,255,255,191, 63, 59, 27, 25,  9, 17,  0,  0,  0, 16,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0, 64,  2,  0,  0,  0,  0, 36,  0,  0,  0, 16,  0, 64,  2,  0,  0,  0,128,136,128,  0,  0,  0,  8,  0,  0,  0, 33,  0,  0,  0,  0,  0, 64,  2,  0,  0,  0,  4,  0,  0,224, 16, 16, 16, 16, 16, 16, 30,239,  3,  0,  0, 32,  0,  0,  0,  0,  4,  0,  0,  0,  0,128,  0,  0,  0,  0,  0,  0,  0,128,240,127, 48, 32, 32, 32,160, 32, 48,127,240,128,  0,  0,  0,  0,  0,  0, 32,  0,  0, 
        0,  0,  0,  0, 96,176, 72, 38, 26,  9,  8,255,192,194,153,175,142,158,147,200,192,255,  4,  9, 21, 98,172,112,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8, 22,243, 32, 32, 17, 17,201, 17, 17, 32, 32,241, 18, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,135,252,162,162,255,  0,255,162,162,252,131,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    };
    oled_clear();
    
//oled_write_P(qmk_logo, false);
    oled_write_raw_P(raw_logo,sizeof(raw_logo));


}

void oled_idle_task3() {

 
     static const char PROGMEM raw_logo[] = {
        0,  0,  0,  8,  0, 64,  0,  8,  0,  0,  0, 64,  0, 10,  0,128,  0,  0,  0,  0, 16,  0,  0, 32,  0,  0,  0,128,  0,  8,  0,  0,  0,  0,  0, 32,  0,  0,  8,  0,  0,  0,130,  0,  0,  0, 16,  0,  0,  0,  0, 32,  0,128,  0,  0,  0,  0,  0, 32,  0,128,  0,  0,  0,  0,  0,  8,  0,  0,  0,  0,  0,  8,  0,  0, 32,  0,  2,  0,  0,  0,  8,  0,  0,  0,  0,  0,  0,  0, 32,  0,  0,  0,132,  0,  0,  0, 64,  0, 32,  0,  1,  0,  2,  0,  0,  0, 16,  0,  0,  1,  0,  0, 64,  0,  0,  8,  0,  0,  0,  2,  0,128,  8,  0,  0,  0,
        0,  0,  0,  0,  8,  0,128,  2,  0,128, 28,  8,  8, 28,  0,  0,  0,  0,  0, 32,  0,  2,  0,  0,  0,  0,128, 16,  0,  2,  0, 32,  0,  0,  0,  0,  0,  0,  3,  1,  1,  3,  0,252, 48, 48,120,120, 50, 48,252,  0,  0,129,  0,  0,  0, 32,  0,  0,  0, 32,  0,  0,  0,  0,  0,  8,128,192,225,240,240,248,248,248,248,248,248,240,208, 64,  0,  0,  0,  0, 32,  0,  0,  0,  8,  0,  0, 64,  1,  0,  0,  0,216,223,223,223,223,223,223,223,223,223,223,223,223,223,223,221,221,220,216,216,216,  0,  0,  2,128, 16,  0,  0,  8, 32, 
        0,  0,  0,  7, 15, 31, 63,127,127,127,255,255,255,255,191, 63, 59, 27, 25,  9, 17,  0,  0,  0, 16,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0, 64,  2,  0,  0,  0,  0, 36,  0,  0,  0, 16,  0, 64,  2,  0,  0,  0,  0,136,128,  0,  0,  0,  8,  0,  0,  0, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4,  0,  0,224, 16,144,144,144,144,144, 30,239,  3,  0,  0,  0,  0,  0,  0,  0,  4,  0,  0,  0,  0,128,  0,  0,  0,  0,  0,  0,  0,128,240,127, 48, 33, 32, 32,160, 33, 48,127,240,128,  0,  0,  0,  0,  0,  0, 32,  0,  0, 
        0,  0,  0,  0,128, 96, 24,132,106, 17,  8,255,192,194,153,175,142,158,147,200,192,255, 16, 33,210, 12, 56,192,  0,  0,  0,  0,  0,  0,  0,  0, 15, 25, 14,  1,  0,  0,  0,243, 32, 32, 17, 17,201, 17, 17, 32, 32,241,  0,  0,  0, 15, 18, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,135,252,162,162,255,  0,255,162,162,252,131,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    };
    oled_clear();
    
//oled_write_P(qmk_logo, false);
    oled_write_raw_P(raw_logo,sizeof(raw_logo));


}

void oled_idle_task4() {

 
     static const char PROGMEM raw_logo[] = {
        0,  0,  0,  8,  0, 64,  0,  8,  0,  0,  0, 64,  0, 10,  0,128,  0,  0,  0,  0, 16,  0,  0, 32,  0,  0,  0,128,  0,  8,  0,  0,  0,  0,  0, 32,  0,  0,  8,  0,  0,  0,130,  0,  0,  0, 16,  0,  0,  0,  0, 32,  0,128,  0,  0,  0,  0,  0, 32,  0,128,  0,  0,  0,  0,  0,  8,  0,  0,  0,  0,  0,  8,  0,  0, 32,  0,  2,  0,  0,  0,  8,  0,  0,  0,  0,  0,  0,  0, 32,  0,  0,  0,132,  0,  0,  0, 64,  0, 32,  0,  1,  0,  2,  0,  0,  0, 16,  0,  0,  1,  0,  0, 64,  0,  0,  8,  0,  0,  0,  2,  0,128,  8,  0,  0,  0,
        0,  0,  0,  0,  8,  0,112, 34, 32,112,  7,  2,  2,  7,  0,  0,  0,  0,  0, 32,  0,  2,  0,  0,  0,  0,128, 16,  0,  2,  0, 32,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,126, 24, 24, 60, 60, 24, 24,126,  0,  0,129,  0,  0,  0, 32,  0,  0,  0, 32,  0,  0,  0,  0,  0,  8,  0,  0, 33,112,240,248,248,248,248,248,248,240,208, 64,  0,  0,  0,  0, 32,  0,  0,  0,  8,  0,  0, 64,  1,  0,  0,  0,216,220,216,216,220,222,223,223,223,223,223,223,223,223,223,221,221,220,216,216,216,  0,  0,  2,128, 16,  0,  0,  8, 32, 
        0,  0,  0,  7, 15, 31, 63,127,127,127,255,255,255,255,191, 63, 59, 27, 25,  9, 17,  0,  0,  0, 16,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0, 64,  2,  0,  0,  0,  0, 36,  0,  0,  0, 16,  0, 64,  2,  0,  0,  0,128,136,128,  0,  0,  0,  8,  0,  0,  0, 33,  0,  0,  0,  0,  0, 64,  2,  0,  0,  0,  4,  0,  0,224, 16,144,144,144,144,144, 30,239,  3,  0,  0, 32,  0,  0,  0,  0,  4,  0,  0,  0,  0,128,  0,  0,  0,  0,  0,  0,  0,128,240,127, 48, 33, 33, 33,161, 33, 48,127,240,128,  0,  0,  0,  0,  0,  0, 32,  0,  0, 
        0,  0,  0,  0, 96,176, 72, 38, 26,  9,  8,255,192,194,153,175,142,158,147,200,192,255,  4,  9, 21, 98,172,112,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8, 22,243, 32, 32, 17, 17,201, 17, 17, 32, 32,241, 18, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,135,252,162,162,255,  0,255,162,162,252,131,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    };
    oled_clear();
    
//oled_write_P(qmk_logo, false);
    oled_write_raw_P(raw_logo,sizeof(raw_logo));


}


void oled_idle_task5() {

 
     static const char PROGMEM raw_logo[] = {
        0,  0,  0,  8,  0, 64,  0,  8,  0,  0,  0, 64,  0, 10,  0,128,  0,  0,  0,  0, 16,  0,  0, 32,  0,  0,  0,128,  0,  8,  0,  0,  0,  0,  0, 32,  0,  0,  8,  0,  0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0,128,  0,  0,  0,  0,  0, 32,  0,128,  0,  0,  0,  0,  0,  8,  0,  0,  0,  0,  0,  8,  0,  0, 32,  0,  2,  0,  0,  0,  8,  0,  0,  0,  0,  0,  0,  0, 32,  0,  0,  0,132,  0,  0,  0,  0,  0, 32,  0,  1,  0,  0,  0,  0,  0, 16,128,  0,  0,128,  0, 64,  0,  0,  8,  0,  0,  0,  2,  0,128,  0,  0,  0,  0,
        0,  0,  0,  0,  8,  0,  0,  2,  0,  0,224, 64, 64,227,  1,  1,  3,  0,  0,  0,  0,  2,  0,  0,  0,  0,  0, 16,  0,  0,  0, 32,  0,  0,  0,  0,  0,  0,126, 24, 24, 60, 60, 24, 24,126,  0,  0,130,  0,  0,  8,  0,129,  0,  0,  0,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8,128, 64, 33, 16, 16, 56,120,248,248,248,248,240,208, 64,  0,  0,  0,  0, 32,  0,  0,  0,  8,  0,  0, 64,  1,  0,  0,  0,216,223,223,222,220,216,216,220,222,223,223,223,223,223,223,221,221,220,216,216,216,  0,  0,  2,128, 16,  0,  0,  0, 32, 
        0,  0,  0,  7, 15, 31, 63,127,127,127,255,255,255,255,191, 63, 59, 27, 25,  9, 17,  0,  0,  0, 16,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0, 64,  2,  0,  0,  0,  0, 36,  0,  0,  0,  0,  0, 64,  2,  0,  0,  0,128,200,  0,  0,  0,  0, 16,  0,  0,  0, 33,  0,  0,  0,  0,  0, 64,  2,  0,  0,  0,  4,  0,  0,224, 16,144,144,144,144,144, 30,239,  3,  0,  0, 32,  0,  0,  0,  0,  4,  0,  0,  0,  0,128,  0, 48,104,144, 32,192,128,128,240,127, 48, 33, 32, 32,160, 33, 48,127,240,128, 64,160, 24,212, 56,  0, 32,  0,  0, 
        0,  0,  0,  0,  0, 32,  0,  3,  2,  4,  8,255,192,194,153,175,142,158,147,200,192,255,  4,  2, 34,  1,  0,  0,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8, 22,243, 32, 32, 17, 17,201, 17, 17, 32, 32,241, 18, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,135,252,162,162,255,  0,255,162,162,252,131,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    };
    oled_clear();
    
//oled_write_P(qmk_logo, false);
    oled_write_raw_P(raw_logo,sizeof(raw_logo));


}
#endif
