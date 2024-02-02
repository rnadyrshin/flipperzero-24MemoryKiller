#include <i2ctools_i.h>
#include "i2cscanner.h"

void scan_i2c_bus() {
    i2ctools->address_num = 0;
    i2ctools->scanned = true;
    // Get the bus
    furi_hal_i2c_acquire(I2C_BUS);
    // Scan
    for(uint8_t addr7bit = 0x50; addr7bit <= 0x57; addr7bit++) {
        // Check for peripherals
        if(furi_hal_i2c_is_device_ready(I2C_BUS, addr7bit << 1, I2C_TIMEOUT)) {
            i2ctools->addresses[i2ctools->address_num] = addr7bit;
            i2ctools->address_num++;
        }
    }
/*
    i2ctools->addresses[i2ctools->address_num] = 0x51;
    i2ctools->address_num++;
    i2ctools->addresses[i2ctools->address_num] = 0x52;
    i2ctools->address_num++;
*/
    furi_hal_i2c_release(I2C_BUS);
}
