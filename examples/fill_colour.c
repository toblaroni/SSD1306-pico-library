#include <math.h>
#include "driver/SSD1306_driver.h"
#include "graphics/graphics.h"

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
    int res = SSD1306_init(&screen, i2c0, OLED_ADDR, GPIO_SDA, GPIO_SCL, 128, 64);
    switch (res) {
        case SSD1306_OK:
            printf("Initialised successfully\n");
            break;
        case SSD1306_ERROR_BAD_ADDRESS:
            printf("Initialisation failed. Bad address... :(\n");
            break;
        case SSD1306_ERROR_TIMEOUT:
            printf("Initialisation failed. Timeout... :(\n");
            break;
    }

    graphics_init(&gfx, screen.framebuff, screen.width, screen.height);
    graphics_clear(&gfx);
    
    int stroke_width = 2;
    int x = 0, y = 0;

    graphics_no_stroke(&gfx);
    for (
        int w = screen.width-1, h = screen.height-1; 
        w > 0 && h > 0; 
        w -= stroke_width*2, h -= stroke_width*2
    ) {
        graphics_draw_rectangle(&gfx, x, y, w, h);

        graphics_fill(&gfx, gfx.fill_colour);
        gfx.fill_colour = gfx.fill_colour == GRAPHICS_COLOUR_WHITE ? GRAPHICS_COLOUR_BLACK : GRAPHICS_COLOUR_WHITE;

        x += stroke_width;
        y += stroke_width;

    }

    for (int i = screen.width/2; i >= 2; i -= 2) {
        graphics_draw_circle(&gfx, screen.width/2, screen.height/2, i);

        graphics_fill(&gfx, gfx.fill_colour);
        gfx.fill_colour = gfx.fill_colour == GRAPHICS_COLOUR_WHITE ? GRAPHICS_COLOUR_BLACK : GRAPHICS_COLOUR_WHITE;
    }
    
    SSD1306_update(&screen);

    while(1) {
    }

    return 0;
}
