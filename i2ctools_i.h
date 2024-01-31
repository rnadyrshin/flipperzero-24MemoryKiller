#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <input/input.h>
#include <notification/notification_messages.h>
#include "chip.h"
#include "i2cscanner.h"
#include "i2cread.h"
#include "i2cwrite.h"
#include "i2csender.h"
#include "views/main_view.h"
#include "views/scanner_view.h"
#include "views/read_view.h"
#include "views/write_view.h"
#include "views/sender_view.h"
#include "views/settings_view.h"

extern FuriHalI2cBusHandle i2c_handle;

// I2C BUS
#define I2C_TIMEOUT     3
#define I2C_BUS         &i2c_handle

// App datas
typedef struct {
    FuriMutex* mutex;
    ViewPort* view_port;
    NotificationApp* notification;

    i2cMainView* main_view;
    i2cRead* read;
    i2cWrite* write;
    i2cSender* sender;

    bool scanned;
    chip_model chip;
    uint8_t addresses[8];
    uint8_t address_num;
    uint8_t address_idx;
    uint16_t page_size;
    uint16_t test_page;
    uint8_t pattern;
    uint8_t tx_buff[CHIP_MAX_PAGE_SIZE];
    uint8_t rx_buff[CHIP_MAX_PAGE_SIZE];
    uint16_t tx_len;
    uint16_t rx_len;
} i2cTools;

extern i2cTools* i2ctools;