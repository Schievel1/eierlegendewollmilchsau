#pragma once

#include QMK_KEYBOARD_H

enum custom_layers {
    _QWERTZ,
    _LOWER,
    _RAISE,
    _GAME,
    _CONF,
};

enum my_keycodes {
    BSPCDEL = SAFE_RANGE,
    ZOOM,
    UNREDO
};

typedef int wpm_state_t;
#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)
#define GAME MO(_GAME)
#define CONF MO(_CONF)

#    define MIN_WALK_SPEED      5
#    define MIN_RUN_SPEED       50

extern bool idle_mode;
extern bool sleep_mode;
extern bool isSneaking;
extern bool isJumping;
extern wpm_state_t  current_wpms;
extern uint8_t ANIM_FRAME_DURATION1_OLD;
extern uint8_t OffsLayer_1;
extern uint8_t OffsLayer_2;
extern uint8_t OffsLayer_3;
extern uint16_t DragScroll;
extern bool zoom;
extern bool troughtTime;
