#include <math.h>
#include <string.h>

#include "driver/SSD1306_driver.h"
#include "graphics/graphics.h"

#include "custom_font.h"
#include "hello_world_font.h"

#define OLED_ADDR 0x3D
#define GPIO_SDA 0
#define GPIO_SCL 1

int main() {

    stdio_init_all();
    i2c_init(i2c0, 400 * 1000); 
    sleep_ms(500);

    SSD1306_t screen;
    graphics_t gfx;

    // Initialise the OLED
    SSD1306_init(&screen, i2c0, OLED_ADDR, GPIO_SDA, GPIO_SCL, 128, 64);

    graphics_init(&gfx, screen.framebuff, screen.width, screen.height);

    font_t custom_font = {
        .bytes_per_col = CUSTOM_FONT_BYTES_PER_COLUMN,
        .char_height = CUSTOM_FONT_CHAR_HEIGHT,
        .char_width = CUSTOM_FONT_CHAR_WIDTH,
        .first_char = CUSTOM_FONT_FIRST_CHAR,
        .last_char = CUSTOM_FONT_LAST_CHAR,
        .line_height = 1,
        .char_spacing = 1,
        .data = custom_font_data,
        .proportional = true
    };

    font_t hello_world_font = {
        .bytes_per_col = HELLO_WORLD_BYTES_PER_COLUMN,
        .char_height = HELLO_WORLD_CHAR_HEIGHT,
        .char_width = HELLO_WORLD_CHAR_WIDTH,
        .first_char = HELLO_WORLD_FIRST_CHAR,
        .last_char = HELLO_WORLD_LAST_CHAR,
        .line_height = 1,
        .char_spacing = 1,
        .data = hello_world_data
    };


    char *text = "HELLO WORLD!";

    graphics_clear(&gfx);

    graphics_set_font(&gfx, &custom_font);
    graphics_draw_text(
        &gfx,
        text,
        strlen(text),
        10, 10,
        screen.width - 10, screen.height / 2
    );

    graphics_set_font(&gfx, &hello_world_font);
    graphics_draw_text(
        &gfx,
        text,
        strlen(text),
        10, screen.height/2 + 5,
        screen.width - 10, screen.height
    );

    SSD1306_update(&screen);
    return 0;
}
