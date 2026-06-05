#include <math.h>
#include "oled_driver/oled_driver.h"

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
    sleep_ms(6000);

    oled_t screen;

    // Initialise the OLED
    int res = oled_init(&screen, i2c0, OLED_ADDR, GPIO_SDA, GPIO_SCL, 128, 64);
    switch (res) {
        case OLED_OK:
            printf("Initialised successfully\n");
            break;
        case OLED_ERROR_BAD_ADDRESS:
            printf("Initialisation failed. Bad address... :(\n");
            break;
        case OLED_ERROR_TIMEOUT:
            printf("Initialisation failed. Timeout... :(\n");
            break;
    }

    uint32_t frame_count = 0;
    while(1) {
        oled_clear(&screen);
        for (int x = 0; x < screen.width; x++) {
            uint8_t y = sin(x * 0.08 + frame_count * 0.1) * (screen.height / 4) + (screen.height / 2);
            oled_draw_pixel(&screen, x, y, true);
        }
        oled_update(&screen);
        frame_count++;
    }

    return 0;
}
