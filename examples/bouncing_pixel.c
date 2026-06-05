
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

    // Position
    vec2_t pixel_pos = {128/2, 32};
    // Velocity
    vec2_t vel = {3, 2};
    
    uint16_t update_time_ms = 0;
    
    while(1) {
        oled_clear(&screen);
        oled_draw_pixel(&screen, pixel_pos.x, pixel_pos.y, true);
        oled_update(&screen);
        if (pixel_pos.x + vel.x >= screen.width || pixel_pos.x + vel.x < 0) {
            vel.x *= -1;
        } 
        if (pixel_pos.y + vel.y >= screen.height || pixel_pos.y + vel.y < 0) {
            vel.y *= -1;
        } 
        pixel_pos.x += vel.x;
        pixel_pos.y += vel.y;
        sleep_ms(update_time_ms);
    }

    return 0;
}
