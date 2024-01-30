#include "main_view.h"

#define MENU_X_START    50
#define MENU_WIDTH      76
#define MENU_X_CENTER   (MENU_X_START + (MENU_WIDTH / 2))
#define MENU_X_END      (MENU_X_START + MENU_WIDTH)
#define MENU_Y_STEP     12

//#define SCAN_MENU_TEXT "Find all chips"
#define SCAN_MENU_TEXT "Find chip(s)"
#define SCAN_MENU_Y     4

#define READ_MENU_TEXT "Read test page"
#define READ_MENU_Y     (SCAN_MENU_Y + MENU_Y_STEP)

#define WRITE_MENU_TEXT "Write test page"
#define WRITE_MENU_Y    (READ_MENU_Y + MENU_Y_STEP)

#define KILL_MENU_TEXT "Kill the chip"
#define KILL_MENU_Y     (WRITE_MENU_Y + MENU_Y_STEP)

#define INFOS_MENU_TEXT "Settings"
#define INFOS_MENU_Y    (KILL_MENU_Y + MENU_Y_STEP)

void draw_main_view(Canvas* canvas, i2cMainView* main_view) {
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_rframe(canvas, MENU_X_START - 2, 0, MENU_WIDTH + 4, 64, 5);

    canvas_set_bitmap_mode(canvas, true);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 0, 8, "24 series");
    canvas_draw_str(canvas, 0, 17, "EEPROM");
    canvas_draw_str(canvas, 0, 26, "killer");
    canvas_draw_frame(canvas, 9, 32, 15, 25);
    canvas_draw_frame(canvas, 7, 33, 3, 5);
    canvas_draw_frame(canvas, 7, 39, 3, 5);
    canvas_draw_frame(canvas, 7, 45, 3, 5);
    canvas_draw_frame(canvas, 7, 51, 3, 5);
    canvas_draw_frame(canvas, 23, 51, 3, 5);
    canvas_draw_frame(canvas, 23, 45, 3, 5);
    canvas_draw_frame(canvas, 23, 39, 3, 5);
    canvas_draw_frame(canvas, 23, 33, 3, 5);
    canvas_draw_line(canvas, 0, 59, 8, 59);
    canvas_draw_line(canvas, 2, 61, 6, 61);
    canvas_draw_line(canvas, 4, 63, 4, 63);
    canvas_draw_line(canvas, 4, 59, 4, 36);
    canvas_draw_line(canvas, 5, 35, 7, 35);
    canvas_draw_line(canvas, 3, 41, 7, 41);
    canvas_draw_line(canvas, 3, 47, 7, 47);
    canvas_draw_line(canvas, 3, 53, 7, 53);
    canvas_draw_circle(canvas, 12, 35, 1);
    canvas_draw_line(canvas, 27, 63, 27, 63);
    canvas_draw_line(canvas, 25, 61, 29, 61);
    canvas_draw_line(canvas, 23, 59, 31, 59);
    canvas_draw_line(canvas, 27, 59, 27, 42);
    canvas_draw_line(canvas, 26, 41, 25, 41);
    canvas_draw_line(canvas, 25, 35, 34, 35);
    canvas_draw_line(canvas, 39, 31, 29, 31);
    canvas_draw_line(canvas, 34, 35, 34, 31);
    canvas_draw_line(canvas, 25, 47, 36, 47);
    canvas_draw_line(canvas, 25, 53, 36, 53);
    canvas_draw_line(canvas, 30, 47, 30, 43);
    canvas_draw_box(canvas, 29, 46, 3, 3);
    canvas_draw_box(canvas, 33, 52, 3, 3);
    canvas_draw_box(canvas, 3, 46, 3, 3);
    canvas_draw_box(canvas, 3, 40, 3, 3);
    canvas_draw_line(canvas, 34, 53, 34, 43);
    canvas_draw_box(canvas, 33, 34, 3, 3);
    canvas_draw_box(canvas, 3, 52, 3, 3);
    canvas_draw_box(canvas, 29, 34, 3, 3);
    canvas_draw_line(canvas, 38, 52, 38, 54);
    canvas_draw_dot(canvas, 39, 51);
    canvas_draw_line(canvas, 30, 39, 30, 35);
    canvas_draw_line(canvas, 35, 25, 34, 29);
    canvas_draw_line(canvas, 29, 25, 30, 25);
    canvas_draw_dot(canvas, 39, 55);
    canvas_draw_line(canvas, 34, 39, 34, 35);
    canvas_draw_line(canvas, 33, 25, 34, 29);
    canvas_draw_line(canvas, 29, 29, 30, 29);
    canvas_draw_dot(canvas, 40, 52);
    canvas_draw_line(canvas, 37, 29, 38, 29);
    canvas_draw_dot(canvas, 31, 26);
    canvas_draw_dot(canvas, 40, 54);
    canvas_draw_dot(canvas, 39, 28);
    canvas_draw_dot(canvas, 30, 27);
    canvas_draw_line(canvas, 38, 48, 38, 46);
    canvas_draw_dot(canvas, 38, 27);
    canvas_draw_dot(canvas, 31, 28);
    canvas_draw_dot(canvas, 39, 45);
    canvas_draw_dot(canvas, 39, 26);
    canvas_draw_dot(canvas, 39, 49);
    canvas_draw_line(canvas, 38, 25, 37, 25);
    canvas_draw_dot(canvas, 40, 46);
    canvas_draw_dot(canvas, 40, 48);
    canvas_draw_line(canvas, 42, 46, 42, 48);
    canvas_draw_line(canvas, 44, 46, 44, 48);
    canvas_draw_dot(canvas, 43, 45);
    canvas_draw_dot(canvas, 43, 49);
    canvas_draw_line(canvas, 43, 51, 43, 55);
    canvas_draw_line(canvas, 42, 55, 44, 55);
    canvas_draw_line(canvas, 42, 52, 43, 52);
    canvas_draw_line(canvas, 13, 43, 13, 47);
    canvas_draw_line(canvas, 13, 43, 19, 43);
    canvas_draw_line(canvas, 14, 42, 18, 42);
    canvas_draw_frame(canvas, 29, 39, 3, 5);
    canvas_draw_line(canvas, 16, 43, 16, 48);
    canvas_draw_frame(canvas, 33, 39, 3, 5);
    canvas_draw_line(canvas, 13, 47, 19, 47);
    canvas_draw_line(canvas, 19, 43, 19, 47);
    canvas_draw_dot(canvas, 16, 42);
    canvas_draw_dot(canvas, 16, 42);
    canvas_draw_line(canvas, 13, 46, 19, 46);
    canvas_draw_line(canvas, 14, 47, 14, 48);
    canvas_draw_line(canvas, 18, 47, 18, 48);

    canvas_set_font(canvas, FontSecondary);

    switch(main_view->menu_index) {
    case SCAN_VIEW:
        canvas_set_color(canvas, ColorBlack);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, READ_MENU_Y, AlignCenter, AlignTop, READ_MENU_TEXT);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, WRITE_MENU_Y, AlignCenter, AlignTop, WRITE_MENU_TEXT);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, KILL_MENU_Y, AlignCenter, AlignTop, KILL_MENU_TEXT);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, INFOS_MENU_Y, AlignCenter, AlignTop, INFOS_MENU_TEXT);
        canvas_draw_rbox(
            canvas, MENU_X_START, SCAN_MENU_Y - 2, MENU_WIDTH, 12, 3);
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, SCAN_MENU_Y, AlignCenter, AlignTop, SCAN_MENU_TEXT);
        break;

    case READ_VIEW:
        canvas_set_color(canvas, ColorBlack);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, SCAN_MENU_Y, AlignCenter, AlignTop, SCAN_MENU_TEXT);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, WRITE_MENU_Y, AlignCenter, AlignTop, WRITE_MENU_TEXT);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, KILL_MENU_Y, AlignCenter, AlignTop, KILL_MENU_TEXT);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, INFOS_MENU_Y, AlignCenter, AlignTop, INFOS_MENU_TEXT);
        canvas_draw_rbox(
            canvas, MENU_X_START, READ_MENU_Y - 2, MENU_WIDTH, 12, 3);
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, READ_MENU_Y, AlignCenter, AlignTop, READ_MENU_TEXT);
        break;

    case WRITE_VIEW:
        canvas_set_color(canvas, ColorBlack);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, SCAN_MENU_Y, AlignCenter, AlignTop, SCAN_MENU_TEXT);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, READ_MENU_Y, AlignCenter, AlignTop, READ_MENU_TEXT);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, INFOS_MENU_Y, AlignCenter, AlignTop, INFOS_MENU_TEXT);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, KILL_MENU_Y, AlignCenter, AlignTop, KILL_MENU_TEXT);
        canvas_draw_rbox(
            canvas, MENU_X_START, WRITE_MENU_Y - 2, MENU_WIDTH, 12, 3);
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, WRITE_MENU_Y, AlignCenter, AlignTop, WRITE_MENU_TEXT);
        break;

    case KILL_VIEW:
        canvas_set_color(canvas, ColorBlack);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, SCAN_MENU_Y, AlignCenter, AlignTop, SCAN_MENU_TEXT);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, READ_MENU_Y, AlignCenter, AlignTop, READ_MENU_TEXT);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, WRITE_MENU_Y, AlignCenter, AlignTop, WRITE_MENU_TEXT);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, INFOS_MENU_Y, AlignCenter, AlignTop, INFOS_MENU_TEXT);
        canvas_draw_rbox(
            canvas, MENU_X_START, KILL_MENU_Y - 2, MENU_WIDTH, 12, 3);
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, KILL_MENU_Y, AlignCenter, AlignTop, KILL_MENU_TEXT);
        break;

    case SETTINGS_VIEW:
        canvas_set_color(canvas, ColorBlack);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, SCAN_MENU_Y, AlignCenter, AlignTop, SCAN_MENU_TEXT);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, KILL_MENU_Y, AlignCenter, AlignTop, KILL_MENU_TEXT);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, READ_MENU_Y, AlignCenter, AlignTop, READ_MENU_TEXT);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, WRITE_MENU_Y, AlignCenter, AlignTop, WRITE_MENU_TEXT);
        canvas_draw_rbox(
            canvas, MENU_X_START, INFOS_MENU_Y - 2, MENU_WIDTH, 12, 3);
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_str_aligned(
            canvas, MENU_X_CENTER, INFOS_MENU_Y, AlignCenter, AlignTop, INFOS_MENU_TEXT);
        break;

    default:
        break;
    }
}

i2cMainView* i2c_main_view_alloc() {
    i2cMainView* main_view = malloc(sizeof(i2cMainView));
    main_view->menu_index = SCAN_VIEW;
    main_view->current_view = MAIN_VIEW;
    return main_view;
}

void i2c_main_view_free(i2cMainView* main_view) {
    furi_assert(main_view);
    free(main_view);
}