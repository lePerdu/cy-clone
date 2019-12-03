# LuCY Project

LuCY: Linearized CYclone


## Building

The following are build dependencies:
- [AVR GCC toolchain](https://www.microchip.com/mplab/avr-support/avr-and-arm-toolchains-c-compilers)
- [AVRdude](https://www.nongnu.org/avrdude) (for uploading)

Once the dependencies are installed, this can be built by running `make` and
uploaded to the microcontroller with `make upload` (you may need to set the
PORT variable like `make PORT=... upload`).

## References

These libraries are not used directly because they are either for different
platforms or have extra/worse functionality, but were very helpful in making our
implementation.

- Video of the game this is loosely based on:
  https://www.youtube.com/watch?v=9-XAB7g7R84
- Grove LCD controller for Arduino:
  https://github.com/Seeed-Studio/Grove_LCD_RGB_Backlight/
- I2C library for AVR: https://github.com/knightshrub/I2C-master-lib
- Another I2C library: https://github.com/jeremycole/avr
