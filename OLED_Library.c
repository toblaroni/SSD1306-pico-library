
#include "oled_driver/oled_driver.h"

#define OLED_ADDR 0x3D
#define GPIO_SDA 0
#define GPIO_SCL 1

int main() {

    stdio_init_all();
    i2c_init(i2c0, 100 * 1000); 
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

    // Left eye
    oled_draw_pixel(&screen, 58, 10, true);
    oled_update(&screen);

    while(1) {
        continue;
    }

    return 0;
}
