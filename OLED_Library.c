#include <math.h>
#include "driver/SSD1306_driver.h"
#include "graphics/graphics.h"

#define OLED_ADDR 0x3D
#define GPIO_SDA 0
#define GPIO_SCL 1

typedef struct {
    int x;
    int y;
} vec2_t;

int main() {

    stdio_init_all();
    i2c_init(i2c0, 400 * 1000); 
    sleep_ms(3000);

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
    // Horizontal and vertical
    graphics_draw_line(&gfx, 0, screen.height/2, screen.width - 1, screen.height / 2);
    graphics_draw_line(&gfx, screen.width / 2, 0, screen.width / 2, screen.height - 1);
    // Diagonals
    printf("Drawing diagonal lines...\n");
    graphics_draw_line(&gfx, 0, 0, screen.width - 1, screen.height - 1);
    graphics_draw_line(&gfx, 0, screen.height - 1, screen.width - 1, 0);
    // Steep lines
    printf("Drawing steep lines...\n");
    graphics_draw_line(&gfx, screen.width/4, 0, screen.width*3/4, screen.height - 1);
    graphics_draw_line(&gfx, screen.width*3/4, 0, screen.width/4, screen.height - 1);
    // Shallow lines
    printf("Drawing shallow lines...\n");
    graphics_draw_line(&gfx, 0, screen.height/4, screen.width - 1, screen.height*3/4);
    graphics_draw_line(&gfx, 0, screen.height*3/4, screen.width - 1, screen.height/4);
    SSD1306_update(&screen);

    uint32_t frame_count = 0;
    while(1) {
        frame_count++;
    }

    return 0;
}
