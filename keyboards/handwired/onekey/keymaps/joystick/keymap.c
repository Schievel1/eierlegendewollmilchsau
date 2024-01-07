#include QMK_KEYBOARD_H
#include "print.h"
#include "analog.h"

#ifndef ADC_PIN
#    define ADC_PIN F6
#endif
int16_t val2 = 1023;

int16_t val3 = 0;
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    LAYOUT_ortho_1x1(JS_0)
};

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  debug_matrix=true;
  debug_keyboard=true;
  //debug_mouse=true;

}

void matrix_scan_user(void) {
    int16_t val = ((1 % 5000 - 2500) * 255) / 5000;
    joystick_set_axis(1, val);
    int16_t val1 = analogReadPin(ADC_PIN);
     if (val1<val2) {
        val2 = val1;
    }
    dprintf("got: %d ", val2);

         if (val1>val3) {
        val3 = val1;
    }
    dprintf(", %d ", val1);
    
    dprintf(", %d \n", val3);
}

// Joystick config
joystick_config_t joystick_axes[JOYSTICK_AXIS_COUNT] = {
    [0] = JOYSTICK_AXIS_IN(ADC_PIN, 970, 410, 150),
    [1] = JOYSTICK_AXIS_VIRTUAL
};


void raw_hid_receive(uint8_t *data, uint8_t length) {
    data[length-1] = 0;
    dprintf("got: %s", data);
}