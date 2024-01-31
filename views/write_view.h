#pragma once

#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <24memkiller_icons.h>
#include "../i2cwrite.h"

void draw_write_view(Canvas* canvas, i2cWrite* i2c_write);