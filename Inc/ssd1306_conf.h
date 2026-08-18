#ifndef __SSD1306_CONF_H__
#define __SSD1306_CONF_H__

// NUCLEO-L476RG Arduino I2C header: PB8 = SCL/D15, PB9 = SDA/D14.
#define SSD1306_USE_I2C
#define SSD1306_I2C_ADDR        0x3CU

// Mirror the screen if needed.
// #define SSD1306_MIRROR_VERT
// #define SSD1306_MIRROR_HORIZ

// Set inverse color if needed.
// # define SSD1306_INVERSE_COLOR

// Include only the fonts used by the test program to keep flash usage smaller.
#define SSD1306_INCLUDE_FONT_6x8
#define SSD1306_INCLUDE_FONT_11x18

// The width of the screen can be set using this define. The default is 128.
// #define SSD1306_WIDTH           64

// If your screen horizontal axis does not start in column 0, adjust it here.
// #define SSD1306_X_OFFSET

// The height can be 32, 64, or 128. The default is 64.
// #define SSD1306_HEIGHT          64

#endif /* __SSD1306_CONF_H__ */
