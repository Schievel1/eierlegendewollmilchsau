#include QMK_KEYBOARD_H

#include "rgb_matrix_user.h"
#include "keymap.h"
 #include "print.h"

void oled_idle_task0(void);
void oled_idle_task1(void);
void oled_idle_task2(void);
void oled_idle_task3(void);
void oled_idle_task4(void);
void oled_idle_task5(void);

/* timers */
uint32_t anim_timer = 0;
uint32_t anim_timer1 = 0;
/* current frame */
uint8_t current_frame1 = 0;
led_t led_usb_state;
 bool oldjump = 0;
/* Defines */
#ifdef OLED_ENABLE
 uint32_t ANIM_FRAME_DURATION1 = 160;
 uint8_t ANIM_FRAME_DURATION1_OLD_Loc = 1;
 uint8_t blink = 0;
#define ANIM_FRAME_DURATION 150
/* Defines Luna */
/* settings */

/* advanced settings */

#    define ANIM_SIZE         186  //96   // number of bytes in array. If you change sprites, minimize for adequate firmware size. max is 1024


//Setup some mask which can be or'd with bytes to turn off pixels
const uint8_t single_bit_masks[8] = {127, 191, 223, 239, 247, 251, 253, 254};

static void fade_display(void) {
    //Define the reader structure
    oled_buffer_reader_t reader;
    uint8_t buff_char;
    if (random() % 30 == 0) {
        srand(timer_read());
        // Fetch a pointer for the buffer byte at index 0. The return structure
        // will have the pointer and the number of bytes remaining from this
        // index position if we want to perform a sequential read by
        // incrementing the buffer pointer
        reader = oled_read_raw(0);
        //Loop over the remaining buffer and erase pixels as we go
        for (uint16_t i = 0; i < reader.remaining_element_count; i++) {
            //Get the actual byte in the buffer by dereferencing the pointer
            buff_char = *reader.current_element;
            if (buff_char != 0) {
                oled_write_raw_byte(buff_char & single_bit_masks[rand() % 8], i);
            }
            //increment the pointer to fetch a new byte during the next loop
            reader.current_element++;
        }
    }
}



oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}


void write_LED(int mode) {

if (!(rgb_matrix_is_enabled())) {
            oled_write_P(PSTR("Off\n"), false);
        } else {
            switch (mode) {
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

}

/* KEYBOARD PET START */


/* timers */
//uint32_t anim_timer = 0;

/* current frame */
uint8_t current_frame = 0;

/* bool isSneaking = false;
bool isJumping  = false;
bool showedJump = true; */





/* logic */
static void render_luna(int LUNA_X, int LUNA_Y) {
    /* Sit */
    static const char PROGMEM sit[2][ANIM_SIZE] = {/* 'sit1', 32x22px */
                                                   {
                                                    //Dog sit
                                                    // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c,
                                                    // 0x02, 0x05, 0x02, 0x24, 0x04, 0x04, 0x02, 0xa9, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x08, 0x68, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00,
                                                    // 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x82, 0x7c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x0c, 0x10, 0x10, 0x20, 0x20, 0x20, 0x28,
                                                    // 0x3e, 0x1c, 0x20, 0x20, 0x3e, 0x0f, 0x11, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

                                                    // 'CatIdle1', 32x38px
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0xc0, 0x60, 0x10, 0x10, 0x60, 0xc0, 0x00, 0x80, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20,
                                                    0x20, 0x20, 0x20, 0x20, 0x40, 0x40, 0x80, 0x00, 0xc0, 0x60, 0x10, 0x10, 0x60, 0xc0, 0x00, 0x00,
                                                    0x80, 0x70, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x60, 0x60, 0x00, 0x80, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x80, 0x00, 0x60, 0x60, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x78, 0xc0,
                                                    0x1f, 0x10, 0x10, 0x10, 0x10, 0x70, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x71, 0x12, 0x12, 0x11,
                                                    0x12, 0x72, 0xc1, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1f,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                                                   },

                                                   /* 'sit2', 32x22px */
                                                   {
                                                   //Dog sit 2
                                                    //0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0x02, 0x05, 0x02, 0x24, 0x04, 0x04, 0x02, 0xa9, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x90, 0x08, 0x18, 0x60, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x0e, 0x82, 0x7c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x0c, 0x10, 0x10, 0x20, 0x20, 0x20, 0x28, 0x3e, 0x1c, 0x20, 0x20, 0x3e, 0x0f, 0x11, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

                                                    // 'CatIdle2', 32x38px
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0xc0, 0x20, 0x20, 0xc0, 0x80, 0x00, 0x80, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20,
                                                    0x20, 0x20, 0x20, 0x20, 0x40, 0x40, 0x80, 0x00, 0x80, 0xc0, 0x20, 0x20, 0xc0, 0x80, 0x00, 0x00,
                                                    0x80, 0x70, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x78, 0xc0,
                                                    0x1f, 0x10, 0x10, 0x10, 0x10, 0x70, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x71, 0x12, 0x12, 0x11,
                                                    0x12, 0x72, 0xc1, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1f,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                                                    }};

    /* Walk */
    static const char PROGMEM walk[2][ANIM_SIZE] = {/* 'walk1', 32x22px */
                                                    {
                                                     //Dog walk 1
                                                     //   0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x90, 0x90, 0x90, 0xa0, 0xc0, 0x80, 0x80, 0x80, 0x70, 0x08, 0x14, 0x08, 0x90, 0x10, 0x10, 0x08, 0xa4, 0x78, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x08, 0xfc, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x18, 0xea, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1c, 0x20, 0x20, 0x3c, 0x0f, 0x11, 0x1f, 0x03, 0x06, 0x18, 0x20, 0x20, 0x3c, 0x0c, 0x12, 0x1e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

                                                     // 'CatTap1', 32x38px
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0xc0, 0x60, 0x10, 0x10, 0x60, 0xc0, 0x00, 0x80, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20,
                                                    0x20, 0x20, 0x20, 0x20, 0x40, 0x40, 0x80, 0x00, 0x80, 0xc0, 0x20, 0x20, 0xc0, 0x80, 0x00, 0x00,
                                                    0x80, 0x70, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x01, 0x81, 0xa0, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x80, 0x00, 0x60, 0x60, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x78, 0xc0,
                                                    0x1f, 0x10, 0x10, 0x10, 0x10, 0x1c, 0x07, 0x01, 0x0c, 0x0c, 0x01, 0x07, 0x1c, 0x90, 0x92, 0x91,
                                                    0x92, 0x12, 0x71, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x70, 0x10, 0x90, 0x90, 0x90, 0x10, 0x1f,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03,
                                                    0x23, 0x33, 0x38, 0x39, 0x01, 0x02, 0x02, 0x39, 0x31, 0x20, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00,
 },

                                                    /* 'walk2', 32x22px */
                                                    {
                                                     //Dog walk 2
                                                     //   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x20, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x28, 0x10, 0x20, 0x20, 0x20, 0x10, 0x48, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x20, 0xf8, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x30, 0xd5, 0x20, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x20, 0x30, 0x0c, 0x02, 0x05, 0x09, 0x12, 0x1e, 0x02, 0x1c, 0x14, 0x08, 0x10, 0x20, 0x2c, 0x32, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

                                                     // 'CatTap2', 32x38px
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0xc0, 0x20, 0x20, 0xc0, 0x80, 0x00, 0x80, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20,
                                                    0x20, 0x20, 0x20, 0x20, 0x40, 0x40, 0x80, 0x00, 0xc0, 0x60, 0x10, 0x10, 0x60, 0xc0, 0x00, 0x00,
                                                    0x80, 0x70, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x60, 0x60, 0x00, 0x80, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x20, 0xa0, 0x80, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x78, 0xc0,
                                                    0x1f, 0x90, 0x90, 0x90, 0x10, 0x70, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x71, 0x12, 0x92, 0x91,
                                                    0x92, 0x90, 0x1c, 0x07, 0x01, 0x0c, 0x0c, 0x01, 0x07, 0x1c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1f,
                                                    0x00, 0x03, 0x03, 0x23, 0x33, 0x38, 0x39, 0x01, 0x02, 0x02, 0x39, 0x31, 0x20, 0x03, 0x03, 0x03,
                                                    0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

                                                    }};

    /* Run */
    static const char PROGMEM run[2][ANIM_SIZE] = {/* 'run1', 32x22px */
                                                   {
                                                     //  0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x08, 0x08, 0xc8, 0xb0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x3c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0xc4, 0xa4, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc8, 0x58, 0x28, 0x2a, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x09, 0x04, 0x04, 0x04, 0x04, 0x02, 0x03, 0x02, 0x01, 0x01, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    // 'CatShout1', 32x38px
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0xc0, 0x60, 0x10, 0x10, 0x60, 0xc0, 0x00, 0x80, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20,
                                                    0x20, 0x20, 0x20, 0x20, 0x40, 0x40, 0x80, 0x00, 0xc0, 0x60, 0x10, 0x10, 0x60, 0xc0, 0x00, 0x00,
                                                    0x80, 0x70, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x40, 0x60, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x78, 0xc0,
                                                    0x1f, 0x90, 0x90, 0x90, 0x10, 0x70, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x71, 0x12, 0x96, 0x96,
                                                    0x96, 0x12, 0x71, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x70, 0x10, 0x90, 0x90, 0x90, 0x10, 0x1f,
                                                    0x00, 0x03, 0x03, 0x03, 0x03, 0x20, 0x31, 0x39, 0x02, 0x02, 0x39, 0x39, 0x30, 0x23, 0x03, 0x03,
                                                    0x03, 0x23, 0x30, 0x39, 0x39, 0x02, 0x02, 0x39, 0x31, 0x20, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00,
                                                   },

                                                   /* 'run2', 32x22px */
                                                   {
                                                     //  0x00, 0x00, 0x00, 0xe0, 0x10, 0x10, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x78, 0x28, 0x08, 0x10, 0x20, 0x30, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0xb0, 0x50, 0x55, 0x20, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x1e, 0x20, 0x20, 0x18, 0x0c, 0x14, 0x1e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                       // 'CatShout2', 32x38px
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0xc0, 0x20, 0x20, 0xc0, 0x80, 0x00, 0x80, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20,
                                                    0x20, 0x20, 0x20, 0x20, 0x40, 0x40, 0x80, 0x00, 0x80, 0xc0, 0x20, 0x20, 0xc0, 0x80, 0x00, 0x00,
                                                    0x80, 0x70, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x01, 0x81, 0xa0, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x20, 0xa0, 0x80, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x78, 0xc0,
                                                    0x1f, 0x10, 0x10, 0x10, 0x10, 0x1c, 0x07, 0x01, 0x0c, 0x0c, 0x01, 0x07, 0x1c, 0x10, 0x12, 0x16,
                                                    0x12, 0x10, 0x1c, 0x07, 0x01, 0x0c, 0x0c, 0x01, 0x07, 0x1c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1f,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

                                                   }};

    /* Bark */
    static const char PROGMEM bark[2][ANIM_SIZE] = {/* 'bark1', 32x22px */
                                                    {
                                                     //   0x00, 0xc0, 0x20, 0x10, 0xd0, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x3c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc8, 0x48, 0x28, 0x2a, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    // 'CatWait1', 32x38px
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x1e, 0x7e,
                                                    0x7e, 0x1e, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0xc0, 0x60, 0x10, 0x10, 0x60, 0xc0, 0x00, 0x80, 0x40, 0x40, 0x20, 0x20, 0x22, 0x27,
                                                    0x27, 0x22, 0x20, 0x20, 0x40, 0x40, 0x80, 0x00, 0xc0, 0x60, 0x10, 0x10, 0x60, 0xc0, 0x00, 0x00,
                                                    0x80, 0x70, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x01, 0x81, 0xb0, 0x30, 0x00, 0x00, 0x00, 0x00, 0x80,
                                                    0x00, 0x00, 0x00, 0x00, 0x30, 0xb0, 0x80, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x78, 0xc0,
                                                    0x1f, 0x10, 0x10, 0x10, 0x10, 0x1c, 0x07, 0x01, 0x0c, 0x0c, 0x01, 0x07, 0x1c, 0x10, 0x11, 0x10,
                                                    0x11, 0x10, 0x1c, 0x07, 0x01, 0x0c, 0x0c, 0x01, 0x07, 0x1c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1f,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

                                                    /* 'bark2', 32x22px */
                                                    {
                                                      //  0x00, 0xe0, 0x10, 0x10, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x40, 0x2c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0x48, 0x28, 0x2a, 0x10, 0x0f, 0x20, 0x4a, 0x09, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      // 'CatWait2', 32x38px
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x33, 0xe1, 0x81,
                                                    0x81, 0xe1, 0x33, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0xc0, 0x20, 0x20, 0xc0, 0x80, 0x00, 0x80, 0x40, 0x40, 0x20, 0x27, 0x2d, 0x28,
                                                    0x28, 0x2d, 0x27, 0x20, 0x40, 0x40, 0x80, 0x00, 0x80, 0xc0, 0x20, 0x20, 0xc0, 0x80, 0x00, 0x00,
                                                    0x80, 0x70, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x01, 0x81, 0xa0, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x20, 0xa0, 0x80, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x78, 0xc0,
                                                    0x1f, 0x10, 0x10, 0x10, 0x10, 0x1c, 0x07, 0x01, 0x0c, 0x0c, 0x01, 0x07, 0x1c, 0x10, 0x12, 0x11,
                                                    0x12, 0x10, 0x1c, 0x07, 0x01, 0x0c, 0x0c, 0x01, 0x07, 0x1c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1f,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    }};

    /* Sneak */
    static const char PROGMEM sneak[2][ANIM_SIZE] = {/* 'sneak1', 32x22px */
                                                     {
                                                    //Dog sneak 1
                                                    //     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x40, 0x40, 0x80, 0x00, 0x80, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x21, 0xf0, 0x04, 0x02, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x04, 0x04, 0x04, 0x03, 0x01, 0x00, 0x00, 0x09, 0x01, 0x80, 0x80, 0xab, 0x04, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1c, 0x20, 0x20, 0x3c, 0x0f, 0x11, 0x1f, 0x02, 0x06, 0x18, 0x20, 0x20, 0x38, 0x08, 0x10, 0x18, 0x04, 0x04, 0x02, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00,

                                                    // 'CatWhisp1', 32x38px
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0xc0, 0x20, 0x20, 0xc0, 0x80, 0x00, 0x80, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20,
                                                    0x20, 0x20, 0x20, 0x20, 0x40, 0x40, 0x80, 0x00, 0x80, 0xc0, 0x20, 0x20, 0xc0, 0x80, 0x00, 0x00,
                                                    0x80, 0x70, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x01, 0x81, 0xa0, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x78, 0xc0,
                                                    0x1f, 0x10, 0x10, 0x10, 0x10, 0x1c, 0x07, 0x01, 0x0c, 0x0c, 0x01, 0x07, 0x1c, 0x10, 0x12, 0x11,
                                                    0x12, 0x10, 0x70, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1f,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

                                                     },

                                                     /* 'sneak2', 32x22px */
                                                     {
                                                    //Dog sneak 2
                                                      //   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xa0, 0x20, 0x40, 0x80, 0xc0, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x41, 0xf0, 0x04, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x02, 0x04, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x40, 0x40, 0x55, 0x82, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x20, 0x30, 0x0c, 0x02, 0x05, 0x09, 0x12, 0x1e, 0x04, 0x18, 0x10, 0x08, 0x10, 0x20, 0x28, 0x34, 0x06, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,

                                                    // 'CatWhisp2', 32x38px
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0xc0, 0x20, 0x20, 0xc0, 0x80, 0x00, 0x80, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20,
                                                    0x20, 0x20, 0x20, 0x20, 0x40, 0x40, 0x80, 0x00, 0x80, 0xc0, 0x20, 0x20, 0xc0, 0x80, 0x00, 0x00,
                                                    0x80, 0x70, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x20, 0xa0, 0x80, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x78, 0xc0,
                                                    0x1f, 0x10, 0x10, 0x10, 0x10, 0x70, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x70, 0x10, 0x12, 0x11,
                                                    0x12, 0x10, 0x1c, 0x07, 0x01, 0x0c, 0x0c, 0x01, 0x07, 0x1c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1f,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

                                                     }};


    /* animation */
    void animate_luna(void) {


        /* switch frame */
        current_frame = (current_frame + 1) % 2;


    }


if (current_wpms <= MIN_WALK_SPEED) {
        if (current_frame == 0){
            ANIM_FRAME_DURATION1 = ANIM_FRAME_DURATION1_OLD_Loc*500;
        if (ANIM_FRAME_DURATION1 < 500) {
            ANIM_FRAME_DURATION1 = 500;
        }}else {
             ANIM_FRAME_DURATION1 = 100;
        }

}else if (current_wpms <=MIN_RUN_SPEED ) {
            ANIM_FRAME_DURATION1 = 1400-current_wpms*1400/MIN_RUN_SPEED+66;
 } else {
             ANIM_FRAME_DURATION1 = 66;
        }

    /* animation timer */
    if (timer_elapsed32(anim_timer1) > ANIM_FRAME_DURATION1) {
        anim_timer1 = timer_read32();
        ANIM_FRAME_DURATION1_OLD_Loc=ANIM_FRAME_DURATION1_OLD;

        animate_luna();

    }

/* jump */
            if (isJumping == 0) {
                oldjump = 0;
            }
        if (isJumping != oldjump) {
            /* clear */
            oled_set_cursor(LUNA_X, LUNA_Y + 2);
            oled_write("     ", false);

            oled_set_cursor(LUNA_X, LUNA_Y - 1);
            oldjump = isJumping;

        } else {
            /* clear */
            oled_set_cursor(LUNA_X, LUNA_Y - 1);
            oled_write("     ", false);

            oled_set_cursor(LUNA_X, LUNA_Y);
        }



        /* current status */
        if (led_usb_state.caps_lock) {
            oled_write_raw_P(bark[current_frame], ANIM_SIZE);

        } else if (isSneaking) {
            oled_write_raw_P(sneak[current_frame], ANIM_SIZE);

        } else if (current_wpms <= MIN_WALK_SPEED) {
            oled_write_raw_P(sit[current_frame], ANIM_SIZE);

        } else if (current_wpms <= MIN_RUN_SPEED) {
            oled_write_raw_P(walk[current_frame], ANIM_SIZE);

        } else {
            oled_write_raw_P(run[current_frame], ANIM_SIZE);
        }



}

/* KEYBOARD PET END */
void Int_to_Oled(uint16_t InputInt ) {

 char tempStr[4+1]; // One extra for terminator

// Clear tempStr and make sure there's always a string-terminating `\0` at the end
for ( uint16_t i = 0; i < sizeof(tempStr); i++ ) {
  tempStr[i] = '\0';
}

itoa(InputInt, tempStr, 10);
oled_write_P(PSTR(tempStr), false);
}





bool oled_task_user(void) {



if (is_oled_on()){
       /* KEYBOARD PET VARIABLES START */


    led_usb_state = host_keyboard_led_state();
    /* KEYBOARD PET VARIABLES END */

    if (!idle_mode) {

        /* Host Keyboard Layer Status */
		oled_clear();
	    oled_set_cursor(0, 0);
         oled_write_P(PSTR("\nTB:"), false);
        switch (biton32(layer_state)) {
            case _QWERTZ:
                Int_to_Oled(charybdis_get_pointer_default_dpi()/100 );
                oled_write_P(PSTR("\nQWRTZ"), false);
                oled_write_P(PSTR("\n"), false);
                write_LED(rgb_matrix_get_mode());
                break;
            case _LOWER:
                Int_to_Oled(charybdis_get_pointer_sniping_dpi()/100 );
                oled_write_P(PSTR("\n\nLOWER"), false);
                oled_write_P(PSTR("\n"), false);
                write_LED(rgb_matrix_get_mode());
                break;
            case _RAISE:
                oled_write_P(PSTR("Dg"), false);
                oled_write_P(PSTR("\nRAISE"), false);
                oled_write_P(PSTR("\n"), false);
                write_LED(rgb_matrix_get_mode());
                break;
            case _PROG:
                oled_write_P(PSTR("Dc"), false);
                oled_write_P(PSTR("\nPROG\n"), false);
                oled_write_P(PSTR("\n"), false);
                write_LED(rgb_matrix_get_mode());
                break;
            case _GAME:
                render_luna(0, 11);
            	oled_set_cursor(3, 1);
                Int_to_Oled(charybdis_get_pointer_default_dpi()/100 );
                oled_write_P(PSTR("\nGAME\n"), false);
                oled_write_P(PSTR("\n"), false);
                write_LED(rgb_matrix_get_mode());
                break;
            case _CONF:
                oled_set_cursor(0, 0);
                oled_write_P(PSTR("SETUP-----POINT"), false);
                Int_to_Oled(charybdis_get_pointer_default_dpi()/100 );
                oled_write_P(PSTR("\n\nSNIPE"), false);
                Int_to_Oled(charybdis_get_pointer_sniping_dpi()/100 );
                oled_write_P(PSTR("\nAktiv"), false);
                write_LED(LayerEFF);
                oled_write_P(PSTR("\nTime\n"), false);
                 oled_write_P(PSTR("I:"), false);
                Int_to_Oled(user_config2.EE_TimeIdle );
                 oled_write_P(PSTR("\nS:"), false);
                Int_to_Oled(user_config2.EE_TimeSleep);
                break;
            default:
                // Or use the write_ln shortcut over adding '\n' to the end of your string
                oled_write_ln_P(PSTR("Undef"), false);
        }
        /* Host Keyboard LED Status */
if (!(biton32(layer_state)==5))
{
        led_t led_state = host_keyboard_led_state();
        oled_write_P(PSTR("\n"), false);
        oled_write_P(led_state.num_lock ? PSTR("NUM\n") : PSTR(""), false);
        oled_write_P(led_state.scroll_lock ? PSTR("SCR\n") : PSTR(""), false);
        oled_write_P(led_state.caps_lock ? PSTR("CAP\n") : PSTR(""), false);

        render_luna(0, 11);
}




















} else if (!sleep_mode) {
        uprintf("%i Status sleep\n",sleep_mode);

        /***********************/
       /*  i d l e   m o d e oled  */
      /***********************/

if (timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
        anim_timer = timer_read32();
        if (current_frame1 < 5) {
        current_frame1 = current_frame1 + 1;
    }else {
        current_frame1 = 0;
        }

            switch (current_frame1) {
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
    }else {
        print("oled off\n");
      fade_display();
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
