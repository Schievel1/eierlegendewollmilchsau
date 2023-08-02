#pragma once

#include QMK_KEYBOARD_H

enum custom_layers {
    _QWERTZ,
    _GAME,
    _PROG,
    _LOWER,
    _RAISE,
    _CONF
};

enum my_keycodes {
    BSPCDEL = SAFE_RANGE,
    ZOOM,
    UNREDO,
    DPIUP,
    DPIDWN,
    DPISPUP,
    DPISPDWN,
    HUELAY1,
    HUELAY2,
    HUELAY3,
    HUELAY4,
    HUELAY5,
    EFFLAY1,
    EFFLAY2,
    EFFLAY3,
    EFFLAY4,
    EFFLAY5,
    SNIPE,
    DRAG,
    COMMDOT,
    P4BTN1,
    P5BTN2,
    EESave,
};
typedef union {
  int32_t raw1;
  struct {
uint8_t EE_OffsLayer_1;
uint8_t EE_OffsLayer_2;
uint8_t EE_OffsLayer_3;
uint8_t EE_OffsLayer_4;

  };

  int64_t raw2;
  struct {
uint8_t EE_EffectL1;
uint8_t EE_EffectL2;
uint8_t EE_EffectL3;
uint8_t EE_EffectL4;
uint8_t EE_EffectL5;
uint8_t EE_EffectSleep;

  };
} user_config_t;




typedef int wpm_state_t;
#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)
#define GAME MO(_GAME)
#define PROG MO(_PROG)
#define CONF MO(_CONF)

#    define MIN_WALK_SPEED      5
#    define MIN_RUN_SPEED       50

extern bool idle_mode;
extern bool sleep_mode;
extern bool isSneaking;
extern bool isJumping;
extern wpm_state_t  current_wpms;
extern uint8_t ANIM_FRAME_DURATION1_OLD;
extern user_config_t user_config;
extern uint16_t DragScroll;
extern uint16_t DragCurser;
extern bool zoom;
extern bool troughtTime;
extern uint8_t LayerEFF;


