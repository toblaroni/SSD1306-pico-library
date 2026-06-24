
#include "graphics.h"
#include <string.h>
#include <stdio.h>

void graphics_init(graphics_t *const graphics, uint8_t *framebuff, uint16_t width, uint16_t height) {
    graphics->framebuff = framebuff;
    graphics->width = width;
    graphics->height = height;
    graphics->stroke_on = true;
    graphics->fill_on = true;
}


void graphics_clear(graphics_t *const gfx) {
    memset(gfx->framebuff, 0x00, gfx->width * gfx->height / 8);
    return;
}


int graphics_draw_pixel(graphics_t *const gfx, uint16_t x, uint16_t y, bool on) {
    if (x >= gfx->width || y >= gfx->height)
        return GRAPHICS_ERROR_OUT_OF_BOUNDS;

    int page = y / 8;
    int bit_pos = y % 8;
    int byte_index = page * gfx->width + x;

    if (on)
        gfx->framebuff[byte_index] |= (1 << bit_pos);
    else
        gfx->framebuff[byte_index] &= ~(1 << bit_pos);

    return GRAPHICS_OK;
}

static void swap_uint16(uint16_t *a, uint16_t *b) {
    uint16_t temp = *a;
    *a = *b;
    *b = temp;
}

static int draw_horizontal_line(graphics_t *const gfx, uint16_t x0, uint16_t y, uint16_t x1) {
    if (x0 > x1) {
        swap_uint16(&x0, &x1);
    }

    for (uint16_t x = x0; x <= x1; ++x) {
        graphics_draw_pixel(gfx, x, y, true);
    }

    return GRAPHICS_OK;
}

static int draw_vertical_line(graphics_t *const gfx, uint16_t x, uint16_t y0, uint16_t y1) {
    if (y0 > y1) {
        swap_uint16(&y0, &y1);
    }

    for (uint16_t y = y0; y <= y1; ++y) {
        graphics_draw_pixel(gfx, x, y, true);
    }

    return GRAPHICS_OK;
}


int graphics_draw_line(graphics_t *const gfx, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    if (x0 >= gfx->width || x1 >= gfx->width || y0 >= gfx->height || y1 >= gfx->height) {
        printf(
            "Graphics Error: Line coordinates out of bounds. x0: %u, y0: %u, x1: %u, y1: %u\n", 
            x0, y0, x1, y1
        );
        return GRAPHICS_ERROR_OUT_OF_BOUNDS;
    }
   
    if (x0 == x1) {
        return draw_vertical_line(gfx, x0, y0, y1);
    } else if (y0 == y1) {
        return draw_horizontal_line(gfx, x0, y0, x1);
    }

    // Bresenham's line algorithm
    if (x0 > x1 || y0 > y1) {
        swap_uint16(&x0, &x1);
        swap_uint16(&y0, &y1);
    } 
    
    return GRAPHICS_OK;
}

int graphics_draw_circle(graphics_t *const gfx, uint16_t x0, uint16_t y0, uint16_t radius) {
    // Midpoint circle algorithm
    return GRAPHICS_OK;
}

int graphics_draw_triangle(graphics_t *const gfx, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    // Draw triangle
    return GRAPHICS_OK;
}
