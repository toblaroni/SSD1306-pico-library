#include "oled_driver.h"

oled_t screen;

int oled_send_cmd(uint8_t cmd) {
    uint8_t buf[2];

    buf[0] = 0x00;
    buf[1] = cmd;

    int result = i2c_write_blocking(screen.i2c, screen.addr, buf, 2, false);
    if (result == PICO_ERROR_GENERIC) {
        printf("ERROR: I2C write failed: %d\nAddress not acknowledged or no device present at %02X\n", result, screen.addr);
        return OLED_ERROR_BAD_ADDRESS;
    }
    return OLED_OK;
}

bool addr_check() {
    uint8_t buf;
    int res = i2c_read_blocking(screen.i2c, screen.addr, &buf, 1, false);
    if (res == PICO_ERROR_GENERIC) {
        printf("ERROR: Address not acknowledged or no device present at %02X", screen.addr);
        return false;
    }
    printf("Successfully located SSD1306 at %02X", screen.addr);
}

// Initialise the OLED display.
int oled_init(i2c_inst_t* _i2c, uint8_t _address, uint8_t gpio_sda, uint8_t gpio_scl) {
    stdio_init_all();
    i2c_init(_i2c, 400000);

    screen.i2c = _i2c;
    screen.addr = _address; 
    screen.sda_pin = gpio_sda; 
    screen.scl_pin = gpio_scl; 

    gpio_set_function(gpio_sda, GPIO_FUNC_I2C);
    gpio_set_function(gpio_scl, GPIO_FUNC_I2C);

    gpio_pull_up(gpio_sda);
    gpio_pull_up(gpio_scl);

    // Check the OLED address for an ACK
    if (!addr_check)
        return OLED_ERROR_BAD_ADDRESS;

    return OLED_OK;
}

void oled_update() {
    return;
}

void oled_clear() {
    return;
}

void oled_draw_pixel() {
    return;
}
