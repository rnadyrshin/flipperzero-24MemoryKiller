#include "read_view.h"

void draw_read_view(Canvas* canvas, i2cRead* i2c_read) {
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_rframe(canvas, 0, 0, 128, 64, 3);

    if(!i2c_read->scanner->scanned) {
        scan_i2c_bus(i2c_read->scanner);
        i2c_read->address_num = i2c_read->scanner->nb_found;
    }

    canvas_set_font(canvas, FontSecondary);
    if(!i2c_read->address_num) {
        canvas_draw_str_aligned(canvas, 20, 5, AlignLeft, AlignTop, "No memory chips found");
        return;
    }

    char addr_text[8];

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
        // I2C addr
        canvas_set_color(canvas, ColorBlack);
        canvas_draw_str_aligned(canvas, 3, 3, AlignLeft, AlignTop, "I2C addr:");
        canvas_draw_icon(canvas, 58, 5, &I_ButtonDown_7x4);
        canvas_draw_icon(canvas, 100, 5, &I_ButtonUp_7x4);
        uint8_t i2c_addr = i2c_read->scanner->addresses[i2c_read->address_idx];
        snprintf(addr_text, sizeof(addr_text), "0x%02x", i2c_addr);
        canvas_draw_str_aligned(canvas, 83, 3, AlignCenter, AlignTop, addr_text);

        // Page addr
        canvas_draw_str_aligned(canvas, 3, 13, AlignLeft, AlignTop, "Page addr:");
        canvas_draw_icon(canvas, 60, 13, &I_ButtonLeft_4x7);
        canvas_draw_icon(canvas, 102, 13, &I_ButtonRight_4x7);
        snprintf(addr_text, sizeof(addr_text), "%d", (int)i2c_read->addr);
        canvas_draw_str_aligned(canvas, 83, 13, AlignCenter, AlignTop, addr_text);
    }

    // Read Button
    canvas_draw_rbox(canvas, 45, 48, 45, 13, 3);
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_icon(canvas, 50, 50, &I_Ok_btn_9x9);
    canvas_draw_str_aligned(canvas, 62, 51, AlignLeft, AlignTop, "Read");
}