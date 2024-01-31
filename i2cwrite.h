#pragma once

#include <furi.h>
#include <furi_hal.h>
#include "i2cscanner.h"

typedef struct {
    bool written;
} i2cWrite;

void i2c_write(i2cWrite* i2c_write);
i2cWrite* i2c_write_alloc();
void i2c_write_free(i2cWrite* i2c_write);
