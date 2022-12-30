#include QMK_KEYBOARD_H
#include "transactions.h"

#include "transport.h"

/*****************************************************/
/*  T r a n s p o r t   l a y e r   t o   s l a v e  */
/*****************************************************/

void user_sync_init() {
    transaction_register_rpc(USER_SYNC_A, user_sync_a_slave_handler);
}

void user_sync_a_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    const master_to_slave_t *m2s = (const master_to_slave_t*)in_data;
    slave_to_master_t *s2m = (slave_to_master_t*)out_data;
    if (layer_state != m2s->current_layer_state) {
        layer_state = m2s->current_layer_state;
    }
    s2m->current_layer_state = m2s->current_layer_state;  // just mirror it back for debug purposes
}

void master_slave_com() {
    if (is_keyboard_master()) {
        static uint32_t last_sync = 0;
        if (timer_elapsed32(last_sync) > USER_COM_POLL_TIME_MS) {
            /* dprintf("current layer state: %d\n", layer_state); */
            master_to_slave_t m2s = {layer_state};
            slave_to_master_t s2m = {0};
            if (transaction_rpc_exec(USER_SYNC_A, sizeof(m2s), &m2s, sizeof(s2m), &s2m)) {
                last_sync = timer_read32();
                /* dprintf("Slave current layer value: %d\n", s2m.current_layer_state); */
            } else {
                dprint("Slave sync failed!\n");
            }
        }
    }
}
