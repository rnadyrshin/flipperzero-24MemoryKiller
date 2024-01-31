#include "../i2ctools_i.h"
#include "read_view.h"

void draw_read_view(Canvas* canvas, i2cRead* i2c_read) {
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_rframe(canvas, 0, 0, 128, 64, 3);
    canvas_set_font(canvas, FontSecondary);

    if(!i2ctools->scanned)
        scan_i2c_bus();

    if(!i2ctools->address_num) {
        canvas_draw_str_aligned(canvas, 20, 5, AlignLeft, AlignTop, "No memory chips found");
        return;
    }

    if(i2c_read->readed) {
        // Result
        char byte_text[5];
        for(uint16_t byte = 0; byte < i2ctools->rx_len; byte++) {
            uint8_t x_pos = 5 + (byte % 8) * 15;
            uint8_t y_pos = 2 + (byte / 8) * 8;
            snprintf(byte_text, sizeof(byte_text), "%02X", (int)i2ctools->rx_buff[byte]);
            canvas_draw_str_aligned(canvas, x_pos, y_pos, AlignLeft, AlignTop, byte_text);
        }
    }
    else {
/*
        // I2C addr
        char addr_text[8];
        canvas_set_color(canvas, ColorBlack);
        canvas_draw_str_aligned(canvas, 3, 3, AlignLeft, AlignTop, "I2C addr:");
        canvas_draw_icon(canvas, 45, 5, &I_ButtonDown_7x4);
        canvas_draw_icon(canvas, 82, 5, &I_ButtonUp_7x4);
        uint8_t i2c_addr = i2ctools->addresses[i2ctools->address_idx];
        snprintf(addr_text, sizeof(addr_text), "0x%02x", i2c_addr);
        canvas_draw_str_aligned(canvas, 67, 3, AlignCenter, AlignTop, addr_text);
*/
        char text[37];
        chip_model chip = i2ctools->chip;
        uint16_t page_size = chip_to_page_size(chip);
        snprintf(text, sizeof(text), "%d bytes will be read", page_size);
        canvas_draw_str_aligned(canvas, 64, 20, AlignCenter, AlignTop, text);
        snprintf(text, sizeof(text), "from page %d", i2ctools->test_page);
        canvas_draw_str_aligned(canvas, 64, 30, AlignCenter, AlignTop, text);
    }

    // Read Button
    canvas_draw_rbox(canvas, 45, 48, 45, 13, 3);
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_icon(canvas, 50, 50, &I_Ok_btn_9x9);
    canvas_draw_str_aligned(canvas, 62, 51, AlignLeft, AlignTop, "Read");
}