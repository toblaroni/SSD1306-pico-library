#ifndef OLED_DRIVER_H
#define OLED_DRIVER_H
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define OLED_OK 0
#define OLED_ERROR_BAD_ADDRESS -1

typedef struct {
    i2c_inst_t* i2c;
    uint8_t addr;
    uint8_t sda_pin;
    uint8_t scl_pin;
} oled_t;


int oled_init(i2c_inst_t* i2c, uint8_t address, uint8_t sda, uint8_t scl);
void oled_update();
void oled_clear();
void oled_draw_pixel();
#endif