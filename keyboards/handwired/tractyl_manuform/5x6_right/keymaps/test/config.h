#pragma once

/*******************************/
/*  d e b u g   d e f i n e s  */
/*******************************/
/* #define DEBUG_MATRIX_SCAN_RATE */

/*****************************************/
/*  b o o t m a g i c   l i t e   k e y  */
/*****************************************/
/* We want the top left key to be the bootmagic lite key.
 * Hold this key down while plugging in the keyboard to put the MCU
 * into bootloader mode. Be aware that (since we are defining MASTER_RIGHT
 * further down) the key for the left half is also the TOP LEFT key. */

/* #define BOOTMAGIC_LITE_ROW 6 */ // this is only here for reference, it is
                                   // defined like this in 5x6_right config.h
                                   // already
// we want to have the top left key to be the bootmagic lite key
// pressing the top right key (predefined in 5x6_right config.h
// is inconvenient)
#undef BOOTMAGIC_LITE_COLUMN_RIGHT // we want to have the top left key to be
                                   // the bootmagic lite key. Pressing the
                                   // top right key (like defined in 5x6_right config.h
                                   // is inconvenient)
#define BOOTMAGIC_LITE_COLUMN_RIGHT 0

/***********************/
/*  i d l e   m o d e  */
/***********************/
#define IDLE_TIMEOUT_SECS 30
#define RGB_MATRIX_IDLE_MODE MY_RGB_MATRIX_BREATHING
#define RGB_MATRIX_SLEEP_MODE MY_RGB_MATRIX_NONE
/*************************************************************************************************************************/
/*  S p l i t   o p t i o n s                                                                                            */
/*  T h i s   i s   t o   h a v e   l a y e r   d a t a   o f
 *  t h e   m a s t e r   o n   t h e   s l a v e   s i d e  */
/*************************************************************************************************************************/
#undef SPLIT_HAND_PIN
#define MASTER_RIGHT
#define SPLIT_TRANSPORT_MIRROR
#define SPLIT_TRANSACTION_IDS_USER USER_SYNC_A
#define USER_COM_POLL_TIME_MS 100 // user data is sent between the two halfes by this time in milliseconds

/*********************************/
/*  E n c o d e r   c o n f i g  */
/*********************************/
#define ENCODER_RESOLUTION 1

/*****************************************************/
/*  P M W 3 3 6 0   t r a c k b a l l   s e n s o r  */
/*****************************************************/
#define PMW33XX_LIFTOFF_DISTANCE 0x07  //default 0x08
#define POINTING_DEVICE_ROTATION_180
#undef ROTATIONAL_TRANSFORM_ANGLE 
#define ROTATIONAL_TRANSFORM_ANGLE -15
#define CHARYBDIS_MINIMUM_DEFAULT_DPI 1000
#define CHARYBDIS_DRAGSCROLL_DPI 400
#define CHARYBDIS_DRAGSCROLL_REVERSE_Y




/***************************************/
/*  W S 2 8 1 2   R G B   M a t r i x  */
/***************************************/
#undef WS2812_EXTERNAL_PULLUP
#undef RGBLED_NUM
#undef RGBLIGHT_SPLIT
#undef RGBLED_SPLIT
#define DRIVER_LED_TOTAL 62
#define RGB_MATRIX_LED_COUNT 62
#define RGBLED_NUM 62
#define RGB_MATRIX_SPLIT \
    { 32, 30 }
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 80
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_REACTIVE
#define RGB_DISABLE_WHEN_USB_SUSPENDED
#define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#define RGB_DISABLE_WHEN_USB_SUSPENDED
#define RGB_MATRIX_KEYPRESSES
#define RGB_TRIGGER_ON_KEYDOWN

#define ENABLE_RGB_MATRIX_NONE
#define ENABLE_RGB_MATRIX_SOLID_COLOR
#define ENABLE_RGB_MATRIX_ALPHAS_MODS
#define ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
#define ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
#define ENABLE_RGB_MATRIX_BREATHING
#define ENABLE_RGB_MATRIX_BAND_SAT
#define ENABLE_RGB_MATRIX_BAND_VAL
#define ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
#define ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
#define ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT
#define ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
#define ENABLE_RGB_MATRIX_CYCLE_ALL
#define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
#define ENABLE_RGB_MATRIX_CYCLE_OUT_IN
#define ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
#define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#define ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
#define ENABLE_RGB_MATRIX_CYCLE_SPIRAL
#define ENABLE_RGB_MATRIX_DUAL_BEACON
#define ENABLE_RGB_MATRIX_RAINBOW_BEACON
#define ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
#define ENABLE_RGB_MATRIX_RAINDROPS
#define ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
#define ENABLE_RGB_MATRIX_HUE_BREATHING
#define ENABLE_RGB_MATRIX_HUE_PENDULUM
#define ENABLE_RGB_MATRIX_HUE_WAVE
#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS)
#    define ENABLE_RGB_MATRIX_TYPING_HEATMAP
#    define ENABLE_RGB_MATRIX_DIGITAL_RAIN
#endif
#if defined(RGB_MATRIX_KEYPRESSES) || defined(RGB_MATRIX_KEYRELEASES)
#    define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#    define ENABLE_RGB_MATRIX_SOLID_REACTIVE
#    define ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
#    define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
#    define ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
#    define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
#    define ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
#    define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#    define ENABLE_RGB_MATRIX_SPLASH
#    define ENABLE_RGB_MATRIX_MULTISPLASH
#    define ENABLE_RGB_MATRIX_SOLID_SPLASH
#    define ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
#endif
#define ENABLE_RGB_MATRIX_EFFECT_MAX


/* eeprom config */
#undef EXTERNAL_EEPROM_SPI_SLAVE_SELECT_PIN
#undef EXTERNAL_EEPROM_SPI_CLOCK_DIVISOR

/*oleddisplayd */
#define OLED_FONT_H "solairefont.c"
#define OLED_TIMEOUT 6000
#define OLED_DISPLAY_128X64	
