#pragma once

#define MY_RGB_MATRIX_NONE 0
#define MY_RGB_MATRIX_SOLID_COLOR 0
#define MY_RGB_MATRIX_ALPHAS_MODS 1
#define MY_RGB_MATRIX_GRADIENT_UP_DOWN 2
#define MY_RGB_MATRIX_GRADIENT_LEFT_RIGHT 3
#define MY_RGB_MATRIX_BREATHING 4
#define MY_RGB_MATRIX_BAND_SAT 5
#define MY_RGB_MATRIX_BAND_VAL 6
#define MY_RGB_MATRIX_BAND_PINWHEEL_SAT 7
#define MY_RGB_MATRIX_BAND_PINWHEEL_VAL 8
#define MY_RGB_MATRIX_BAND_SPIRAL_SAT 9
#define MY_RGB_MATRIX_BAND_SPIRAL_VAL 10
#define MY_RGB_MATRIX_CYCLE_ALL 11
#define MY_RGB_MATRIX_CYCLE_LEFT_RIGHT 12
#define MY_RGB_MATRIX_CYCLE_UP_DOWN 13
#define MY_RGB_MATRIX_CYCLE_OUT_IN 14
#define MY_RGB_MATRIX_CYCLE_OUT_IN_DUAL 15
#define MY_RGB_MATRIX_RAINBOW_MOVING_CHEVRON 16
#define MY_RGB_MATRIX_CYCLE_PINWHEEL 17
#define MY_RGB_MATRIX_CYCLE_SPIRAL 18
#define MY_RGB_MATRIX_DUAL_BEACON 19
#define MY_RGB_MATRIX_RAINBOW_BEACON 20
#define MY_RGB_MATRIX_RAINBOW_PINWHEELS 21
#define MY_RGB_MATRIX_RAINDROPS 22
#define MY_RGB_MATRIX_JELLYBEAN_RAINDROPS 23
#define MY_RGB_MATRIX_HUE_BREATHING 24
#define MY_RGB_MATRIX_HUE_PENDULUM 25
#define MY_RGB_MATRIX_HUE_WAVE 26
#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS)
#    define MY_RGB_MATRIX_TYPING_HEATMAP 27
#    define MY_RGB_MATRIX_DIGITAL_RAIN 28
#endif
#if defined(RGB_MATRIX_KEYPRESSES) || defined(RGB_MATRIX_KEYRELEASES)
#    define MY_RGB_MATRIX_SOLID_REACTIVE_SIMPLE 29
#    define MY_RGB_MATRIX_SOLID_REACTIVE 30
#    define MY_RGB_MATRIX_SOLID_REACTIVE_WIDE 31
#    define MY_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE 32
#    define MY_RGB_MATRIX_SOLID_REACTIVE_CROSS 33
#    define MY_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS 34
#    define MY_RGB_MATRIX_SOLID_REACTIVE_NEXUS 35
#    define MY_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS 36
#    define MY_RGB_MATRIX_SPLASH 37
#    define MY_RGB_MATRIX_MULTISPLASH 38
#    define MY_RGB_MATRIX_SOLID_SPLASH 39
#    define MY_RGB_MATRIX_SOLID_MULTISPLASH 40
#endif
#define MY_RGB_MATRIX_EFFECT_MAX 41
