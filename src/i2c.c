#include "i2c.h"
#include "defs.h"

#include <avr/io.h>
#include <util/twi.h>

#define TWBR_VAL ((F_CPU / I2C_BAUD - 16) / 2 + 1)

static inline void i2c_wait(void) {
    while (!(TWCR & _BV(TWINT))) {
    }
}

void i2c_init(void) {
    TWBR = TWBR_VAL;
    TWSR = 0; // Prescale 1
    TWCR = 0;
}

uint8_t i2c_start(uint8_t addr, uint8_t mode) {
    TWCR = 0;
    // Send start condition
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
    i2c_wait();

    // Read into a register to avoid reading twice
    const uint8_t start_status = TW_STATUS;
    if (start_status != TW_START && start_status != TW_REP_START) {
        return 1;
    }

    // Send address

    TWDR = addr | mode;
    TWCR = _BV(TWINT) | _BV(TWEN);
    i2c_wait();

    // Status is based on the mode
    const uint8_t good_status =
            (mode == I2C_WRITE) ? TW_MT_SLA_ACK : TW_MR_SLA_ACK;
    if (TW_STATUS != good_status) {
        return 1;
    }

    return 0;
}

void i2c_stop(void) {
    TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);

    // Stop bit is cleared once it has been transmitted
    // while (TWCR & _BV(TWSTO)) {
    // }
}

uint8_t i2c_write(uint8_t data) {
    TWDR = data;
    TWCR = _BV(TWINT) | _BV(TWEN);
    i2c_wait();

    if (TW_STATUS != TW_MT_DATA_ACK) {
        return 1;
    }

    return 0;
}

uint8_t i2c_write_bytes(const uint8_t *buf, uint8_t len) {
    while (len-- > 0) {
        if (i2c_write(*buf++) != 0) {
            return 1;
        }
    }
}

uint8_t i2c_read_ack() {
    TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN);
    i2c_wait();
    return TWDR;
}

uint8_t i2c_read_nack() {
    TWCR = _BV(TWINT) | _BV(TWEN);
    i2c_wait();
    return TWDR;
}

uint8_t i2c_read_bytes(uint8_t *buf, uint8_t len) {
    while (len-- > 1) {
        *buf++ = i2c_read_ack();
    }

    *buf = i2c_read_nack();
    return 0;
}

uint8_t i2c_transmit(uint8_t addr, uint8_t data) {
    if (i2c_start(addr, I2C_WRITE) != 0) {
        return 1;
    }

    if (i2c_write(data) != 0) {
        return 1;
    }

    i2c_stop();
    return 0;
}

uint8_t i2c_transmit2(uint8_t addr, uint8_t data1, uint8_t data2) {
    if (i2c_start(addr, I2C_WRITE) != 0) {
        return 1;
    }

    if (i2c_write(data1) != 0) {
        return 1;
    }

    if (i2c_write(data2) != 0) {
        return 1;
    }

    i2c_stop();
    return 0;
}

uint8_t i2c_transmit_bytes(uint8_t addr, const uint8_t *buf, uint8_t len) {
    if (i2c_start(addr, I2C_WRITE) != 0) {
        return 1;
    }

    if (i2c_write_bytes(buf, len) != 0) {
        return 1;
    }

    i2c_stop();
    return 0;
}
