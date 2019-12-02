#include "defs.h"
#include "i2c.h"
#include "lcd.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#include <util/delay.h>

static uint8_t light_index = 0;

static const uint8_t LIGHT_POINTS[] = {10, 25, 50, 100, 200, 100, 50, 25, 10};

void light_delay(void) {
    _delay_ms(50);
}

void set_light(uint8_t index) {
    light_index = index;
    if (index < 2) {
        PORTD = (1 << index);
        PORTB = 0;
    } else if (index < 5) {
        // Skip over PD2 and PD3 (buttons)
        PORTD = (1 << (index + 2));
        PORTB = 0;
    } else if (index < 9) {
        PORTD = 0;
        PORTB = (1 << (index - 5));
    }
}

void light_loop(void) {
    for (uint8_t i = 0; i <= 8; ++i) {
        set_light(i);
        light_delay();
    }

    for (uint8_t i = 7; i >= 1; --i) {
        set_light(i);
        light_delay();
    }
}

void print_points(void) {
    char buf[4];
    snprintf(buf, 4, "%3d", LIGHT_POINTS[light_index]);
    lcd_puts(buf);
}

ISR(INT0_vect) {
    // In case the set_light() function was interrupted
    set_light(light_index);

    print_points();

    // Stay in the ISR, essentially pausing the game

    // Reset when PD3 is pressed
    while (!(PIND & 0x08)) {
    }

    lcd_clear();
}

void setup() {
    // Output pins
    DDRD = 0b01110011; // Skip PD2 and PD3 for buttons
    DDRB = 0b00001111;

    i2c_init();

    lcd_init(
            2,
            LCD_5x8DOTS,
            LCD_CURSORON | LCD_BLINKON,
            LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);

    lcd_set_rgb(5, 5, 5);

    // Set INT0 for rising edge of PD2
    EIMSK = _BV(INT0);
    EICRA = _BV(ISC01) | _BV(ISC00);

    // Enable interrupts
    sei();
}

int main(void) {
    setup();

    // Main loop
    while (1) {
        light_loop();
    }

    return 0;
}
