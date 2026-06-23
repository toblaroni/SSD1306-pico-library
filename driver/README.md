# SSD1306 Driver
SSD1306 is the driver chip for the [OLED Display Module](https://www.adafruit.com/product/938) that i am using. In order for the PICO to send commands to the screen, I need a driver. I've decided to write this myself in C to teach myself how everything works at a low level. 

### Resources
- [SSD1306 datasheet](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)
- [Python Driver Implementation](https://github.com/makerportal/rpi-pico-ssd1306/blob/main/micropython/data_display/ssd1306.py#L114)
- [Similar C project](https://github.com/daschr/pico-ssd1306/blob/main/ssd1306.c)