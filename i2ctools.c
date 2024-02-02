#include "i2ctools_i.h"

#define I2C_CONFIG_I2C_100KHZ 0x10707DBC
#define I2C_CONFIG_I2C_400KHZ 0x00602173

i2cTools* i2ctools = 0;
void i2c_bus_handle_event(FuriHalI2cBusHandle* handle, FuriHalI2cBusHandleEvent event);

FuriHalI2cBusHandle i2c_handle = {
    .bus = &furi_hal_i2c_bus_external,
    .callback = i2c_bus_handle_event,
};

void i2c_bus_handle_event(FuriHalI2cBusHandle* handle, FuriHalI2cBusHandleEvent event) {
    if(event == FuriHalI2cBusHandleEventActivate) {
        furi_hal_gpio_init_ex(
            &gpio_ext_pc0, GpioModeAltFunctionOpenDrain, GpioPullNo, GpioSpeedLow, GpioAltFn4I2C3);
        furi_hal_gpio_init_ex(
            &gpio_ext_pc1, GpioModeAltFunctionOpenDrain, GpioPullNo, GpioSpeedLow, GpioAltFn4I2C3);

        LL_I2C_InitTypeDef I2C_InitStruct;
        I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
        I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
        I2C_InitStruct.DigitalFilter = 0;
        I2C_InitStruct.OwnAddress1 = 0;
        I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
        I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
        I2C_InitStruct.Timing = I2C_CONFIG_I2C_400KHZ;
        LL_I2C_Init(handle->bus->i2c, &I2C_InitStruct);
        // I2C is enabled at this point
        LL_I2C_EnableAutoEndMode(handle->bus->i2c);
        LL_I2C_SetOwnAddress2(handle->bus->i2c, 0, LL_I2C_OWNADDRESS2_NOMASK);
        LL_I2C_DisableOwnAddress2(handle->bus->i2c);
        LL_I2C_DisableGeneralCall(handle->bus->i2c);
        LL_I2C_EnableClockStretching(handle->bus->i2c);
    } else if(event == FuriHalI2cBusHandleEventDeactivate) {
        LL_I2C_Disable(handle->bus->i2c);
        furi_hal_gpio_write(&gpio_ext_pc0, 1);
        furi_hal_gpio_write(&gpio_ext_pc1, 1);
        furi_hal_gpio_init_ex(
            &gpio_ext_pc0, GpioModeAnalog, GpioPullNo, GpioSpeedLow, GpioAltFnUnused);
        furi_hal_gpio_init_ex(
            &gpio_ext_pc1, GpioModeAnalog, GpioPullNo, GpioSpeedLow, GpioAltFnUnused);
    }
}

uint32_t get_time_us() {
    uint32_t ticks = DWT->CYCCNT;
    return ticks / furi_hal_cortex_instructions_per_microsecond();
}

void i2ctools_draw_callback(Canvas* canvas, void* ctx) {
    i2ctools = ctx;
    if(furi_mutex_acquire(i2ctools->mutex, 200) != FuriStatusOk) {
        return;
    }

    switch(i2ctools->main_view->current_view) {
    case MAIN_VIEW:
        draw_main_view(canvas, i2ctools->main_view);
        break;
    case SCAN_VIEW:
        draw_scanner_view(canvas);
        break;
    case READ_VIEW:
        draw_read_view(canvas, i2ctools->read);
        break;
    case WRITE_VIEW:
        draw_write_view(canvas);
        break;
    case KILL_VIEW:
        draw_kill_view(canvas);
        break;
    case SETTINGS_VIEW:
        draw_settings_view(canvas);
        break;
    default:
        break;
    }
    furi_mutex_release(i2ctools->mutex);
}

void i2ctools_input_callback(InputEvent* input_event, void* ctx) {
    furi_assert(ctx);
    FuriMessageQueue* event_queue = ctx;
    furi_message_queue_put(event_queue, input_event, FuriWaitForever);
}

void fill_tx_buff_by_pattern() {
    uint16_t page_size = chip_to_page_size(i2ctools->chip);
    i2ctools->tx_len = page_size;

    // 24C00 (page_size == 1)
    if(page_size == 1)
        i2ctools->tx_buff[0] = i2ctools->pattern;
    // All other chips
    else {
        for(uint16_t byte = 0; byte < page_size / 2; byte++) {
            // Static part
            i2ctools->tx_buff[byte] = byte;
            // Dynamic part
            i2ctools->tx_buff[(page_size / 2) + byte] = i2ctools->pattern + byte;
        }
    }
}

bool buffers_are_equal() {
    if(i2ctools->tx_len != i2ctools->rx_len)
        return false;
    if(!i2ctools->tx_len)
        return false;

    for(uint16_t byte = 0; byte < i2ctools->tx_len; byte++) {
        if(i2ctools->tx_buff[byte] != i2ctools->rx_buff[byte])
            return false;
    }

    return true;
}

int32_t i2ctools_app(void* p) {
    UNUSED(p);
    FuriMessageQueue* event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    // Alloc i2ctools
    i2ctools = malloc(sizeof(i2cTools));
    memset(i2ctools, 0, sizeof(i2cTools));

    i2ctools->mutex = furi_mutex_alloc(FuriMutexTypeNormal);
    i2ctools->notification = furi_record_open(RECORD_NOTIFICATION);
    i2ctools->chip = chip_24c64;
    i2ctools->test_page = 1;
    i2ctools->address_num = 0;
    i2ctools->address_idx = 0;
    i2ctools->scanned = false;

    // Alloc viewport
    i2ctools->view_port = view_port_alloc();
    view_port_draw_callback_set(i2ctools->view_port, i2ctools_draw_callback, i2ctools);
    view_port_input_callback_set(i2ctools->view_port, i2ctools_input_callback, event_queue);

    // Register view port in GUI
    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, i2ctools->view_port, GuiLayerFullscreen);

    InputEvent event;

    i2ctools->main_view = i2c_main_view_alloc();
    i2ctools->read = i2c_read_alloc();
    i2ctools->write = i2c_write_alloc();

    fill_tx_buff_by_pattern();

    while(furi_message_queue_get(event_queue, &event, FuriWaitForever) == FuriStatusOk) {
        // Back
        if(event.key == InputKeyBack && event.type == InputTypeRelease) {
            switch(i2ctools->main_view->current_view) {
            case KILL_VIEW:
                i2c_test_stop();
                notification_message(i2ctools->notification, &sequence_reset_rgb);
                break;
            default:
                break;
            }

            if(i2ctools->main_view->current_view == MAIN_VIEW)
                break;
            else
                i2ctools->main_view->current_view = MAIN_VIEW;
        }

        // Up
        else if(event.key == InputKeyUp && event.type == InputTypeRelease) {
            switch(i2ctools->main_view->current_view)
            {
            case MAIN_VIEW:
                if((i2ctools->main_view->menu_index > 0))
                    i2ctools->main_view->menu_index--;
                break;
            case SETTINGS_VIEW:
                i2ctools->chip = next_chip(i2ctools->chip);
                if(i2ctools->test_page >= chip_to_page_num_per_slave(i2ctools->chip))
                    i2ctools->test_page = chip_to_page_num_per_slave(i2ctools->chip) - 1;
                fill_tx_buff_by_pattern();
                break;
            default:
                break;
            }
        }

        // Long Up
        else if(
            event.key == InputKeyUp &&
            (event.type == InputTypeLong || event.type == InputTypeRepeat)) {
        }

        // Down
        else if(event.key == InputKeyDown && event.type == InputTypeRelease) {
            switch(i2ctools->main_view->current_view)
            {
            case MAIN_VIEW:
                if(i2ctools->main_view->menu_index < MENU_SIZE - 1)
                    i2ctools->main_view->menu_index++;
                break;
            case SETTINGS_VIEW:
                i2ctools->chip = prev_chip(i2ctools->chip);
                if(i2ctools->test_page >= chip_to_page_num_per_slave(i2ctools->chip))
                    i2ctools->test_page = chip_to_page_num_per_slave(i2ctools->chip) - 1;
                fill_tx_buff_by_pattern();
                break;
            default:
                break;
            }
        }

        // Long Down
        else if(
            event.key == InputKeyDown &&
            (event.type == InputTypeLong || event.type == InputTypeRepeat)) {
        // Ok
        } else if(event.key == InputKeyOk && event.type == InputTypeRelease) {
            switch(i2ctools->main_view->current_view)
            {
            case MAIN_VIEW:
                i2ctools->main_view->current_view = i2ctools->main_view->menu_index;
                break;
            case READ_VIEW:
                i2c_read(i2ctools->read);
                if(i2ctools->read->readed)
                    notification_message(i2ctools->notification, &sequence_blink_yellow_100);
                else
                    notification_message(i2ctools->notification, &sequence_blink_red_100);
                break;
            case WRITE_VIEW:
                i2c_write(i2ctools->write);
                if(i2ctools->write->written)
                    notification_message(i2ctools->notification, &sequence_blink_cyan_100);
                else
                    notification_message(i2ctools->notification, &sequence_blink_red_100);

                i2ctools->pattern++;
                fill_tx_buff_by_pattern();
                break;
            case KILL_VIEW:
                if(i2ctools->test_running)
                    i2c_test_stop();
                else
                    i2c_test_start();
                break;
            default:
                break;
            }
        } else if(event.key == InputKeyRight && event.type == InputTypeRelease) {
            switch(i2ctools->main_view->current_view)
            {
            case WRITE_VIEW:
                i2ctools->pattern++;
                fill_tx_buff_by_pattern();
                break;
            case SETTINGS_VIEW:
                i2ctools->test_page++;
                if(i2ctools->test_page >= chip_to_page_num_per_slave(i2ctools->chip))
                    i2ctools->test_page = chip_to_page_num_per_slave(i2ctools->chip) - 1;
                break;
            default:
                break;
            }
        } else if(event.key == InputKeyLeft && event.type == InputTypeRelease) {
            switch(i2ctools->main_view->current_view)
            {
            case WRITE_VIEW:
                i2ctools->pattern--;
                fill_tx_buff_by_pattern();
                break;
            case SETTINGS_VIEW:
                if(i2ctools->test_page)
                    i2ctools->test_page--;
                break;
            default:
                break;
            }
        }
        view_port_update(i2ctools->view_port);
    }

    gui_remove_view_port(gui, i2ctools->view_port);
    view_port_free(i2ctools->view_port);
    furi_message_queue_free(event_queue);
    i2c_read_free(i2ctools->read);
    i2c_write_free(i2ctools->write);
    i2c_main_view_free(i2ctools->main_view);
    free(i2ctools);
    furi_record_close(RECORD_NOTIFICATION);
    furi_record_close(RECORD_GUI);
    return 0;
}
