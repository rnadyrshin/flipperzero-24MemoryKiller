#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <input/input.h>
#include <notification/notification_messages.h>
#include "i2csniffer.h"
#include "i2cscanner.h"
#include "i2cread.h"
#include "i2csender.h"
#include "views/main_view.h"
#include "views/sniffer_view.h"
#include "views/scanner_view.h"
#include "views/read_view.h"
#include "views/sender_view.h"
#include "views/infos_view.h"

extern FuriHalI2cBusHandle i2c_handle;

#define I2C_BUS &i2c_handle

// App datas
typedef struct {
    FuriMutex* mutex;
    ViewPort* view_port;
    NotificationApp* notification;
    i2cMainView* main_view;
    i2cScanner* scanner;
    i2cRead* read;
    i2cSniffer* sniffer;
    i2cSender* sender;

    uint16_t page_size;
    uint16_t test_page;
} i2cTools;

extern i2cTools* i2ctools;