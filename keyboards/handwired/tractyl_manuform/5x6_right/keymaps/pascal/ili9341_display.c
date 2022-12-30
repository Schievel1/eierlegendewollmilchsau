#include "qp.h"

#include "ili9341_display.h"
#include "config.h"

painter_device_t ili9341_init() {
    painter_device_t display = qp_ili9341_make_spi_device(320, 240, ILI9341_CS_PIN, ILI9341_DCRS_PIN, ILI9341_RESET_PIN, 2, 0);
    qp_init(display, QP_ROTATION_270);
    qp_power(display, true);
    qp_rect(display, 0, 0, 319, 239, HSV_BLACK, true);
    return display;
}

bool ili9341_draw_display(painter_device_t display) {
    if (display != NULL) {
        static uint32_t clear_timer = 0;
        static uint32_t circle_timer = 0;
        /* qp_drawimage(display, 0, 0, dickbutt); */
        qp_circle(display, circle_timer % 320, circle_timer % 240, 25, circle_timer % 255, 255, 255, false);
        circle_timer = timer_read32();
        qp_rect(display, 0, 0, 10, 10, HSV_BLUE, true);
        qp_rect(display, 10, 10, 20, 20, HSV_GREEN, true);
        if (timer_elapsed32(clear_timer) > 10000 && timer_elapsed32(clear_timer) < 10500) {
            qp_clear(display);
            qp_init(display, QP_ROTATION_270);
            qp_power(display, true);
            qp_rect(display, 0, 0, 319, 239, HSV_BLACK, true);
            clear_timer = timer_read32();
        }
        qp_flush(display);
        return true;
    } else {
        dprint("variable display is NULL");
        return false;
    }
}
