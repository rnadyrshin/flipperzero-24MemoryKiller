#pragma once

#include <furi.h>
#include <furi_hal.h>

typedef enum {
	chip_24c00 = 0, // 128 bit      1d16p1
	chip_24c01,     // 1 Kbit       1d16p8
	chip_24c02,     // 2 Kbit       1d32p8
	chip_24c04,     // 4 Kbit       2d32p16
	chip_24c08,     // 8 Kbit       4d64p16
	chip_24c16,     // 16 Kbit      8d128p16
	chip_24c32,     // 32 Kbit      1d128p32
	chip_24c64,     // 64 Kbit      1d256p32
	chip_24c128,    // 128 Kbit     1d256p64
	chip_24c256,    // 256 Kbit     1d512p64
	chip_24c512,    // 512 Kbit     1d512p128
	chip_24c1024,   // 1024 Kbit    2d256p256
} chip_model;

#define CHIP_FIRST          chip_24c00
#define CHIP_LAST           chip_24c1024
#define CHIP_MAX_PAGE_SIZE  256

chip_model prev_chip(chip_model current_chip);
chip_model next_chip(chip_model current_chip);
chip_model str_to_chip(const char *str);
const char *chip_to_str(chip_model chip);
uint16_t chip_to_page_size(chip_model chip);
uint16_t chip_to_page_num_per_slave(chip_model chip);
uint8_t chip_to_addr_size(chip_model chip);
uint8_t chip_to_slave_num(chip_model chip);
