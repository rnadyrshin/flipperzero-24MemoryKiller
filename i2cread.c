#include "i2cread.h"
#include <i2ctools_i.h>
#include <notification/notification_messages.h>

void i2c_read(i2cRead* i2c_read) {
    furi_hal_i2c_acquire(I2C_BUS);

    if(!i2ctools->scanned)
        scan_i2c_bus();

    if(!i2ctools->address_num) {
        i2c_read->readed = false;
        i2ctools->rx_len = 0;
        return;
    }

    chip_model chip = i2ctools->chip;
    uint8_t i2c_addr_8bit = i2ctools->addresses[i2ctools->address_idx] << 1;
    uint8_t int_addr_len = chip_to_addr_size(chip);
    uint16_t page_size = chip_to_page_size(chip);
    uint16_t int_addr_start = i2ctools->test_page * page_size;

    uint8_t tx_buff[2];
    if(int_addr_len == 2){
        tx_buff[0] = int_addr_start >> 8;
        tx_buff[1] = int_addr_start & 0xFF;
    }
    else {
        tx_buff[0] = int_addr_start & 0xFF;
    }

    FURI_CRITICAL_ENTER();

    bool ok = furi_hal_i2c_trx(
        I2C_BUS,
        i2c_addr_8bit,
        tx_buff,
        int_addr_len,
        i2ctools->rx_buff,
        page_size,
        I2C_TIMEOUT);

    FURI_CRITICAL_EXIT();

    if(ok) {
        i2c_read->readed = true;
        i2ctools->rx_len = page_size;
    }
    else {
        i2c_read->readed = false;
        i2ctools->rx_len = 0;
    }

    furi_hal_i2c_release(I2C_BUS);
}

i2cRead* i2c_read_alloc() {
    i2cRead* i2c_read = malloc(sizeof(i2cRead));
    i2c_read->readed = false;
    return i2c_read;
}

void i2c_read_free(i2cRead* i2c_read) {
    furi_assert(i2c_read);
    free(i2c_read);
}