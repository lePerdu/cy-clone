#include "defs.h"
#include "lcd.h"

#include <util/delay.h>

uint8_t lcd_init(
        uint8_t rows,
        uint8_t dotsize,
        uint8_t display_flags,
        uint8_t entry_flags) {
    uint8_t functionset = LCD_FUNCTIONSET | dotsize;
    if (rows > 1) {
        functionset |= LCD_2LINE;
    }

    // Requires 30 ms after power on
    _delay_ms(50);

    // The official Grove library sets the function 4 times for some reason

    if (lcd_command(functionset) != 0) {
        return 1;
    }

    _delay_us(4500);

    if (lcd_command(functionset) != 0) {
        return 1;
    }

    _delay_ms(150);

    if (lcd_command(functionset) != 0) {
        return 1;
    }

    if (lcd_command(functionset) != 0) {
        return 1;
    }

    // Send display flags
    if (lcd_command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | display_flags) != 0) {
        return 1;
    }

    if (lcd_clear() != 0) {
        return 1;
    }

    if (lcd_command(LCD_ENTRYMODESET | entry_flags) != 0) {
        return 1;
    }

    // Turn on the backlight

    if (lcd_set_rgb_reg(REG_MODE1, 0) != 0) {
        return 1;
    }

    if (lcd_set_rgb_reg(REG_OUTPUT, 0xFF) != 0) {
        return 1;
    }

    if (lcd_set_rgb_reg(REG_MODE2, 0x20) != 0) {
        return 1;
    }

    return 0;
}

uint8_t lcd_clear(void) {
    const uint8_t res = lcd_command(LCD_CLEARDISPLAY);
    _delay_us(2000);
    return res;
}

uint8_t lcd_cursor_home(void) {
    const uint8_t res = lcd_command(LCD_RETURNHOME);
    _delay_us(2000);
    return res;
}

uint8_t lcd_set_cursor(uint8_t col, uint8_t row) {
    const uint8_t column_mask = ((row == 0) ? 0x80 : 0xC0);
    return lcd_command(col | column_mask);
}

uint8_t lcd_puts(char *s) {
    while (*s) {
        if (lcd_putc(*s++) != 0) {
            return 1;
        }
    }

    return 0;
}

uint8_t lcd_set_rgb_reg(uint8_t reg, uint8_t val) {
    return i2c_transmit2(RGB_ADDR, reg, val);
}

uint8_t lcd_set_rgb(uint8_t r, uint8_t g, uint8_t b) {
    if (lcd_set_rgb_reg(REG_RED, r) != 0) {
        return 1;
    }

    if (lcd_set_rgb_reg(REG_GREEN, g) != 0) {
        return 1;
    }

    if (lcd_set_rgb_reg(REG_BLUE, b) != 0) {
        return 1;
    }

    return 0;
}

uint8_t lcd_set_color(struct rgb_color color) {
    return lcd_set_rgb(color.r, color.g, color.b);
}
