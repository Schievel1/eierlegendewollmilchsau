#pragma once

#include QMK_KEYBOARD_H

enum custom_layers {
    _DVORAK,
    _LOWER,
    _RAISE,
};


typedef int wpm_state_t;
#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)

#    define MIN_WALK_SPEED      5
#    define MIN_RUN_SPEED       50

extern bool idle_mode;
extern bool sleep_mode;
extern bool isSneaking;
extern bool isJumping;
extern wpm_state_t  current_wpms;

