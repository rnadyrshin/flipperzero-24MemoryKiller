#pragma once

#include <furi.h>
#include <furi_hal.h>
#include "i2ckill.h"

typedef struct {
    //uint32_t time_start_1write;
    //uint8_t address_idx;
} i2cKill;

void i2c_kill();
