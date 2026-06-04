#include "oled_driver.h"

#define CONTROL_CMD_STREAM  0x00
#define CONTROL_CMD         0x80
#define CONTROL_DATA_STREAM 0x40

#define SET_MUX_RATIO 0xA8
#define SET_DISP_OFFSET 0xD3
#define SET_DISP_START_LINE 0x40

static int oled_handle_error(const oled_t *screen, int res) {
    switch (res) {
        case PICO_ERROR_GENERIC:
            printf("ERROR: I2C write failed. Address not acknowledged or no device present at %02X\n", screen->addr);
            return OLED_ERROR_BAD_ADDRESS;
        case PICO_ERROR_TIMEOUT:
            printf("ERROR: I2C write failed. Timed out.");
            return OLED_ERROR_TIMEOUT;
        default:
            return OLED_OK;
    }
}

static int oled_send_cmd(const oled_t *screen, uint8_t cmd) {
    /** Send a single command */
    uint8_t buf[2] = { CONTROL_CMD, cmd };

    return oled_handle_error(
        screen,
        i2c_write_blocking(screen->i2c, screen->addr, buf, 2, false)
    );
}

static int oled_send_cmd_stream(const oled_t *screen, uint8_t* cmds, size_t len) {
    /** Send a stream of commands */
    uint8_t buf[len+1];
    buf[0] = CONTROL_CMD_STREAM;
    memcpy(&buf[1], cmds, len);
    return oled_handle_error(
        screen,
        i2c_write_blocking(screen->i2c, screen->addr, buf, len+1, false)
    );

}

static int oled_send_data(const oled_t *screen) {
    /** Used to send GDDRAM data to device */
    // First send control bit
    uint8_t control = CONTROL_DATA_STREAM;
    int res = oled_handle_error(
        screen,
        i2c_write_blocking(screen->i2c, screen->addr, &control, 1, true)
    );

    if (res != OLED_OK)
        return res;

    return oled_handle_error(
        screen,
        i2c_write_blocking(screen->i2c, screen->addr, screen->framebuff, screen->framebuf_size, false)
    );
}


static int addr_check(oled_t *const screen) {
    uint8_t buf;
    int res = i2c_read_blocking(screen->i2c, screen->addr, &buf, 1, false);

    switch (res) {
        case PICO_ERROR_GENERIC:
            printf("ERROR: Address not acknowledged or no device present at %02X\n", screen->addr);
            return res;
        case PICO_ERROR_TIMEOUT:
            printf("ERROR: Initialisation failed. I2C read timed out at address %02X\n", screen->addr);
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
        return OLED_ERROR_NULL_FRAMEBUFFER;
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
