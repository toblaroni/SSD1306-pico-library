
#include "oled_driver/oled_driver.h"

#define OLED_ADDR 0x3D
#define GPIO_SDA 0
#define GPIO_SCL 1

int main() {

    sleep_ms(6000);

    // Initialise the OLED
    oled_init(i2c0, OLED_ADDR, GPIO_SDA, GPIO_SCL);

    while(1) {
        printf("Running...\n");
    }

    return 0;
}
