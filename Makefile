PROJECT_NAME:=cyclone

SRC:=src
BIN:=bin

MCU:=atmega328p

CC:=avr-gcc
AS:=avr-as
# Use gcc as the linker so that it will use link with the CRTs
LD:=avr-gcc
OBJCOPY:=avr-objcopy

AVRDUDE:=avrdude
AVRDUDE_CONF:=/etc/avrdude.conf

LOCAL_ASFLAGS:=-Iinclude -mmcu=$(MCU)
LOCAL_CFLAGS:=-mmcu=$(MCU) -g
LOCAL_LDFLAGS:=-mmcu=$(MCU) -g

# Make it small by default
CFLAGS?=-Os

# Gather all by default
SOURCES?=$(wildcard $(SRC)/*.c $(SRC)/*.asm)
OBJECTS:=$(patsubst $(SRC)/%,$(BIN)/%.o, $(basename $(SOURCES)))

ELFFILE:=$(BIN)/$(PROJECT_NAME).elf
HEXFILE:=$(patsubst %.elf,%.hex,$(ELFFILE))

all: $(HEXFILE)

$(BIN)/%.o: $(SRC)/%.c | $(BIN)
	$(CC) $(LOCAL_CFLAGS) $(CFLAGS) -c -o $@ $<

$(BIN)/%.o: $(SRC)/%.asm | $(BIN)
	$(AS) $(LOCAL_ASFLAGS) $(ASFLAGS) -o $@ $<

$(ELFFILE): $(OBJECTS) | $(BIN)
	$(LD) $(LOCAL_LDFLAGS) $(LDFLAGS) -o $@ $^

$(HEXFILE): $(ELFFILE)
	$(OBJCOPY) -O ihex $< $@

$(BIN):
	@mkdir -p $@

# Keep these around
.SECONDARY: $(ELFFILE) $(OBJECTS)

upload: $(HEXFILE)
	$(AVRDUDE) -C $(AVRDUDE_CONF) -P /dev/ttyUSB0 -p $(MCU) -c arduino -b 115200 \
		-U flash:w:$<:i

clean:
	rm -rf $(BIN)

.PHONY: all clean
