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
    EFFIdle,
    SNIPE,
    DRAG,
    COMMDOT,
    P4BTN1,
    P5BTN2,
    EESave,
    TimeSleep,
    TimeIdle,
};
typedef union {
  int32_t raw;
  struct {
uint8_t EE_OffsLayer_1;
uint8_t EE_OffsLayer_2;
uint8_t EE_OffsLayer_3;
uint8_t EE_OffsLayer_4;

  };


} user_config_t;
typedef union {

  int32_t raw;
  struct {
uint8_t EE_EffectL1;
uint8_t EE_EffectL2;
uint8_t EE_EffectL3;
uint8_t EE_EffectL4;

  };

} user_config_t1;
typedef union {


  int32_t raw;
  struct {
uint8_t EE_EffectL5;
uint8_t EE_EffectIdle;
uint8_t EE_TimeSleep;
uint8_t EE_TimeIdle;

  };
} user_config_t2;




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
extern user_config_t1 user_config1;
extern user_config_t2 user_config2;
extern uint16_t DragScroll;
extern uint16_t DragCurser;
extern bool zoom;
extern bool troughtTime;
extern uint8_t LayerEFF;
