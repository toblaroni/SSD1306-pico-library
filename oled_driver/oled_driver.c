#include "oled_driver.h"

#define SET_MUX_RATIO 0xA8
#define SET_DISP_OFFSET 0xD3
#define SET_DISP_START_LINE 0x40

static int oled_send_cmd(const oled_t *screen, uint8_t cmd) {
    uint8_t buf[2];

    buf[0] = 0x00;
    buf[1] = cmd;

    int res = i2c_write_blocking(screen->i2c, screen->addr, buf, 2, false);

    switch (res) {
        case PICO_ERROR_GENERIC:
            printf("ERROR: I2C write failed: %d\nAddress not acknowledged or no device present at %02X\n", cmd, screen.addr);
            return OLED_ERROR_BAD_ADDRESS;
        case PICO_ERROR_TIMEOUT:
            printf("ERROR: I2C write failed. Timed out while writing %d\n", cmd);
            return OLED_ERROR_TIMEOUT;
        default:
            return OLED_OK;
    }
}

static int addr_check(oled_t *const screen) {
    uint8_t buf;
    int res = i2c_read_blocking(screen->i2c, screen->addr, &buf, 1, false);

    switch (res) {
        case PICO_ERROR_GENERIC:
            printf("ERROR: Address not acknowledged or no device present at %02X\n", screen.addr);
            return res;
        case PICO_ERROR_TIMEOUT:
            printf("ERROR: Initialisation failed. I2C read timed out at address %02X\n", screen.addr);
            return res;
        default:
            printf("Successfully located SSD1306 at %02X\n", screen->addr);
            return OLED_OK;
    }
}

// Initialise the OLED display.
int oled_init(oled_t *const screen, i2c_inst_t* _i2c, uint8_t _address, uint8_t gpio_sda, uint8_t gpio_scl) {
    screen->i2c = _i2c;
    screen->addr = _address; 
    screen->sda_pin = gpio_sda; 
    screen->scl_pin = gpio_scl; 

    gpio_set_function(gpio_sda, GPIO_FUNC_I2C);
    gpio_set_function(gpio_scl, GPIO_FUNC_I2C);

    gpio_pull_up(gpio_sda);
    gpio_pull_up(gpio_scl);

    // Check the OLED address for an ACK
    int res = addr_check(screen);
    if (res != OLED_OK)
        return res;

    // Allocate frame buffer
    screen->pages = screen->height / 8;
    screen->framebuf_size = screen->width * screen->pages;
    screen->framebuff = (uint8_t*) malloc(screen->framebuf_size);

    if (screen->framebuff == NULL) {
        screen->framebuf_size = 0;
        return OLED_ERROR_NULL_FRAMEBUFFER
    }

    // Init sequence
    

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
