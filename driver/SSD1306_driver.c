/*
 *  Lightweight driver for the SSD1306.
 */
#include "SSD1306_driver.h"

#define CONTROL_CMD_STREAM  0x00    // Co = 0, CD# = 0 - Control byte for command stream
#define CONTROL_CMD         0x80    // Co = 1, CD# = 0 - Control byte for single command
#define CONTROL_DATA_STREAM 0x40    // Co = 0, CD# = 1 - Control byte for data stream

// SSD1306 command definitions
#define SET_MUX_RATIO           0xA8
#define SET_DISP_OFFSET         0xD3
#define SET_DISP_START_LINE     0x40
#define SET_SEG_REMAP_RST       0xA0    // Column address 0 is mapped to SEG0 (A1 for inverse)
#define SET_COM_OUT_DIR         0xC0    // Scan from COM0 to COM[N-1] (C8 for inverse)
#define SET_COM_PIN_CFG         0xDA
#define SET_CONTRAST_CTRL       0x81
#define SET_ENTIRE_OFF          0xA4
#define SET_ENTIRE_ON           0xA5
#define SET_NORM_DISP           0xA6
#define SET_INV_DISP            0xA7
#define SET_OSC_FREQ            0xD5
#define SET_CHARGE_PUMP_REG     0x8D
#define DISPLAY_ON              0xAF
#define DISPLAY_OFF             0xAE
#define SET_ADDR_MODE           0x20
#define ADDR_MODE_HORZ          0x00    // Horizontal addressing mode
#define SET_COL_ADDR            0x21
#define SET_PAGE_ADDR           0x22
#define SET_PRECHARGE_PERIOD    0xD9
#define VCOMH_DESEL_LEVEL       0xDB
#define SET_DISP_CLK_DIV        0xD5

static void free_framebuffer(SSD1306_t *const screen) {
    if (screen->framebuff != NULL) {
        free(screen->framebuff);
        screen->framebuff = NULL;
        screen->framebuf_size = 0;
    }
}

static int SSD1306_handle_error(SSD1306_t *const screen, int res) {
    switch (res) {
        case PICO_ERROR_GENERIC:
            printf("ERROR: I2C write failed. Address not acknowledged or no device present at %02X\n", screen->addr);
            return SSD1306_ERROR_BAD_ADDRESS;
        case PICO_ERROR_TIMEOUT:
            printf("ERROR: I2C write failed. Timed out.");
            return SSD1306_ERROR_TIMEOUT;
        default:
            return SSD1306_OK;
    }
}

static int SSD1306_send_cmd(SSD1306_t *const screen, uint8_t cmd) {
    /** Send a single command */
    uint8_t buf[2] = { CONTROL_CMD, cmd };

    return SSD1306_handle_error(
        screen,
        i2c_write_blocking(screen->i2c, screen->addr, buf, 2, false)
    );
}

static int SSD1306_send_cmd_stream(SSD1306_t *const screen, uint8_t* cmds, size_t len) {
    /** Send a stream of commands */
    uint8_t buf[len+1];
    buf[0] = CONTROL_CMD_STREAM;
    memcpy(&buf[1], cmds, len);
    return SSD1306_handle_error(
        screen,
        i2c_write_blocking(screen->i2c, screen->addr, buf, len+1, false)
    );
}

static int SSD1306_send_data(SSD1306_t *const screen) {
    /** Used to send GDDRAM data to device */
    uint8_t *buf = malloc(screen->framebuf_size + 1);
    if (buf == NULL) return SSD1306_ERROR_NULL_FRAMEBUFFER;

    buf[0] = CONTROL_DATA_STREAM;
    memcpy(buf + 1, screen->framebuff, screen->framebuf_size);

    int res = SSD1306_handle_error(screen,
        i2c_write_blocking(screen->i2c, screen->addr, buf, screen->framebuf_size + 1, false)
    );

    free(buf);
    return res;
}


static int addr_check(SSD1306_t *const screen) {
    uint8_t buf;
    int res = i2c_read_blocking(screen->i2c, screen->addr, &buf, 1, false);

    switch (res) {
        case PICO_ERROR_GENERIC:
            printf("ERROR: Address not acknowledged or no device present at %02X\n", screen->addr);
            return SSD1306_ERROR_BAD_ADDRESS;
        case PICO_ERROR_TIMEOUT:
            printf("ERROR: Initialisation failed. I2C read timed out at address %02X\n", screen->addr);
            return SSD1306_ERROR_TIMEOUT;
        default:
            printf("Successfully located SSD1306 at %02X\n", screen->addr);
            return SSD1306_OK;
    }
}

// Initialise the SSD1306 display.
int SSD1306_init(SSD1306_t *const screen, i2c_inst_t* _i2c, uint8_t _address, uint8_t gpio_sda, uint8_t gpio_scl, uint16_t width, uint16_t height) {
    screen->i2c = _i2c;
    screen->addr = _address;
    screen->sda_pin = gpio_sda;
    screen->scl_pin = gpio_scl;
    screen->width = width;
    screen->height = height;
    screen->framebuff = NULL;

    // Configure GPIO pins for I2C
    gpio_set_function(gpio_sda, GPIO_FUNC_I2C);
    gpio_set_function(gpio_scl, GPIO_FUNC_I2C);
    gpio_pull_up(gpio_scl);
    gpio_pull_up(gpio_sda);

    // Check the SSD1306 address for an ACK
    int res = addr_check(screen);
    if (res != SSD1306_OK)
        return res;

    // Allocate frame buffer
    screen->pages = screen->height / 8;
    screen->framebuf_size = screen->width * screen->pages;
    screen->framebuff = (uint8_t*) malloc(screen->framebuf_size);

    if (screen->framebuff == NULL) {
        screen->framebuf_size = 0;
        return SSD1306_ERROR_NULL_FRAMEBUFFER;
    }

    // https://github.com/makerportal/rpi-pico-ssd1306/blob/main/micropython/data_display/ssd1306.py#L114
    uint8_t com_pin_cfg = (width > 2*height) ? 0x02 : 0x12;
    // Init sequence
    // https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
    uint8_t init_cmds[] = {
        DISPLAY_OFF,                    // Display off
        SET_ADDR_MODE, ADDR_MODE_HORZ,  // Set horizontal addressing mode
        SET_DISP_START_LINE | 0x00,     // Display start line
        SET_SEG_REMAP_RST | 0x01,       // Column address 127 is mapped to SEG0
        SET_MUX_RATIO, height-1,        // Mux ratio (1 to 64)
        SET_COM_OUT_DIR | 0x08,         // Scan from COM0 to COM[N-1] (C8 for inverse)
        SET_DISP_OFFSET, 0x00,          // Display offset
        SET_COM_PIN_CFG, com_pin_cfg,
        SET_DISP_CLK_DIV, 0x80,         // Set display clock divide ratio/oscillator frequency
        SET_PRECHARGE_PERIOD, 0xF1,     // Should be determined by external VCC source...
        VCOMH_DESEL_LEVEL, 0x30,
        SET_CONTRAST_CTRL, 0x7F,        // Contrast control
        SET_ENTIRE_OFF,                 // Disable entire display on
        SET_NORM_DISP,                  // Set normal display mode (not inverted)
        SET_CHARGE_PUMP_REG, 0x14,      // Enable charge pump regulator
        SET_OSC_FREQ, 0x80,             // Set clock divide ratio and oscillator frequency
        DISPLAY_ON
    };

    res = SSD1306_send_cmd_stream(screen, init_cmds, sizeof(init_cmds));
    if (res != SSD1306_OK) return res;

    return SSD1306_OK;
}

int SSD1306_deinit(SSD1306_t *const screen) {
    free_framebuffer(screen);
    return SSD1306_OK;
}

int SSD1306_update(SSD1306_t *const screen) {
    // Set column/page addresses to update full screen
    uint8_t addr_cmds[] = {
        SET_COL_ADDR, 0x00, screen->width - 1,  // Set column address range
        SET_PAGE_ADDR, 0x00, screen->pages - 1  // Set page address range
    };
    int res = SSD1306_send_cmd_stream(screen, addr_cmds, sizeof(addr_cmds));
    if (res != SSD1306_OK) return res;
    return SSD1306_send_data(screen);
}

int SSD1306_on(SSD1306_t *const screen) {
    return SSD1306_send_cmd(screen, DISPLAY_ON);
}

int SSD1306_off(SSD1306_t *const screen) {
    return SSD1306_send_cmd(screen, DISPLAY_OFF);
}

int SSD1306_invert(SSD1306_t *const screen, bool invert) {
    return SSD1306_send_cmd(screen, invert ? SET_INV_DISP : SET_NORM_DISP);
}