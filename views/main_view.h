#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <24memkiller_icons.h>
#define APP_NAME "I2C Tools"

// Menu
typedef enum {
    MAIN_VIEW,
    SCAN_VIEW,
    READ_VIEW,
    WRITE_VIEW,
    KILL_VIEW,
    SETTINGS_VIEW,

    /* Know menu Size*/
    MENU_SIZE
} i2cToolsViews;

typedef struct {
    i2cToolsViews current_view;
    i2cToolsViews menu_index;
} i2cMainView;

void draw_main_view(Canvas* canvas, i2cMainView* main_view);

i2cMainView* i2c_main_view_alloc();
void i2c_main_view_free(i2cMainView* main_view);