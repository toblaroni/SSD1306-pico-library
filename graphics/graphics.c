
#include "graphics.h"
#include <string.h>

void graphics_init(graphics_t *const graphics, uint8_t *framebuff, uint16_t width, uint16_t height) {
    graphics->framebuff = framebuff;
    graphics->width = width;
    graphics->height = height;
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

    return GRPAPHICS_OK;
}

