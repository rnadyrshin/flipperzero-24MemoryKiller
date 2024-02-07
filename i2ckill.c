#include <i2ctools_i.h>
#include <furi_hal_cortex.h>
#include "i2ckill.h"

static int32_t test_worker(void* context) {
    furi_assert(context);

    uint32_t time_gr_writes_start = 0;
    uint32_t time_gr_writes_end = 0;
    i2cTools* cfg = context;
    cfg->test_error = false;
    cfg->test_running = true;
    notification_message(cfg->notification, &sequence_blink_start_cyan);
    
    while (!cfg->test_need_to_stop) {
        fill_tx_buff_by_pattern();
        i2c_write(cfg->write);
        i2c_read(cfg->read);

        if (!cfg->write->written || !cfg->read->readed || !buffers_are_equal()) {
            cfg->test_error = true;
            break;
        }

        if((cfg->written % 100) == 0) {
            if (cfg->written && time_gr_writes_start) {
                time_gr_writes_end = get_time_us() / 1000;
                if(time_gr_writes_end > time_gr_writes_start) {
                    float wr_per_sec = 100;
                    wr_per_sec /= (time_gr_writes_end - time_gr_writes_start);
                    wr_per_sec *= 1000;
                    cfg->last_gr_writes_per_sec = wr_per_sec;
                }

                furi_delay_ms(1);
            }

            time_gr_writes_start = get_time_us() / 1000;
        }

        cfg->pattern++;
        cfg->written++;
    }

    notification_message(cfg->notification, &sequence_blink_stop);
    if(cfg->test_error) {
        notification_message(cfg->notification, &sequence_error);
        notification_message(cfg->notification, &sequence_set_only_red_255);
    }

    cfg->test_running = false;
    return 0;
}

void i2c_test_start() {
    if(i2ctools->test_running) {
        i2c_test_stop();
    }

    i2ctools->test_need_to_stop = false;
    i2ctools->scan_worker = furi_thread_alloc();
    furi_thread_set_name(i2ctools->scan_worker, "TestWorker");
    furi_thread_set_context(i2ctools->scan_worker, i2ctools);
    furi_thread_set_stack_size(i2ctools->scan_worker, 4 * 1024);
    //furi_thread_set_priority(i2ctools->scan_worker, FuriThreadPriorityIdle);
    furi_thread_set_callback(i2ctools->scan_worker, test_worker);
    furi_thread_start(i2ctools->scan_worker);
}

void i2c_test_stop() {
    if(!i2ctools->test_running)
        return;
    if(!i2ctools->scan_worker)
        return;

    i2ctools->test_need_to_stop = true;

    furi_thread_join(i2ctools->scan_worker);
    furi_thread_free(i2ctools->scan_worker);

    i2ctools->scan_worker = NULL;
    i2ctools->test_error = false;
}