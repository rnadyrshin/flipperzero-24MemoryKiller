#include "../i2ctools_i.h"
#include "kill_view.h"

void draw_kill_view(Canvas* canvas) {
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

    char addr_text[32];

    snprintf(addr_text, sizeof(addr_text), "Written:  %lu", i2ctools->written);
    canvas_draw_str_aligned(canvas, 3, 3, AlignLeft, AlignTop, addr_text);

    snprintf(addr_text, sizeof(addr_text), "Page write: %lu (%lu) us", 
        i2ctools->last_1write_full_time_us, i2ctools->last_1write_write_time_us);
    canvas_draw_str_aligned(canvas, 3, 13, AlignLeft, AlignTop, addr_text);

    snprintf(addr_text, sizeof(addr_text), "Pages per sec: %.2f", (double)i2ctools->last_gr_writes_per_sec);
    canvas_draw_str_aligned(canvas, 3, 23, AlignLeft, AlignTop, addr_text);

    // Start Button
    canvas_draw_rbox(canvas, 45, 48, 45, 13, 3);
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_icon(canvas, 50, 50, &I_Ok_btn_9x9);
    if(i2ctools->test_running) {
        canvas_draw_str_aligned(canvas, 62, 51, AlignLeft, AlignTop, "Pause");
    } else {
        canvas_draw_str_aligned(canvas, 62, 51, AlignLeft, AlignTop, "Start");
    }
}