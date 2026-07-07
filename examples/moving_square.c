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

    graphics_no_fill(&gfx);
    graphics_stroke(&gfx, GRAPHICS_COLOUR_WHITE);

    int w = 25;
    int centre_x = screen.width/2;
    int centre_y = screen.height/2;
    int framecount = 0;
    int circle_radius = 10;
    float speed = 0.1f;

    while(1) {
        graphics_clear(&gfx);

        float angle = framecount * speed;

        int x = centre_x + (int)roundf(circle_radius * cosf(angle)) - w / 2;
        int y = centre_y + (int)roundf(circle_radius * sinf(angle)) - w / 2;

        graphics_draw_rectangle(&gfx, x, y, w, w);

        SSD1306_update(&screen);
        framecount++;
    }

    return 0;
}
