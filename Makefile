#
# Makefile for msp430
#
# 'make' builds everything
# 'make clean' deletes everything except source files and Makefile
# You need to set TARGET, MCU and SOURCES for your project.
# TARGET is the name of the executable file to be produced 
# $(TARGET).elf $(TARGET).hex and $(TARGET).txt nad $(TARGET).map are all generated.
# The TXT file is used for BSL loading, the ELF can be used for JTAG use
# 

MCU        = msp430g2553
TOOL_PREFIX = msp430-elf-
CC       = $(TOOL_PREFIX)gcc
CXX      = $(TOOL_PREFIX)g++
LD       = $(TOOL_PREFIX)ld
AR       = $(TOOL_PREFIX)ar
AS       = $(TOOL_PREFIX)gcc
GASP     = $(TOOL_PREFIX)gasp
NM       = $(TOOL_PREFIX)nm
OBJCOPY  = $(TOOL_PREFIX)objcopy
RANLIB   = $(TOOL_PREFIX)ranlib
STRIP    = $(TOOL_PREFIX)strip
SIZE     = $(TOOL_PREFIX)size
READELF  = $(TOOL_PREFIX)readelf
GDB      = $(TOOL_PREFIX)gdb
MSPDEBUG = mspdebug
MAKETXT  = srec_cat
CP       = cp -p
RM       = rm -r -f
MV       = mv
MKDIR    = mkdir -p

GCC_DIR       = $(HOME)/Software/toolchain/msp430-gcc
GCC_MSP_INC_DIR = $(GCC_DIR)/include
GCC_INC_DIR = $(GCC_DIR)/msp430-elf/include

TARGET = app
# OUTDIR: directory to use for output
OUTDIR = build

SRC_DIR = src
BIN_DIR = $(OUTDIR)/bin
OBJ_DIR = $(OUTDIR)/obj

# List all the source files here
# eg if you have a source file foo.c then list it here
SOURCES := $(shell find $(SRC_DIR) -name *.cpp -or -name *.c -or -name *.s)

MODULES := $(shell find $(SRC_DIR) -type d -print)
MODULE_OBJ_PATH := $(MODULES:$(SRC_DIR)%=$(OBJ_DIR)%)


# Include are located in the Include directory
INCLUDES = -I$(GCC_MSP_INC_DIR) $(MODULES:%=-I%)

# list of object files, placed in the build directory regardless of source path
OBJECTS := $(SOURCES:$(SRC_DIR)/%=$(OBJ_DIR)/%.o)

# Add or subtract whatever MSPGCC flags you want. There are plenty more
#######################################################################################
ifeq ($(PROFILE),release)
OPT=-O1
else
OPT=-g3 -gdwarf-2 -O0
endif
CFLAGS   = -mmcu=$(MCU) $(OPT) -Wall -Wextra -Werror -std=c11
CXXFLAGS = -mmcu=$(MCU) -nostartfiles -nostdlib $(OPT) -Wall -Wextra -Werror -std=c++11
CPPFLAGS = $(INCLUDES) #-MMD -MP 
ASFLAGS  = -mmcu=$(MCU) -x assembler-with-cpp -Wa,-gstabs
LDFLAGS  = -mmcu=$(MCU) -Wl,-Map=$(BIN_DIR)/$(TARGET).map -L $(GCC_MSP_INC_DIR)

# default: build hex file and TI TXT file
.PHONY: all
all: setup $(BIN_DIR)/$(TARGET).hex #$(OUTDIR)/$(TARGET).txt

# create the output directory
setup:
	$(MKDIR) $(BIN_DIR) $(OBJ_DIR) $(MODULE_OBJ_PATH)
	
# TI TXT file
#$(OUTDIR)/%.txt: $(OUTDIR)/%.hex
#	$(MAKETXT) -O $@ -TITXT $< -I
#	$(UNIX2DOS) $(OUTDIR)/$(TARGET).txt

# intel hex file
$(BIN_DIR)/$(TARGET).hex: $(BIN_DIR)/$(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

# elf file
$(BIN_DIR)/$(TARGET).elf: $(OBJECTS)
	echo "Linking $@"
	$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $@
	echo
	echo ">>>> Size of Firmware <<<<"
	$(SIZE) $@
	echo
	
# c source
$(OBJ_DIR)/%.c.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
	
# c++ source
$(OBJ_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


# assembly
$(OBJ_DIR)/%.s.o: $(SRC_DIR)/%.s
	$(AS) $(ASFLAGS) -c $< -o $@
	
# remove build artifacts and executables
.PHONY: clean
clean:
	$(RM) $(OUTDIR)

