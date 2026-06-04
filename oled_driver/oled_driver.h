#ifndef OLED_DRIVER_H
#define OLED_DRIVER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define OLED_OK 0
#define OLED_ERROR_BAD_ADDRESS -1
#define OLED_ERROR_TIMEOUT -2
#define OLED_ERROR_NULL_FRAMEBUFFER -3
#define OLED_ERROR_OUT_OF_BOUNDS -4

typedef struct {
    i2c_inst_t* i2c;
    uint8_t addr;
    uint8_t sda_pin;
    uint8_t scl_pin;
    uint16_t width;
    uint16_t height;
    uint8_t pages;
    uint16_t framebuf_size;
    uint8_t *framebuff;
} oled_t;

int oled_init(oled_t *const screen, i2c_inst_t* i2c, uint8_t address, uint8_t sda, uint8_t scl, uint16_t width, uint16_t height);
int oled_update(oled_t *const screen);
void oled_clear(oled_t *const screen);
int oled_draw_pixel(oled_t *const screen, uint16_t x, uint16_t y, bool on);
#endif