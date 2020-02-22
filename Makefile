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
MAKETXT  = srec_cat
CP       = cp -p
RM       = rm -r -f
MV       = mv
MKDIR    = mkdir -p

GCC_DIR       = $(HOME)/Programme/tools/msp430-gcc
GCC_MSP_INC_DIR = $(GCC_DIR)/include
GCC_INC_DIR = $(GCC_DIR)/msp430-elf/include

TARGETS = $(wildcard src/*)
MODULES = $(subst src/,,$(TARGETS))
BUILD_DIR = $(subst src/,build/,$(TARGETS))

# List all the source files here
# eg if you have a source file foo.c then list it here
SOURCES = $(shell find $(TARGETS) -name *.cpp -or -name *.c -or -name *.s)

# Include are located in the Include directory
INCLUDES = -Iinclude -I$(GCC_MSP_INC_DIR)

# OUTDIR: directory to use for output
OUTDIR = build

# list of object files, placed in the build directory regardless of source path
OBJECTS = $(SOURCES:src/%=$(OUTDIR)/%.o)

# Add or subtract whatever MSPGCC flags you want. There are plenty more
#######################################################################################
CFLAGS   = -mmcu=$(MCU) -g -gdwarf-3 -O1 -Wall -Wunused
CXXFLAGS = -mmcu=$(MCU) -nostartfiles -nostdlib -g -gdwarf-3 -O1 -Wall -Wunused -std=c++11
CPPFLAGS = $(INCLUDES) -MMD -MP 
ASFLAGS  = -mmcu=$(MCU) -x assembler-with-cpp -Wa,-gstabs
LDFLAGS  = -mmcu=$(MCU) -Wl,-Map=$(OUTDIR)/$(MODULES).map -L $(GCC_MSP_INC_DIR)

# default: build hex file and TI TXT file
.PHONY: all
all: $(BUILD_DIR) $(OUTDIR)/$(MODULES).hex #$(OUTDIR)/$(TARGET).txt

# TI TXT file
#$(OUTDIR)/%.txt: $(OUTDIR)/%.hex
#	$(MAKETXT) -O $@ -TITXT $< -I
#	$(UNIX2DOS) $(OUTDIR)/$(TARGET).txt

# intel hex file
$(OUTDIR)/$(MODULES).hex: $(OUTDIR)/$(MODULES).elf
	$(OBJCOPY) -O ihex $< $@

# elf file
$(OUTDIR)/$(MODULES).elf: $(OBJECTS)
	echo "Linking $@"
	$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $@
	echo
	echo ">>>> Size of Firmware <<<<"
	$(SIZE) $@
	echo
	
# search path for *.cpp files
vpath %.cpp $(TARGETS)
vpath %.c $(TARGETS)

define make-goal
# assembly
$(1)/%.s.o: %.s
	$(AS) $(ASFLAGS) -c $$< -o $$@
	
# c source
$(1)/%.c.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $$< -o $$@
	

# c++ source
$(1)/%.cpp.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $$< -o $$@
endef
	
# create the output directory
$(BUILD_DIR):
	$(MKDIR) $@

# remove build artifacts and executables
.PHONY: clean
clean:
	$(RM) $(OUTDIR)

$(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir))))