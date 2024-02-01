#include <i2ctools_i.h>
#include <furi_hal_cortex.h>
#include "i2ckill.h"

void i2c_kill() {
    uint32_t time_gr_writes_start = 0;
    uint32_t time_gr_writes_end = 0;

    for(uint32_t i = 0; i < 1000; i++) {
        i2c_write(i2ctools->write);

        //if((i2ctools->written % 10) == 0) {
            i2c_read(i2ctools->read);
            if (!buffers_are_equal())
                break;
            if (!i2ctools->write->written || !i2ctools->read->readed)
                break;
        //}

        if((i2ctools->written % 100) == 0) {
            if (i2ctools->written && time_gr_writes_start) {
                time_gr_writes_end = get_time_us() / 1000;
                if(time_gr_writes_end > time_gr_writes_start) {
                    float wr_per_sec = 100;
                    wr_per_sec /= (time_gr_writes_end - time_gr_writes_start);
                    wr_per_sec *= 1000;
                    i2ctools->last_gr_writes_per_sec = wr_per_sec;
                }

                furi_delay_ms(1);
            }

            time_gr_writes_start = get_time_us() / 1000;
        }

        i2ctools->pattern++;
        i2ctools->written++;
        fill_tx_buff_by_pattern();

        if(!i2ctools->test_running)
            break;
    }

    i2ctools->test_running = false;
}
