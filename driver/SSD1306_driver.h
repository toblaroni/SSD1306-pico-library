#ifndef SSD1306_DRIVER_H
#define SSD1306_DRIVER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define SSD1306_OK 0
#define SSD1306_ERROR_BAD_ADDRESS -1
#define SSD1306_ERROR_TIMEOUT -2
#define SSD1306_ERROR_NULL_FRAMEBUFFER -3
#define SSD1306_ERROR_OUT_OF_BOUNDS -4

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
} SSD1306_t;

int SSD1306_init(SSD1306_t *const screen, i2c_inst_t* i2c, uint8_t address, uint8_t sda, uint8_t scl, uint16_t width, uint16_t height);
int SSD1306_update(SSD1306_t *const screen);
void SSD1306_clear(SSD1306_t *const screen);
int SSD1306_draw_pixel(SSD1306_t *const screen, uint16_t x, uint16_t y, bool on);
#endif