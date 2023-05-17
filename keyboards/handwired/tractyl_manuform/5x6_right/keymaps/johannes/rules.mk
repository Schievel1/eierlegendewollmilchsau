BOOTLOADER = tinyuf2

RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = WS2812

ENCODER_ENABLE = yes

# for the small oled display
OLED_ENABLE = yes


# for changing what shift does to a key
KEY_OVERRIDE_ENABLE = yes

# for the big oled display
QUANTUM_PAINTER_ENABLE = no

EEPROM_DRIVER = vendor

# master slave user sync
SRC += transport.c
# small oled display
SRC += oled_display.c
# rgb matrix
SRC += rgb_matrix_user.c

