#include "i2cread.h"
#include <i2ctools_i.h>
#include <notification/notification_messages.h>

void i2c_read(i2cRead* i2c_read) {
    furi_hal_i2c_acquire(I2C_BUS);

    if(!i2c_read->scanner->scanned) {
        scan_i2c_bus(i2c_read->scanner);
        i2c_read->address_num = i2c_read->scanner->nb_found;
    }

    if(!i2c_read->address_num) {
        i2c_read->readed = false;
        i2c_read->len = 0;
        return;
    }

    uint8_t i2c_addr = i2c_read->scanner->addresses[i2c_read->address_idx] << 1;
    uint8_t tx_buff[2];
    tx_buff[0] = i2c_read->addr >> 8;
    tx_buff[1] = i2c_read->addr & 0xFF;

    bool ok = furi_hal_i2c_trx(
        I2C_BUS,
        i2c_addr,
        tx_buff,
        2,
        i2c_read->recv,
        i2c_read->len,
        I2C_TIMEOUT);

    if(ok) {
        i2c_read->readed = true;
        notification_message(i2ctools->notification, &sequence_blink_yellow_100);
    }
    else {
        i2c_read->readed = false;
        i2c_read->len = 0;
        notification_message(i2ctools->notification, &sequence_blink_red_100);
    }

    furi_hal_i2c_release(I2C_BUS);
}

i2cRead* i2c_read_alloc() {
    i2cRead* i2c_read = malloc(sizeof(i2cRead));
    i2c_read->address_idx = 0;
    i2c_read->address_num = 0;
    i2c_read->readed = false;
    return i2c_read;
}

void i2c_read_free(i2cRead* i2c_read) {
    furi_assert(i2c_read);
    free(i2c_read);
}