# added by pascal
RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = WS2812

ENCODER_ENABLE = yes

# for the small oled display
OLED_ENABLE = yes
BACKLIGHT_ENABLE = yes # using feature_backlight for the backlight of the LED screen
BACKLIGHT_DRIVER = pwm

# for changing what shift does to a key
KEY_OVERRIDE_ENABLE = yes

# for the big oled display
QUANTUM_PAINTER_ENABLE = yes
QUANTUM_PAINTER_DRIVERS = ili9341_spi
# painter images
SRC += graphics/dickbutt.qgf.c
# SRC += rgb_matrix_user.c
