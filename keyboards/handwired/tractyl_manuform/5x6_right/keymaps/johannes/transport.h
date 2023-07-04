#pragma once

typedef struct _master_to_slave_t {
    layer_state_t current_layer_state;
    bool          current_idle_state;
    bool          current_sleep_state;
    bool          current_Sneaking;
    bool          current_Jumping;
    uint8_t current_ANIM_FRAME_DURATION1_OLD;
    uint8_t current_OffsLayer_1;
    uint8_t current_OffsLayer_2;
    uint8_t current_OffsLayer_3;
} master_to_slave_t;

typedef struct _slave_to_master_t {
    layer_state_t current_layer_state;
    bool          current_idle_state;
    bool          current_sleep_state;
    bool          current_Sneaking;
    bool          current_Jumping;
    uint8_t current_ANIM_FRAME_DURATION1_OLD;
    uint8_t current_OffsLayer_1;
    uint8_t current_OffsLayer_2;
    uint8_t current_OffsLayer_3;
} slave_to_master_t;

void user_sync_init(void);
void user_sync_a_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data);
void master_slave_com(void);
