#include "defs.h"
#include "i2c.h"
#include "lcd.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>
#include <util/delay.h>

#define STOP_BTN (PIND & _BV(PD2))
#define RESET_BTN (PIND & _BV(PD3))

#define BUZZER_PORT PORTD
#define BUZZER_PIN PD7

#define STOP_FLASH_COUNT 2

enum lcd_color {
    WHITE,
    DARK_RED,
    LIGHT_RED,
    DARK_GREEN,
    LIGHT_GREEN,
    BLUE,
};

static uint8_t light_index = 0;

struct light_config {
    uint8_t portb;
    uint8_t portd;
    uint8_t score;
    enum lcd_color color_index;
};

static const struct light_config LIGHT_CONFIGS[] = {
        {0x00, 0x01, 10, DARK_GREEN},
        {0x00, 0x02, 25, BLUE},
        // Skip PD2 and PD3
        {0x00, 0x10, 50, DARK_RED},
        {0x00, 0x20, 100, LIGHT_RED},
        {0x00, 0x40, 200, LIGHT_GREEN},
        // Skip PD7 (to balance lights across PORTB and PORTD)
        {0x01, 0x00, 100, LIGHT_RED},
        {0x02, 0x00, 50, DARK_RED},
        {0x04, 0x00, 25, BLUE},
        {0x08, 0x00, 10, DARK_GREEN},
};

static const struct rgb_color LCD_COLORS[] = {
        [WHITE] = {127, 127, 127},
        [DARK_RED] = {127, 0, 0},
        [LIGHT_RED] = {255, 0, 0},
        [DARK_GREEN] = {0, 127, 0},
        [LIGHT_GREEN] = {0, 255, 0},
        [BLUE] = {0, 0, 255},
};

static inline void light_delay(void) {
    _delay_ms(40);
}

static inline void flash_delay(void) {
    _delay_ms(250);
}

static inline void buzzer_on(void) {
    BUZZER_PORT |= _BV(BUZZER_PIN);
}

static inline void buzzer_off(void) {
    BUZZER_PORT &= ~_BV(BUZZER_PIN);
}

static inline const struct light_config *current_light(void) {
    return &LIGHT_CONFIGS[light_index];
}

static void clear_light(void) {
    PORTB = 0;
    PORTD = 0;
}

static void set_light(void) {
    PORTB = current_light()->portb;
    PORTD = current_light()->portd;
}

static void light_loop(void) {
    for (light_index = 0; light_index <= 8; ++light_index) {
        set_light();
        light_delay();
    }

    for (light_index = 7; light_index >= 1; --light_index) {
        set_light();
        light_delay();
    }
}

static void show_instructions(void) {
    lcd_clear();
    lcd_puts("Touch the sensor");
    lcd_set_cursor(0, 1);
    lcd_puts("to stop light");
}

static void print_points(void) {
    char buf[4];
    utoa(current_light()->score, buf, 10);
    lcd_clear();
    lcd_puts("Your score: ");
    lcd_puts(buf);

    lcd_set_cursor(0, 1);
    lcd_puts("Button to retry");
}

ISR(INT0_vect) {
    // Set the screen and the light (in case set_light()) was interrupted
    lcd_set_color(LCD_COLORS[current_light()->color_index]);
    set_light();

    // clear_light() and set_light() turn off the buzzer
    buzzer_on();

    // Flash the light a few times
    for (uint8_t i = 0; i < STOP_FLASH_COUNT; ++i) {
        flash_delay();
        lcd_set_color(LCD_COLORS[WHITE]);
        clear_light();

        flash_delay();

        lcd_set_color(LCD_COLORS[current_light()->color_index]);
        set_light();
        buzzer_on();
    }

    flash_delay();
    buzzer_off();
    lcd_clear();
    print_points();

    // Stay in the ISR, essentially pausing the game

    // Reset when PD3 is pressed
    while (!RESET_BTN) {
    }

    lcd_set_color(LCD_COLORS[WHITE]);
    show_instructions();

    // Clear the pending interrupt in case the sensor was tapped during this
    // handler
    EIFR |= _BV(INTF0);
}

static void init(void) {
    // Output pins
    DDRD = 0b11110011; // Skip PD2 and PD3 for buttons
    DDRB = 0b00001111;

    i2c_init();

    lcd_init(
            2,
            LCD_5x8DOTS,
            LCD_CURSOROFF | LCD_BLINKOFF,
            LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);

    lcd_set_color(LCD_COLORS[WHITE]);
    show_instructions();

    // Set INT0 for rising edge of PD2
    EIMSK = _BV(INT0);
    EICRA = _BV(ISC01) | _BV(ISC00);

    // Enable interrupts
    sei();
}

int main(void) {
    init();

    // Main loop
    while (1) {
        light_loop();
    }

    return 0;
}
