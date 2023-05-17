#pragma once

typedef struct _master_to_slave_t {
    layer_state_t current_layer_state;
    bool          current_idle_state;
} master_to_slave_t;

typedef struct _slave_to_master_t {
    layer_state_t current_layer_state;
    bool          current_idle_state;
} slave_to_master_t;

void user_sync_init(void);
void user_sync_a_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data);
void master_slave_com(void);
