#ifndef CUSTOM_FONT_H
#define CUSTOM_FONT_H

#include <stdint.h>

#define CUSTOM_FONT_FIRST_CHAR 32
#define CUSTOM_FONT_LAST_CHAR 126
#define CUSTOM_FONT_CHAR_WIDTH 5
#define CUSTOM_FONT_CHAR_HEIGHT 8
#define CUSTOM_FONT_BYTES_PER_COLUMN 1

extern const uint8_t custom_font_data[];

#endif
