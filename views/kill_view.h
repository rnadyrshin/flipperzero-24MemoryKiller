#pragma once

#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <24memkiller_icons.h>
#include "../i2ckill.h"

void draw_kill_view(Canvas* canvas, i2cKill* i2c_kill);