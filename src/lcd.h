#ifndef LCD_H_
#define LCD_H_

#include "i2c.h"

#include <inttypes.h>

#define LCD_ADDR (0x3E << 1)
#define RGB_ADDR (0x62 << 1)

#define REG_RED 0x04
#define REG_GREEN 0x03
#define REG_BLUE 0x02

#define REG_MODE1 0x00
#define REG_MODE2 0x01
#define REG_OUTPUT 0x08

#define LCD_SEND_COMMAND 0x80
#define LCD_SEND_CHAR 0x40

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

typedef struct rgb_color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_color;

/**
 * Send a command to the LCD.
 *
 * \param command Command made from a combination of the flags defined above.
 * \return 0 on success, 1 on error.
 */
static inline uint8_t lcd_command(uint8_t command) {
    return i2c_transmit2(LCD_ADDR, LCD_SEND_COMMAND, command);
}

/**
 * Print a character to the LCD.
 *
 * \param c Character to print.
 * \return 0 on success, 1 on error.
 */
static inline uint8_t lcd_putc(char c) {
    return i2c_transmit2(LCD_ADDR, LCD_SEND_CHAR, c);
}

/**
 * Initialize and clear the LCD.
 *
 * \param rows Number of rows (1 or 2).
 * \param dotsize Either LCD_5x8DOTS or LCD_5x10DOTS (only for single-row).
 * \param display_flags Display flags (e.g. for the cursor).
 * \return 0 on success, 1 on error.
 */
uint8_t lcd_init(
        uint8_t rows,
        uint8_t dotsize,
        uint8_t display_flags,
        uint8_t entry_flags);

/**
 * Clear the LCD.
 *
 * \return 0 on success, 1 on error.
 */
uint8_t lcd_clear(void);

/**
 * Reset cursor to initial position.
 *
 * \return 0 on success, 1 on error.
 */
uint8_t lcd_cursor_home(void);

/**
 * Set the LCD cursor position.
 *
 * \param col Column, from 0 to 15.
 * \param row Row, from 0 to 1 (depending on the configuration in lcd_init).
 * \return 0 on success, 1 on error.
 */
uint8_t lcd_set_cursor(uint8_t col, uint8_t row);

/**
 * Print a string on a single line to the LCD.
 *
 * \param s String to print.
 * \return 0 on success, 1 on error.
 */
uint8_t lcd_puts(char *s);

/**
 * Set the LCD RGB color or mode register.
 *
 * \param reg Register to set.
 * \param val Value to set it.
 * \return 0 on success, 1 on error.
 */
uint8_t lcd_set_rgb_reg(uint8_t reg, uint8_t val);

/**
 * Sets the LCD backlight color intensities.
 *
 * \param r, g, b RGB intensities.
 * \return 0 on success, 1 on error.
 */
uint8_t lcd_set_rgb(uint8_t r, uint8_t g, uint8_t b);

/**
 * Sets the LCD backlight color.
 *
 * \param color RGB intensities.
 * \return 0 on success, 1 on error.
 */
uint8_t lcd_set_color(rgb_color color);

#endif // LCD_H_
