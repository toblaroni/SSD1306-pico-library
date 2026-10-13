#include <math.h>
#include <string.h>

#include "driver/SSD1306_driver.h"
#include "graphics/graphics.h"

#include "graphics/graphics_math.h"

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

    // Clear the screen
    graphics_clear(&gfx);

    SSD1306_update(&screen);
    return 0;
}
