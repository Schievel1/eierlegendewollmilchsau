#include QMK_KEYBOARD_H

#include "rgb_matrix_user.h"
#include "keymap.h"

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
