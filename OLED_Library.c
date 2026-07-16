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

    graphics_no_stroke(&gfx);
    graphics_fill(&gfx, GRAPHICS_COLOUR_WHITE);

    int y = screen.height / 2;
    int framecount = 0;
    while(1) {
        graphics_clear(&gfx);

        graphics_draw_triangle(
            &gfx,
            screen.width/2, 10, 
            10, y,
            screen.width-10, y
        );

        int offset = roundf(sin(framecount * 0.1f) * 19.f);
        y = screen.height/2 + offset;

        SSD1306_update(&screen);
        framecount++;
    }

    return 0;
}
