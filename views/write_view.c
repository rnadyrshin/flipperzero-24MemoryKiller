#include "../i2ctools_i.h"
#include "write_view.h"

void draw_write_view(Canvas* canvas, i2cWrite* i2c_write) {
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

    char addr_text[8];

    if(i2c_write)
        i2c_write = 0;
/*
    if(i2c_write->readed) {
        // Result
        for(uint16_t byte = 0; byte < i2c_write->len; byte++) {
            uint8_t x_pos = 5 + (byte % 8) * 15;
            uint8_t y_pos = 2 + (byte / 8) * 8;
            snprintf(addr_text, sizeof(addr_text), "%02X", (int)i2c_write->recv[byte]);
            canvas_draw_str_aligned(canvas, x_pos, y_pos, AlignLeft, AlignTop, addr_text);
        }
    }
    else */{
/*        
        // I2C addr
        canvas_set_color(canvas, ColorBlack);
        canvas_draw_str_aligned(canvas, 3, 3, AlignLeft, AlignTop, "I2C addr:");
        canvas_draw_icon(canvas, 58, 5, &I_ButtonDown_7x4);
        canvas_draw_icon(canvas, 100, 5, &I_ButtonUp_7x4);
        uint8_t i2c_addr = i2ctools->addresses[i2ctools->address_idx];
        snprintf(addr_text, sizeof(addr_text), "0x%02x", i2c_addr);
        canvas_draw_str_aligned(canvas, 83, 3, AlignCenter, AlignTop, addr_text);
*/
        // Pattern num
        uint8_t pattern = i2ctools->pattern;
        canvas_draw_str_aligned(canvas, 3, 03, AlignLeft, AlignTop, "Pattern:");
        canvas_draw_icon(canvas, 60, 3, &I_ButtonLeft_4x7);
        canvas_draw_icon(canvas, 102, 3, &I_ButtonRight_4x7);
        snprintf(addr_text, sizeof(addr_text), "%d", pattern);
        canvas_draw_str_aligned(canvas, 83, 3, AlignCenter, AlignTop, addr_text);

        // Pattern
        uint16_t page_size = i2ctools->page_size;
        i2ctools->tx_len = page_size;
        // 24C00 (page_size == 1)
        if(page_size == 1) {
            i2ctools->tx_buff[0] = pattern;
            snprintf(addr_text, sizeof(addr_text), "%02X", i2ctools->tx_buff[0]);
            canvas_draw_str_aligned(canvas, 5, 13, AlignLeft, AlignTop, addr_text);
        }
        // All other chips
        else {
            for(uint16_t byte = 0; byte < page_size / 2; byte++) {
                // Static part
                i2ctools->tx_buff[byte] = byte;
                // Dynamic part
                i2ctools->tx_buff[(page_size / 2) + byte] = pattern + byte;
            }
        }
        // Print buffer
        for(uint16_t byte = 0; byte < page_size; byte++) {
            uint8_t x_pos = 5 + (byte % 8) * 15;
            uint8_t y_pos = 13 + (byte / 8) * 8;
            snprintf(addr_text, sizeof(addr_text), "%02X", i2ctools->tx_buff[byte]);
            canvas_draw_str_aligned(canvas, x_pos, y_pos, AlignLeft, AlignTop, addr_text);
        }
    }

    // Write Button
    canvas_draw_rbox(canvas, 45, 48, 45, 13, 3);
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_icon(canvas, 50, 50, &I_Ok_btn_9x9);
    canvas_draw_str_aligned(canvas, 62, 51, AlignLeft, AlignTop, "Write");
}