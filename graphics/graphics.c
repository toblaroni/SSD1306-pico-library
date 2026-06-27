/*
 * Basic 2D Graphics functions.
 */

#include "graphics.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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


int graphics_draw_line(graphics_t *const gfx, uint16_t _x0, uint16_t _y0, uint16_t x1, uint16_t y1) {
    if (_x0 >= gfx->width || x1 >= gfx->width || _y0 >= gfx->height || y1 >= gfx->height) {
        printf(
            "Graphics Error: Line coordinates out of bounds. x0: %u, y0: %u, x1: %u, y1: %u\n", 
            _x0, _y0, x1, y1
        );
        return GRAPHICS_ERROR_OUT_OF_BOUNDS;
    }
   
    // Remove unisigned integers to avoid wraparound
    int x0 = (int)_x0;
    int y0 = (int)_y0;
   
    if (x0 == x1) {
        return draw_vertical_line(gfx, x0, y0, y1);
    } else if (y0 == y1) {
        return draw_horizontal_line(gfx, x0, y0, x1);
    }

    // Bresenham's line algorithm
    int dx = abs((int)x1 - (int)x0);
    int dy = abs((int)y1 - (int)y0);
    // Negative or positive gradient
    int step_x = (x1 > x0) ? 1 : -1;
    int step_y = (y1 > y0) ? 1 : -1;
    bool x_major = dx >= dy;
    int error = x_major ? 2*dy - dx : 2*dx - dy;

    while (1) {
        // printf("Drawing pixel at (%u, %u)\n", x0, y0);
        graphics_draw_pixel(gfx, x0, y0, true);
        if (x0 == x1 && y0 == y1) break;
        if (x_major) {
            if (error > 0) {
                y0 += step_y;
                error -= 2*dx;
            }
            x0 += step_x;
            error += 2*dy;
        } else {
            if (error > 0) {
                x0 += step_x;
                error -= 2*dy;
            }
            y0 += step_y;
            error += 2*dx;
        }
    }

    return GRAPHICS_OK;
}

int draw_rectangle(graphics_t *const gfx, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    if (gfx->fill_on) {
        for (uint16_t y = y0; y <= y1; ++y)
            graphics_draw_line(gfx, x0, y, x1, y);
    } else {
        graphics_draw_line(gfx, x0, y0, x1, y0); // Top edge
        graphics_draw_line(gfx, x1, y0, x1, y1); // Right edge
        graphics_draw_line(gfx, x1, y1, x0, y1); // Bottom edge
        graphics_draw_line(gfx, x0, y1, x0, y0); // Left edge
    }
    return GRAPHICS_OK;
}

int graphics_draw_circle(graphics_t *const gfx, uint16_t x0, uint16_t y0, uint16_t radius) {
    if (x0 >= gfx->width || y0 >= gfx->height) {
        printf(
            "Graphics Error: Circle coordinates out of bounds. x0: %u, y0: %u, radius: %u\n", 
            x0, y0, radius
        );
        return GRAPHICS_ERROR_OUT_OF_BOUNDS;
    }

    // Midpoint circle algorithm
    // https://www.geeksforgeeks.org/dsa/mid-point-circle-drawing-algorithm/

    int x = (int)radius, y = 0;

    // P is the equation of the circle;
    int P = 1-radius;
    while (x >= y) {
        // Draw the points in all octants
        graphics_draw_pixel(gfx, x0 + x, y0 + y, true);
        graphics_draw_pixel(gfx, x0 - x, y0 + y, true);
        graphics_draw_pixel(gfx, x0 + x, y0 - y, true);
        graphics_draw_pixel(gfx, x0 - x, y0 - y, true);
        if (x != y) {
            graphics_draw_pixel(gfx, x0 + y, y0 + x, true);
            graphics_draw_pixel(gfx, x0 - y, y0 + x, true);
            graphics_draw_pixel(gfx, x0 + y, y0 - x, true);
            graphics_draw_pixel(gfx, x0 - y, y0 - x, true);
        }

        y++;

        // Mid point inside / on perimeter
        if (P <= 0) 
            P += 2*y + 1;
        else {  // Outside perim
            x--;
            P += 2*y - 2*x + 1;
        }

    }
    
    return GRAPHICS_OK;
}

int graphics_draw_triangle(graphics_t *const gfx, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    // Draw triangle
    return GRAPHICS_OK;
}
