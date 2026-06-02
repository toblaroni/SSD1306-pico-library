#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define OLED_ADDR 0x3C

void send_command(uint8_t cmd) {
    uint8_t buf[2];

    buf[0] = 0x00;
    buf[1] = cmd;

    i2c_write_blocking(i2c0, OLED_ADDR, buf, 2, false);
}

int main() {
    stdio_init_all();
    i2c_init(i2c0, 400000);

    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);

    gpio_pull_up(0);
    gpio_pull_up(1);

    sleep_ms(5000);
    printf("Starting I2C scan...\n");

    for (int addr = 0; addr < 128; addr++) {
        uint8_t data = 0;

        int result = i2c_write_blocking(i2c0, addr, &data, 1, false);

        if (result >= 0) {
            printf("Device found at 0x%02X\n", addr);
        }
    }

    printf("Scan done.\n");

    while (true) {
        sleep_ms(1000);
    }
}
