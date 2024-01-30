#pragma once

#include <furi.h>
#include <furi_hal.h>
#include "i2cscanner.h"

typedef struct {
    uint8_t address_idx;
    uint8_t address_num;
    uint16_t addr;
    uint16_t len;
    uint8_t recv[256];
    bool readed;

    i2cScanner* scanner;
} i2cRead;

void i2c_read(i2cRead* i2c_read);
i2cRead* i2c_read_alloc();
void i2c_read_free(i2cRead* i2c_read);
