#pragma once

#include <furi.h>
#include <furi_hal.h>
#include "i2cscanner.h"

typedef struct {
    bool readed;
} i2cRead;

void i2c_read(i2cRead* i2c_read);
i2cRead* i2c_read_alloc();
void i2c_read_free(i2cRead* i2c_read);
