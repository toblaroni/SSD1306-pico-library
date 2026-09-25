#ifndef FONT_H
#define FONT_H

#include "graphics.h"

int graphics_draw_char(graphics_t *const gfx, char c, int x, int y);
int graphics_draw_text(graphics_t *const gfx, char *const c, size_t length, int x1, int y1, int x2, int y2);

#endif