#include "i2cwrite.h"
#include <i2ctools_i.h>
#include <notification/notification_messages.h>

void i2c_write(i2cWrite* i2c_write) {
    furi_hal_i2c_acquire(I2C_BUS);

    if(!i2ctools->scanned)
        scan_i2c_bus();

    if(!i2ctools->address_num) {
        i2c_write->written = false;
        i2ctools->tx_len = 0;
        return;
    }

    chip_model chip = i2ctools->chip;
    uint8_t i2c_addr_8bit = i2ctools->addresses[i2ctools->address_idx] << 1;
    uint8_t int_addr_len = chip_to_addr_size(chip);
    uint16_t page_size = chip_to_page_size(chip);
    uint16_t int_addr_start = i2ctools->test_page * page_size;

    uint8_t buff[2 + CHIP_MAX_PAGE_SIZE];
    if(int_addr_len == 2){
        buff[0] = int_addr_start >> 8;
        buff[1] = int_addr_start & 0xFF;
    }
    else {
        buff[0] = int_addr_start & 0xFF;
    }
    memcpy(&buff[int_addr_len], i2ctools->tx_buff, page_size);

    FURI_CRITICAL_ENTER();

    uint32_t time_1write_send_start = get_time_us();

    bool ok = furi_hal_i2c_tx(
        I2C_BUS,
        i2c_addr_8bit,
        buff,
        page_size + int_addr_len,
        I2C_TIMEOUT);

    uint32_t time_1write_write_start = get_time_us();

    if(ok) {
        uint16_t try_cnt = 500;
        while (try_cnt--) {
            if(furi_hal_i2c_is_device_ready(I2C_BUS, i2c_addr_8bit, I2C_TIMEOUT))
                break;
        }
    }

    uint32_t time_1write_write_end = get_time_us();
    
    if(time_1write_write_end > time_1write_write_start)
        i2ctools->last_1write_write_time_us = time_1write_write_end - time_1write_write_start;
    if(time_1write_write_end > time_1write_send_start)
        i2ctools->last_1write_full_time_us = time_1write_write_end - time_1write_send_start;

    FURI_CRITICAL_EXIT();

    if(ok) {
        i2c_write->written = true;
        i2ctools->tx_len = page_size;
    }
    else {
        i2c_write->written = false;
        i2ctools->tx_len = 0;
    }

    furi_hal_i2c_release(I2C_BUS);
}

i2cWrite* i2c_write_alloc() {
    i2cWrite* i2c_write = malloc(sizeof(i2cWrite));
    i2c_write->written = false;
    return i2c_write;
}

void i2c_write_free(i2cWrite* i2c_write) {
    furi_assert(i2c_write);
    free(i2c_write);
}