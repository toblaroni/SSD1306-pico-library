#include "font.h"
#include "fonts/bitfont.h"

#include <stdint.h>
#include <stdio.h>

int graphics_draw_char(graphics_t *const gfx, char c, int x, int y) {
    if (c < FONT_FIRST_CHAR || c > FONT_LAST_CHAR) {
        return GRAPHICS_OK;
    }

    size_t glyph_index = c - FONT_FIRST_CHAR;
    size_t bytes_per_glyph = FONT_BYTES_PER_COLUMN * FONT_CHAR_WIDTH;
    const uint8_t *glyph = &font[glyph_index * bytes_per_glyph];
    
    for (int col = 0; col < FONT_CHAR_WIDTH; col++) {
        for (int row = 0; row < FONT_CHAR_HEIGHT; row++) {
            uint8_t byte = glyph[FONT_BYTES_PER_COLUMN * col + row/8]; 
            
            if (byte & (1 << (row % 8))) {
                graphics_draw_pixel(
                    gfx, 
                    x+col,
                    y+row,
                    gfx->fill_on
                );
            }
        }
    }

    return GRAPHICS_OK;
}

// graphics_draw_pixel() handles clipping if x1, x2, y1 or y2 are out of bounds.
int graphics_draw_text(
    graphics_t *const gfx, 
    const char *c, 
    size_t length, 
    int x1, 
    int y1, 
    int x2, 
    int y2
) {
    if (x2 < x1 || y2 < y1) {
        return GRAPHICS_INVALID_ARGUMENT;
    } 


    int x = x1;
    int y = y1;
    for (size_t i = 0; i < length; i++) {
        // Move the cursor along. Wrap if necessary.
        if (x + FONT_CHAR_WIDTH > x2) {
            x = x1;
            y += FONT_CHAR_HEIGHT + gfx->line_height;
        }
        if (y + FONT_CHAR_HEIGHT > y2) {
            break;  // Stop drawing when exceeds bounding box
        }

        // Draw each character
        graphics_draw_char(gfx, c[i], x, y);

        x += FONT_CHAR_WIDTH + gfx->char_spacing;

    }

    return GRAPHICS_OK;
}