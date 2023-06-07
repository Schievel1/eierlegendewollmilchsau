#pragma once

#include QMK_KEYBOARD_H

enum custom_layers {
    _DVORAK,
    _LOWER,
    _RAISE,
};



#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)

extern bool idle_mode;
extern bool sleep_mode;
extern bool isSneaking;
extern bool isJumping;
extern bool showedJump;


