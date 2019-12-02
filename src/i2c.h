/**
 * I2C (TWI) driver for the master end.
 */

#ifndef I2C_H_
#define I2C_H_

#include <inttypes.h>

#define I2C_READ 1
#define I2C_WRITE 0

void i2c_init(void);

/**
 * Start an I2C transmission.
 * 
 * \param addr 7-bit address, shifted to the left 1 bit to make space for the
 * R/W mode.
 * \param mode Either I2C_READ or I2C_WRITE.
 * \return 0 on success, 1 on error.
 */
uint8_t i2c_start(uint8_t addr, uint8_t mode);

/**
 * End an I2C transmission.
 */
void i2c_stop(void);

/**
 * Sends a byte/packet via I2C.
 * 
 * This must be called between i2c_start() and i2c_stop().
 * 
 * \param data Byte to send.
 * \return 0 on success, 1 on error.
 */
uint8_t i2c_write(uint8_t data);

/**
 * Sends a buffer of bytes via I2C.
 * 
 * This must be called between i2c_start() and i2c_stop().
 * 
 * \param buf Start of the buffer to send.
 * \param len Number of bytes to send.
 * \return 0 on success, 1 on error of any of the writes.
 */
uint8_t i2c_write_bytes(const uint8_t *buf, uint8_t len);

/**
 * Read a byte from the salve, with acknowledgement.
 * 
 * This must be called between i2c_start() and i2c_stop().
 * 
 * \return Byte read.
 */
uint8_t i2c_read_ack();

/**
 * Read a byte from the salve, without acknowledgement.
 * 
 * This must be called between i2c_start() and i2c_stop().
 * 
 * \return Byte read.
 */
uint8_t i2c_read_nack();

/**
 * Read a byte from the salve, with acknowledgement.
 * 
 * This must be called between i2c_start() and i2c_stop().
 * 
 * \param buf Buffer to read into.
 * \param lenNumber of bytes to read.
 * \return 0 on success, 1 on error.
 */
uint8_t i2c_read_bytes(uint8_t *buf, uint8_t len);

/**
 * Helper function for sending a single byte to a slave.
 * 
 * \param addr Addresss to send to.
 * \param data Byte to send.
 * \return 0 on success, 1 on error.
 */
uint8_t i2c_transmit(uint8_t addr, uint8_t data);

/**
 * Helper function for sending a pair of bytes to a slave.
 * 
 * This is common for sending commands or setting regisers.
 * 
 * \param addr Addresss to send to.
 * \param data1 First byte to send.
 * \param data2 Second byte to send.
 * \return 0 on success, 1 on error.
 */
uint8_t i2c_transmit2(uint8_t addr, uint8_t data1, uint8_t data2);

/**
 * Helper function for sending a string of bytes to a slave.
 * 
 * \param addr Addresss to send to.
 * \param buf Start of the buffer to send.
 * \param len Number of bytes to send.
 * \return 0 on success, 1 on error.
 */
uint8_t i2c_transmit_bytes(uint8_t addr, const uint8_t *buf, uint8_t len);

#endif // I2C_H_
