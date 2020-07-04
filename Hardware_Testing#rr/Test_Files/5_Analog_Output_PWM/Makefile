# ATMega2560
# startup: Makefile for $(CPU)
# AUTHOR: Sourav Jena (sourav@e-yantra.org)

# $< - first file
# $@ - second file

#-------------- VARIABLES --------------
PROJECT_NAME = $(shell basename $(CURDIR))

CPU = atmega2560

AVR_CC = avr-gcc 						# exported by eY-extension
AVR_CCFLAGS = -g -Os -mmcu=$(CPU) -std=gnu99 -c
AVR_OBJCOPY = avr-objcopy				# exported by eY-extension

BUILDDIR = build

C_SRCS:=$(wildcard *.c) 	# list of all c-files in directory
C_OBJ:=$(C_SRCS:.c=.o) 		# list of all object files in directory

objects = $(C_OBJ)

DIR_C_OBJ:=$(patsubst %.o, $(BUILDDIR)/%.o, $(C_OBJ))

# --------------------------------------
all: startup $(PROJECT_NAME).bin all_2

clean: 
	rm -rf *.o *.bin *.hex *.elf -r $(BUILDDIR)

$(PROJECT_NAME).bin: $(PROJECT_NAME).hex
	$(AVR_OBJCOPY) -I ihex $(BUILDDIR)/$(PROJECT_NAME).hex -O binary $(BUILDDIR)/$(PROJECT_NAME).bin

$(PROJECT_NAME).hex: $(PROJECT_NAME).elf
	$(AVR_OBJCOPY) -j .text -j .data -O ihex $(BUILDDIR)/$(PROJECT_NAME).elf $(BUILDDIR)/$(PROJECT_NAME).hex

$(PROJECT_NAME).elf: $(C_OBJ) all_2
	$(AVR_CC) -g -mmcu=$(CPU) -o $(BUILDDIR)/$(PROJECT_NAME).elf $(DIR_C_OBJ)

# dependencies chain
all_2: $(objects)

$(C_OBJ): %.o: %.c
	$(AVR_CC) -c $(AVR_CCFLAGS) $< -o $(BUILDDIR)/$@

# Just to print the startup of the Makefile
startup:
	@mkdir -p $(BUILDDIR)
	@echo "\033[92m***** e-Yantra eYFi AVR Build System *****\033[0m"



