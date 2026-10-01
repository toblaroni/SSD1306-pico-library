/* Graphics library for OLED displays. Treats each pixel as a bit in a framebuffer ordered by rows */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <pico/stdlib.h>

#define GRAPHICS_OK 0
#define GRAPHICS_ERROR_OUT_OF_BOUNDS -1
#define GRAPHICS_ERROR_INVALID_ARGUMENT -2
#define GRAPHICS_ERROR_NO_FONT -3

typedef enum {
    GRAPHICS_COLOUR_BLACK = 0,
    GRAPHICS_COLOUR_WHITE = 1
} graphics_colour_t;


typedef struct {
    int first_char;
    int last_char;
    int char_width;
    int char_height;
    int bytes_per_col;
    int char_spacing;
    int line_height;
    bool proportional;      // Skip empty columns in the bitmap (trim each glyph)
    const uint8_t *data;
} font_t;


typedef struct {
    uint8_t *framebuff;
    uint16_t width;
    uint16_t height;

    // Drawing
    bool fill_on;
    bool stroke_on;
    graphics_colour_t fill_colour;
    graphics_colour_t stroke_colour;

    font_t *font;
} graphics_t;

void graphics_init(graphics_t *const graphics, uint8_t *framebuff, uint16_t width, uint16_t height);

void graphics_clear(graphics_t *const graphics);
int graphics_draw_pixel(graphics_t *const graphics, int x, int y, bool on);
int graphics_draw_line(graphics_t *const graphics, int x0, int y0, int x1, int y1);
int graphics_draw_rectangle(graphics_t *const graphics, int x0, int y0, int w, int h);
int graphics_draw_circle(graphics_t *const graphics, int x0, int y0, int radius);
int graphics_draw_ellipse(graphics_t *const graphics, int x0, int y0, int radius_x, int radius_y);
int graphics_draw_triangle(graphics_t *const graphics, int x0, int y0, int x1, int y1, int x2, int y2);

void graphics_no_fill(graphics_t *const graphics);
void graphics_fill(graphics_t *const graphics, graphics_colour_t);
void graphics_no_stroke(graphics_t *const graphics);
void graphics_stroke(graphics_t *const graphics, graphics_colour_t);

void graphics_set_font(graphics_t *const graphics, font_t *const font);
void graphics_set_c_spacing(graphics_t *const graphics, int);
void graphics_set_line_h(graphics_t *const graphics, int);
void graphics_set_font_proportional(graphics_t *const graphics, bool);

int graphics_draw_text(graphics_t *const gfx, const char *c, size_t length, int x1, int y1, int x2, int y2);

// TODO Polygon
// int graphics_draw_poly();

#endif