#include <i2ctools_i.h>
#include "chip.h"

static const char *chip_24c00_str = "24C00";
static const char *chip_24c01_str = "24C01";
static const char *chip_24c02_str = "24C02";
static const char *chip_24c04_str = "24C04";
static const char *chip_24c08_str = "24C08";
static const char *chip_24c16_str = "24C16";
static const char *chip_24c32_str = "24C32";
static const char *chip_24c64_str = "24C64";
static const char *chip_24c128_str = "24C128";
static const char *chip_24c256_str = "24C256";
static const char *chip_24c512_str = "24C512";
static const char *chip_24c1024_str = "24C1024";

chip_model prev_chip(chip_model current_chip){
    if(current_chip != CHIP_FIRST)
        current_chip--;

    return current_chip;
}

chip_model next_chip(chip_model current_chip){
    if(current_chip != CHIP_LAST)
        current_chip++;

    return current_chip;
}

chip_model str_to_chip(const char *str){
    if (!strcmp(str, chip_24c00_str)) return chip_24c00;
    if (!strcmp(str, chip_24c01_str)) return chip_24c01;
    if (!strcmp(str, chip_24c02_str)) return chip_24c02;
    if (!strcmp(str, chip_24c04_str)) return chip_24c04;
    if (!strcmp(str, chip_24c08_str)) return chip_24c08;
    if (!strcmp(str, chip_24c16_str)) return chip_24c16;
    if (!strcmp(str, chip_24c32_str)) return chip_24c32;
    if (!strcmp(str, chip_24c64_str)) return chip_24c64;
    if (!strcmp(str, chip_24c128_str)) return chip_24c128;
    if (!strcmp(str, chip_24c256_str)) return chip_24c256;
    if (!strcmp(str, chip_24c512_str)) return chip_24c512;
    if (!strcmp(str, chip_24c1024_str)) return chip_24c1024;
    return chip_24c64;
}

const char *chip_to_str(chip_model chip){
    switch (chip)
    {
    case chip_24c00: return chip_24c00_str;
    case chip_24c01: return chip_24c01_str;
    case chip_24c02: return chip_24c02_str;
    case chip_24c04: return chip_24c04_str;
    case chip_24c08: return chip_24c08_str;
    case chip_24c16: return chip_24c16_str;
    case chip_24c32: return chip_24c32_str;
    case chip_24c64: return chip_24c64_str;
    case chip_24c128: return chip_24c128_str;
    case chip_24c256: return chip_24c256_str;
    case chip_24c512: return chip_24c512_str;
    case chip_24c1024: return chip_24c1024_str;
    default: return chip_to_str(chip_24c64);
    }
}

uint16_t chip_to_page_size(chip_model chip){
    switch (chip)
    {
    case chip_24c00: return 1;
    case chip_24c01: return 8;
    case chip_24c02: return 8;
    case chip_24c04: return 16;
    case chip_24c08: return 16;
    case chip_24c16: return 16;
    case chip_24c32: return 32;
    case chip_24c64: return 32;
    case chip_24c128: return 64;
    case chip_24c256: return 64;
    case chip_24c512: return 128;
    case chip_24c1024: return 256;
    default: return chip_to_page_size(chip_24c64);
    }
}

uint16_t chip_to_page_num_per_slave(chip_model chip){
    switch (chip)
    {
    case chip_24c00: return 16;
    case chip_24c01: return 16;
    case chip_24c02: return 32;
    case chip_24c04: return 32;
    case chip_24c08: return 64;
    case chip_24c16: return 128;
    case chip_24c32: return 128;
    case chip_24c64: return 256;
    case chip_24c128: return 256;
    case chip_24c256: return 512;
    case chip_24c512: return 512;
    case chip_24c1024: return 256;
    default: return chip_to_page_num_per_slave(chip_24c64);
    }
}

uint8_t chip_to_addr_size(chip_model chip){
    switch (chip)
    {
    case chip_24c00: return 1;
    case chip_24c01: return 1;
    case chip_24c02: return 1;
    case chip_24c04: return 1; // 2 dev
    case chip_24c08: return 1; // 4 dev
    case chip_24c16: return 1; // 8 dev
    case chip_24c32: return 2;
    case chip_24c64: return 2;
    case chip_24c128: return 2;
    case chip_24c256: return 2;
    case chip_24c512: return 2;
    case chip_24c1024: return 2; // 2 dev
    default: return chip_to_addr_size(chip_24c64);
    }
}

uint8_t chip_to_slave_num(chip_model chip){
    switch (chip)
    {
    case chip_24c00: return 1;
    case chip_24c01: return 1;
    case chip_24c02: return 1;
    case chip_24c04: return 2;
    case chip_24c08: return 4;
    case chip_24c16: return 8;
    case chip_24c32: return 1;
    case chip_24c64: return 1;
    case chip_24c128: return 1;
    case chip_24c256: return 1;
    case chip_24c512: return 1;
    case chip_24c1024: return 2;
    default: return chip_to_slave_num(chip_24c64);
    }
}
