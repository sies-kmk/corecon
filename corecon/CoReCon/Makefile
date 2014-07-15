###############################################################################
# Makefile for the Costar Replacement Controllers
###############################################################################

#MODEL = FrostyFlake
#MODEL = FrostyFlake2
#MODEL = PegasusHoof
#MODEL = KittenPaw
#MODEL = BlackPetal
#MODEL = GhostSquid

## General Flags
PROJECT = CoReCon
MCU = atmega32u2
TARGET = $(MODEL).elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -DF_CPU=16000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -D$(MODEL)
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=$(MODEL).map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"..\..\includes" -I".." 

## Objects that must be built in order to link
OBJECTS = kb.o u_des_kb.o u_hid_kb.o kbmain.o kbmacro.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) $(MODEL).hex $(MODEL).eep $(MODEL).lss## Compile
kb.o: ../kb.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

u_des_kb.o: ../../includes/u_des_kb.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

u_hid_kb.o: ../../includes/u_hid_kb.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

kbmain.o: ../../includes/kbmain.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

kbmacro.o: ../../includes/kbmacro.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) $(MODEL).elf dep/* $(MODEL).hex $(MODEL).eep $(MODEL).lss $(MODEL).map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

