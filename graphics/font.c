#include "font.h"
#include "fonts/bitfont.h"

#include <stdint.h>

int graphics_draw_char(graphics_t *const gfx, char c, int x, int y) {
    if (c < FONT_FIRST_CHAR || c > FONT_LAST_CHAR) {
        return GRAPHICS_OK;
    }
    // Get the index of the character
    size_t index = c - FONT_FIRST_CHAR;

    for (int col = 0; col < FONT_CHAR_WIDTH; ++col) {
        for (int row = 0; row < FONT_CHAR_HEIGHT; row++) {
            size_t byte = font[index + (row * FONT_CHAR_WIDTH + col)];
            for (int pixel_index = 0; pixel_index < 8; pixel_index ++) {
                if (byte & (1 << pixel_index)) {
                    graphics_draw_pixel(
                        gfx, 
                        x+col,
                        y+row,
                        gfx->fill_on
                    );
                }
            }
        }
                
    }

    return GRAPHICS_OK;
}

