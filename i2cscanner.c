#include "i2cscanner.h"
#include <i2ctools_i.h>

void scan_i2c_bus(i2cScanner* i2c_scanner) {
    i2c_scanner->nb_found = 0;
    i2c_scanner->scanned = true;
    // Get the bus
    furi_hal_i2c_acquire(I2C_BUS);
    // Scan
    for(uint8_t addr7bit = 0x50; addr7bit <= 0x57; addr7bit++) {
        // Check for peripherals
        if(furi_hal_i2c_is_device_ready(I2C_BUS, addr7bit << 1, I2C_TIMEOUT)) {
            i2c_scanner->addresses[i2c_scanner->nb_found] = addr7bit;
            i2c_scanner->nb_found++;
        }
    }

    i2c_scanner->addresses[i2c_scanner->nb_found] = 0x51;
    i2c_scanner->nb_found++;
    i2c_scanner->addresses[i2c_scanner->nb_found] = 0x52;
    i2c_scanner->nb_found++;

    furi_hal_i2c_release(I2C_BUS);
}

i2cScanner* i2c_scanner_alloc() {
    i2cScanner* i2c_scanner = malloc(sizeof(i2cScanner));
    i2c_scanner->nb_found = 0;
    i2c_scanner->scanned = false;
    return i2c_scanner;
}

void i2c_scanner_free(i2cScanner* i2c_scanner) {
    furi_assert(i2c_scanner);
    free(i2c_scanner);
}