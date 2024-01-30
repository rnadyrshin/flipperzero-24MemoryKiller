#pragma once

#include <furi.h>
#include <furi_hal.h>

// I2C BUS
#define I2C_TIMEOUT     3

typedef struct {
    uint8_t addresses[8];
    uint8_t nb_found;
    bool scanned;
} i2cScanner;

void scan_i2c_bus(i2cScanner* i2c_scanner);
i2cScanner* i2c_scanner_alloc();
void i2c_scanner_free(i2cScanner* i2c_scanner);
