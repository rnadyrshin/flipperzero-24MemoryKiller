#include "../i2ctools_i.h"
#include "settings_view.h"

void draw_settings_view(Canvas* canvas) {
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_rframe(canvas, 0, 0, 128, 64, 3);
    canvas_set_font(canvas, FontSecondary);

    char addr_text[8];

    // I2C addr
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_str_aligned(canvas, 3, 3, AlignLeft, AlignTop, "Memory chip:");
    canvas_draw_icon(canvas, 68, 5, &I_ButtonDown_7x4);
    canvas_draw_icon(canvas, 118, 5, &I_ButtonUp_7x4);
    canvas_draw_str_aligned(canvas, 97, 3, AlignCenter, AlignTop, chip_to_str(i2ctools->chip));

    // Page addr
    canvas_draw_str_aligned(canvas, 3, 13, AlignLeft, AlignTop, "Test page:");
    canvas_draw_icon(canvas, 70, 13, &I_ButtonLeft_4x7);
    canvas_draw_icon(canvas, 120, 13, &I_ButtonRight_4x7);
    snprintf(addr_text, sizeof(addr_text), "%d", i2ctools->test_page);
    canvas_draw_str_aligned(canvas, 97, 13, AlignCenter, AlignTop, addr_text);
/*
    if(i2c_read->readed) {
        // Result
        for(uint16_t byte = 0; byte < i2c_read->len; byte++) {
            uint8_t x_pos = 5 + (byte % 8) * 15;
            uint8_t y_pos = 2 + (byte / 8) * 8;
            snprintf(addr_text, sizeof(addr_text), "%02X", (int)i2c_read->recv[byte]);
            canvas_draw_str_aligned(canvas, x_pos, y_pos, AlignLeft, AlignTop, addr_text);
        }
    }
    else {
    }
*/
}