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

