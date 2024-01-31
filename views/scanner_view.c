#include "../i2ctools_i.h"
#include "scanner_view.h"

void draw_scanner_view(Canvas* canvas) {
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_rframe(canvas, 0, 0, 128, 64, 3);

    // Scan
    scan_i2c_bus();

    char count_text[46];
    char count_text_fmt[] = "EEPROM chips found: %d";
    canvas_set_font(canvas, FontSecondary);
    snprintf(count_text, sizeof(count_text), count_text_fmt, i2ctools->address_num);
    canvas_draw_str_aligned(canvas, 3, 3, AlignLeft, AlignTop, count_text);

    for(uint8_t i = 0; i < i2ctools->address_num; i++) {
        uint8_t addr7bit = i2ctools->addresses[i];
        uint8_t a0 = addr7bit & 0x1 ? 1 : 0;
        uint8_t a1 = addr7bit & 0x2 ? 1 : 0;
        uint8_t a2 = addr7bit & 0x4 ? 1 : 0;

        snprintf(count_text, sizeof(count_text), "0x%02x (A0=%d, A1=%d, A2=%d)", addr7bit, a0, a1, a2);

        canvas_draw_str_aligned(canvas, 3, 3 + (i + 1) * 8, AlignLeft, AlignTop, count_text);
    }
}