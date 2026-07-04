/*
 * Basic 2D Graphics functions.
 */

#include "graphics.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
} vertex_t;

void graphics_init(graphics_t *const graphics, uint8_t *framebuff, uint16_t width, uint16_t height) {
    graphics->framebuff = framebuff;
    graphics->width = width;
    graphics->height = height;
    graphics->fill_on = true;
    graphics->fill_colour = GRAPHICS_COLOUR_WHITE;
    graphics->stroke_on = true;
    graphics->stroke_colour = GRAPHICS_COLOUR_WHITE;
}


void graphics_clear(graphics_t *const gfx) {
    memset(gfx->framebuff, 0x00, gfx->width * gfx->height / 8);
    return;
}

void graphics_no_fill(graphics_t *const gfx) {
    gfx->fill_on = false;
}

void graphics_fill(graphics_t *const gfx, graphics_colour_t colour) {
    gfx->fill_colour = colour;
    gfx->fill_on = true;
}

void graphics_no_stroke(graphics_t *const gfx) {
    gfx->stroke_on = false;
}

void graphics_stroke(graphics_t *const gfx, graphics_colour_t colour) {
    gfx->stroke_colour = colour;
    gfx->stroke_on = true;
}

int graphics_draw_pixel(graphics_t *const gfx, int x, int y, bool on) {
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

static void swap_int(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}


// Faster than calling draw_pixel since the value of y doesn't change.
static void draw_horizontal_line(graphics_t *const gfx, int x0, int y, int x1, graphics_colour_t colour) {
    if (y < 0 || y >= gfx->height) {
        printf(
            "Graphics Error: Horizontal line out of bounds. x0: %i, x1: %i, y: %i\n", 
            x0, x1, y
        );
        return;
    }

    if (x0 > x1)
        swap_int(&x0, &x1);
    
    if (x1 < 0 || x0 >= gfx->width) {
        printf(
            "Graphics Error: Horizontal line out of bounds. x0: %i, x1: %i, y: %i\n", 
            x0, x1, y
        );
        return;
    }
    
    // Clipping
    if (x0 < 0)
        x0 = 0;
    if (x1 >= gfx->width)
        x1 = gfx->width - 1;       

    uint8_t bit_pos = y % 8;
    uint8_t page = y / 8;
    int byte_index = gfx->width * page + x0;

    for (int i = 0; i < x1 - x0; ++i) {
        gfx->framebuff[byte_index + i] |= (1 << bit_pos);       
    }
}

static void draw_vertical_line(graphics_t *const gfx, int x, int y0, int y1, graphics_colour_t colour) {
    if (y0 > y1) {
        swap_int(&y0, &y1);
    }

    for (uint16_t y = y0; y <= y1; ++y) {
        graphics_draw_pixel(gfx, x, y, colour);
    }
}


int graphics_draw_line(graphics_t *const gfx, int _x0, int _y0, int x1, int y1) {
    if (!gfx->stroke_on) return GRAPHICS_OK;     // Don't draw a line!?
   
    // Remove unisigned integers to avoid wraparound
    int x0 = (int)_x0;
    int y0 = (int)_y0;
   
    if (x0 == x1) {
        draw_vertical_line(gfx, x0, y0, y1, gfx->stroke_colour);
        return GRAPHICS_OK;
    } else if (y0 == y1) {
        draw_horizontal_line(gfx, x0, y0, x1, gfx->stroke_colour);
        return GRAPHICS_OK;
    }

    // Bresenham's line algorithm
    int dx = abs((int)x1 - (int)x0);
    int dy = abs((int)y1 - (int)y0);
    // Negative or positive gradient
    int step_x = (x1 > x0) ? 1 : -1;
    int step_y = (y1 > y0) ? 1 : -1;
    bool x_major = dx >= dy;
    int error = x_major ? 2 * dy - dx : 2 * dx - dy;

    while (1) {
        // printf("Drawing pixel at (%u, %u)\n", x0, y0);
        graphics_draw_pixel(gfx, x0, y0, gfx->stroke_colour);
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

int graphics_draw_rectangle(graphics_t *const gfx, int x0, int y0, uint16_t width, uint16_t height) {
    if (gfx->fill_on) {
        for (int y = y0; y <= y0 + height; ++y) {
            draw_horizontal_line(gfx, x0, y, x0 + width, gfx->fill_colour);
        }
    }  
    
    if (gfx->stroke_on) {
        draw_horizontal_line(gfx, x0, y0, x0 + width, gfx->stroke_colour);
        draw_horizontal_line(gfx, x0, y0 + height, x0 + width, gfx->stroke_colour);
        draw_vertical_line(gfx, x0, y0, y0 + height, gfx->stroke_colour);
        draw_vertical_line(gfx, x0 + width, y0, y0 + height, gfx->stroke_colour);
    }

    return GRAPHICS_OK;
}

static void plot_circle_scanline(graphics_t *const gfx, int x0, int y0, int x, int y) {
    draw_horizontal_line(gfx, x0 - x, y0 + y, x0 + x, gfx->fill_colour);
    draw_horizontal_line(gfx, x0 - x, y0 - y, x0 + x, gfx->fill_colour);
    if (x != y) {
        draw_horizontal_line(gfx, x0 - y, y0 + x, x0 + y, gfx->fill_colour);
        draw_horizontal_line(gfx, x0 - y, y0 - x, x0 + y, gfx->fill_colour);
    }
}

static void plot_circle_points(graphics_t *const gfx, int x0, int y0, int x, int y) {
    graphics_draw_pixel(gfx, x0 + x, y0 + y, gfx->stroke_colour);
    graphics_draw_pixel(gfx, x0 - x, y0 + y, gfx->stroke_colour);
    graphics_draw_pixel(gfx, x0 + x, y0 - y, gfx->stroke_colour);
    graphics_draw_pixel(gfx, x0 - x, y0 - y, gfx->stroke_colour);
    if (x != y) {
        graphics_draw_pixel(gfx, x0 + y, y0 + x, gfx->stroke_colour);
        graphics_draw_pixel(gfx, x0 - y, y0 + x, gfx->stroke_colour);
        graphics_draw_pixel(gfx, x0 + y, y0 - x, gfx->stroke_colour);
        graphics_draw_pixel(gfx, x0 - y, y0 - x, gfx->stroke_colour);
    }

}

int graphics_draw_circle(graphics_t *const gfx, int x0, int y0, uint16_t radius) {
    if (!gfx->fill_on && !gfx->stroke_on) return GRAPHICS_OK;

    // Midpoint circle algorithm
    // https://www.geeksforgeeks.org/dsa/mid-point-circle-drawing-algorithm/

    int x = (int)radius, y = 0;

    // P is the equation of the circle;
    int P = 1-radius;
    while (x >= y) {
        // Draw the points in all octants
        if (gfx->fill_on)
            plot_circle_scanline(gfx, x0, y0, x, y);

        if (gfx->stroke_on)
            plot_circle_points(gfx, x0, y0, x, y);

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

int graphics_draw_ellipse(graphics_t *const gfx, int x0, int y0, uint16_t radius_x, int16_t radius_y) {
    if (!gfx->fill_on && !gfx->stroke_on) 
        return GRAPHICS_OK;

    

    return GRAPHICS_OK;
}

void swap_vertex(vertex_t *v0, vertex_t *v1) {
    vertex_t tmp = *v0;
    *v0 = *v1;
    *v1 = tmp;
    return;
}

void sort_tri_vertices_by_y(vertex_t v0, vertex_t v1,  vertex_t v2) {
    if (v0.y > v1.y)
        swap_vertex(&v0, &v1);

    if (v1.y > v2.y)
        swap_vertex(&v1, &v2);

    if (v0.y > v1.y) {
        swap_vertex(&v0, &v1);
    }

    return;
}

void fill_bottom_flat_triangle(graphics_t *const gfx, int x0, int y0, int x1, int y1, int x2, int y2) {

}

// https://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
int graphics_draw_triangle(graphics_t *const gfx, int x0, int y0, int x1, int y1, int x2, int y2) {
    vertex_t v0 = {x0, y0};
    vertex_t v1 = {x1, y1};
    vertex_t v2 = {x2, y2};
    sort_tri_vertices_by_y(v0, v1, v2);

    if (gfx->fill_on) {
        
    }

    // Draw triangle
    if (gfx->stroke_on) {   // This is actually handled by draw line...
        graphics_draw_line(gfx, x0, y0, x1, y1);
        graphics_draw_line(gfx, x1, y1, x2, y2);
        graphics_draw_line(gfx, x2, y2, x0, y0);
    }

    return GRAPHICS_OK;
}
