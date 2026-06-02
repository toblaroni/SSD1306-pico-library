#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define OLED_ADDR 0x3D
#define GPIO_SDA 0
#define GPIO_SCL 1

void send_command(uint8_t cmd) {
    uint8_t buf[2];

    buf[0] = 0x00;
    buf[1] = cmd;

    int result = i2c_write_blocking(i2c0, OLED_ADDR, buf, 2, false);
    if (result < 0) {
        printf("I2C write failed: %d\n", result);
    }
}

int main() {
    stdio_init_all();
    i2c_init(i2c0, 400000);

    gpio_set_function(GPIO_SDA, GPIO_FUNC_I2C);
    gpio_set_function(GPIO_SCL, GPIO_FUNC_I2C);

    gpio_pull_up(GPIO_SDA);
    gpio_pull_up(GPIO_SCL);

    sleep_ms(5000);

    send_command(0xAE);
    sleep_ms(5000);
    send_command(0xAF);

}
