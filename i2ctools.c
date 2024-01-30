#include "i2ctools_i.h"

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
        draw_scanner_view(canvas, i2ctools->scanner);
        break;
    case KILL_VIEW:
        draw_sniffer_view(canvas, i2ctools->sniffer);
        break;
    case WRITE_VIEW:
        draw_sender_view(canvas, i2ctools->sender);
        break;
    case READ_VIEW:
        draw_read_view(canvas, i2ctools->read);
        break;
    case SETTINGS_VIEW:
        draw_infos_view(canvas);
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

int32_t i2ctools_app(void* p) {
    UNUSED(p);
    FuriMessageQueue* event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    // Alloc i2ctools
    i2cTools* i2ctools = malloc(sizeof(i2cTools));
    memset(i2ctools, 0, sizeof(i2cTools));

    i2ctools->mutex = furi_mutex_alloc(FuriMutexTypeNormal);
    i2ctools->notification = furi_record_open(RECORD_NOTIFICATION);

    i2ctools->page_size = 32;
    i2ctools->test_page = 1;

    // Alloc viewport
    i2ctools->view_port = view_port_alloc();
    view_port_draw_callback_set(i2ctools->view_port, i2ctools_draw_callback, i2ctools);
    view_port_input_callback_set(i2ctools->view_port, i2ctools_input_callback, event_queue);

    // Register view port in GUI
    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, i2ctools->view_port, GuiLayerFullscreen);

    InputEvent event;

    i2ctools->main_view = i2c_main_view_alloc();

    i2ctools->sniffer = i2c_sniffer_alloc();
    i2ctools->sniffer->menu_index = 0;

    i2ctools->scanner = i2c_scanner_alloc();

    i2ctools->read = i2c_read_alloc();
    i2ctools->read->scanner = i2ctools->scanner;
    i2ctools->read->addr = i2ctools->test_page * i2ctools->page_size;

    i2ctools->sender = i2c_sender_alloc();
    i2ctools->sender->scanner = i2ctools->scanner;

    while(furi_message_queue_get(event_queue, &event, FuriWaitForever) == FuriStatusOk) {
        // Back
        if(event.key == InputKeyBack && event.type == InputTypeRelease) {
            if(i2ctools->main_view->current_view == MAIN_VIEW) {
                break;
            } else {
                i2ctools->main_view->current_view = MAIN_VIEW;
            }
        }

        // Up
        else if(event.key == InputKeyUp && event.type == InputTypeRelease) {
            switch(i2ctools->main_view->current_view)
            {
            case MAIN_VIEW:
                if((i2ctools->main_view->menu_index > 0))
                    i2ctools->main_view->menu_index--;
                break;
            case READ_VIEW:
                if(i2ctools->read->address_idx + 1 < i2ctools->read->address_num)
                    i2ctools->read->address_idx++;
                break;
            default:
                break;
            }

/*
            else if(i2ctools->main_view->current_view == SCAN_VIEW) {
                if(i2ctools->scanner->menu_index > 0) {
                    i2ctools->scanner->menu_index--;
                }
            }
            else if(i2ctools->main_view->current_view == SNIFF_VIEW) {
                if(i2ctools->sniffer->row_index > 0) {
                    i2ctools->sniffer->row_index--;
                }
            }
            else if(i2ctools->main_view->current_view == SEND_VIEW) {
                if(i2ctools->sender->value < 0xFF) {
                    i2ctools->sender->value++;
                    i2ctools->sender->sended = false;
                }
            }*/
        }

        // Long Up
        else if(
            event.key == InputKeyUp &&
            (event.type == InputTypeLong || event.type == InputTypeRepeat)) {
/*
            if(i2ctools->main_view->current_view == SCAN_VIEW) {
                if(i2ctools->scanner->menu_index > 5) {
                    i2ctools->scanner->menu_index -= 5;
                }
            }
            else if(i2ctools->main_view->current_view == SEND_VIEW) {
                if(i2ctools->sender->value < 0xF9) {
                    i2ctools->sender->value += 5;
                    i2ctools->sender->sended = false;
                }
            } else if(i2ctools->main_view->current_view == SNIFF_VIEW) {
                if(i2ctools->sniffer->row_index > 5) {
                    i2ctools->sniffer->row_index -= 5;
                } else {
                    i2ctools->sniffer->row_index = 0;
                }
            }*/
        }

        // Down
        else if(event.key == InputKeyDown && event.type == InputTypeRelease) {
            switch(i2ctools->main_view->current_view)
            {
            case MAIN_VIEW:
                if(i2ctools->main_view->menu_index < MENU_SIZE - 1)
                    i2ctools->main_view->menu_index++;
                break;
            case READ_VIEW:
                if(i2ctools->read->address_idx)
                    i2ctools->read->address_idx--;
                break;
            default:
                break;
            }


            /*else if(i2ctools->main_view->current_view == SCAN_VIEW) {
                if(i2ctools->scanner->menu_index < ((int)i2ctools->scanner->nb_found / 3)) {
                    i2ctools->scanner->menu_index++;
                }
            } else if(i2ctools->main_view->current_view == SNIFF_VIEW) {
                if((i2ctools->sniffer->row_index + 3) <
                   (int)i2ctools->sniffer->frames[i2ctools->sniffer->menu_index].data_index) {
                    i2ctools->sniffer->row_index++;
                }
            } else if(i2ctools->main_view->current_view == SEND_VIEW) {
                if(i2ctools->sender->value > 0x00) {
                    i2ctools->sender->value--;
                    i2ctools->sender->sended = false;
                }
            }*/
        }

        // Long Down
        else if(
            event.key == InputKeyDown &&
            (event.type == InputTypeLong || event.type == InputTypeRepeat)) {
/*
            if(i2ctools->main_view->current_view == SEND_VIEW) {
                if(i2ctools->sender->value > 0x05) {
                    i2ctools->sender->value -= 5;
                    i2ctools->sender->sended = false;
                } else {
                    i2ctools->sender->value = 0;
                    i2ctools->sender->sended = false;
                }
            } else if(i2ctools->main_view->current_view == SNIFF_VIEW) {
                if((i2ctools->sniffer->row_index + 8) <
                   (int)i2ctools->sniffer->frames[i2ctools->sniffer->menu_index].data_index) {
                    i2ctools->sniffer->row_index += 5;
                }
            }
*/
        // Ok
        } else if(event.key == InputKeyOk && event.type == InputTypeRelease) {
            switch(i2ctools->main_view->current_view)
            {
            case MAIN_VIEW:
                i2ctools->main_view->current_view = i2ctools->main_view->menu_index;
                break;
            case READ_VIEW:
                i2ctools->read->len = i2ctools->page_size;
                i2c_read(i2ctools->read);
                break;
            default:
                break;
            }

/*
            else if(i2ctools->main_view->current_view == SEND_VIEW) {
                i2ctools->sender->must_send = true;
            } else if(i2ctools->main_view->current_view == SNIFF_VIEW) {
                if(i2ctools->sniffer->started) {
                    stop_interrupts();
                    i2ctools->sniffer->started = false;
                    i2ctools->sniffer->state = I2C_BUS_FREE;
                } else {
                    start_interrupts(i2ctools->sniffer);
                    i2ctools->sniffer->started = true;
                    i2ctools->sniffer->state = I2C_BUS_FREE;
                }
            }
*/
        } else if(event.key == InputKeyRight && event.type == InputTypeRelease) {
            switch(i2ctools->main_view->current_view)
            {
            case READ_VIEW:
                i2ctools->read->addr += i2ctools->page_size;
                i2ctools->read->readed = false;
                break;
            default:
                break;
            }
/*
            if(i2ctools->main_view->current_view == SEND_VIEW) {
                if(i2ctools->sender->address_idx < (i2ctools->scanner->nb_found - 1)) {
                    i2ctools->sender->address_idx++;
                    i2ctools->sender->sended = false;
                }
            } else if(i2ctools->main_view->current_view == SNIFF_VIEW) {
                if(i2ctools->sniffer->menu_index < i2ctools->sniffer->frame_index) {
                    i2ctools->sniffer->menu_index++;
                    i2ctools->sniffer->row_index = 0;
                }
            }
*/
        } else if(event.key == InputKeyLeft && event.type == InputTypeRelease) {
            switch(i2ctools->main_view->current_view)
            {
            case READ_VIEW:
                if(i2ctools->read->addr) {
                    i2ctools->read->addr -= i2ctools->page_size;
                    i2ctools->read->readed = false;
                }
                break;
            default:
                break;
            }
/*
            if(i2ctools->main_view->current_view == SEND_VIEW) {
                if(i2ctools->sender->address_idx > 0) {
                    i2ctools->sender->address_idx--;
                    i2ctools->sender->sended = false;
                }
            } else if(i2ctools->main_view->current_view == SNIFF_VIEW) {
                if(i2ctools->sniffer->menu_index > 0) {
                    i2ctools->sniffer->menu_index--;
                    i2ctools->sniffer->row_index = 0;
                }
            }
*/
        }
        view_port_update(i2ctools->view_port);
    }

    gui_remove_view_port(gui, i2ctools->view_port);
    view_port_free(i2ctools->view_port);
    furi_record_close(RECORD_NOTIFICATION);
    furi_message_queue_free(event_queue);
    i2c_sniffer_free(i2ctools->sniffer);
    i2c_scanner_free(i2ctools->scanner);
    i2c_read_free(i2ctools->read);
    i2c_sender_free(i2ctools->sender);
    i2c_main_view_free(i2ctools->main_view);
    free(i2ctools);
    furi_record_close(RECORD_GUI);
    return 0;
}
