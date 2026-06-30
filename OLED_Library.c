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
    
    graphics_stroke(&gfx, GRAPHICS_COLOUR_WHITE);

    graphics_draw_triangle(
        &gfx, 
        20, 20, 
        screen.width - 20, 20, 
        screen.width / 2, screen.height - 20
    );
    
    SSD1306_update(&screen);

    while(1) {
    }

    return 0;
}
