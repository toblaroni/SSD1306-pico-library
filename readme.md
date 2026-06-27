# SSD1306 Libraries
In order to learn about embedded programming I'm writing a set of libraries for the SSD1306 and the Pico. I'm using the standard Pico and [this OLED display](https://www.adafruit.com/product/938).

### driver
A lightweight driver for the SSD1306. Communicates over I2C. 

> For faster graphics change to SPI?

### graphics
A library of functions for drawing on the SSD1306. The logic is separated from the driver so technically it could be used for any other display that uses the same framebuffer memory layout as the SSD1306.