/* Graphics library for OLED displays. Treats each pixel as a bit in a framebuffer ordered by rows */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <pico/stdlib.h>

#define GRAPHICS_OK 0
#define GRAPHICS_ERROR_OUT_OF_BOUNDS -1

typedef struct {
    uint8_t *framebuff;
    uint16_t width;
    uint16_t height;
    bool stroke_on;
    bool fill_on;
} graphics_t;

void graphics_init(graphics_t *const graphics, uint8_t *framebuff, uint16_t width, uint16_t height);
void graphics_clear(graphics_t *const graphics);
int graphics_draw_pixel(graphics_t *const graphics, uint16_t x, uint16_t y, bool on);
int graphics_draw_line(graphics_t *const graphics, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
int graphics_draw_rectangle(graphics_t *const graphics, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
int graphics_draw_circle(graphics_t *const graphics, uint16_t x0, uint16_t y0, uint16_t radius);
int graphics_draw_triangle(graphics_t *const graphics, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
#endif