
#include "oled_driver/oled_driver.h"

#define OLED_ADDR 0x3D
#define GPIO_SDA 0
#define GPIO_SCL 1

int main() {

    stdio_init_all();
    i2c_init(i2c0, 400000);
    sleep_ms(6000);

    oled_t screen;
    screen.addr = OLED_ADDR;
    screen.i2c = i2c0;
    screen.scl_pin = GPIO_SCL;
    screen.sda_pin = GPIO_SDA;
    screen.width = 128;
    screen.height = 64;

    // Initialise the OLED
    int res = oled_init(&screen, i2c0, OLED_ADDR, GPIO_SDA, GPIO_SCL);

    while(1) {
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
        sleep_ms(1500);
    }

    return 0;
}
