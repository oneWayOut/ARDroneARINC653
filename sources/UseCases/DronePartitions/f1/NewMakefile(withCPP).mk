
#Makefile.arm-linux-toolchain
PREFIX ?= arm-linux-gnueabi

CC   = $(PREFIX)-gcc
LD   = $(PREFIX)-gcc

## useless below
AR   = $(PREFIX)-ar
CP   = $(PREFIX)-objcopy
DMP  = $(PREFIX)-objdump
NM   = $(PREFIX)-nm
SIZE = $(PREFIX)-size
GDB  = $(PREFIX)-gdb
## useless above

#Makefile.arm-linux
FLOAT_ABI ?= -mfloat-abi=softfp -mfpu=vfp
ARCH_CFLAGS ?= -mtune=cortex-a8 -march=armv7-a

# with armv7, unaligned data can still be read
#CFLAGS += -DPPRZLINK_UNALIGNED_ACCESS=1

# add ARM specifc flags to CFLAGS, LDFLAGS
CFLAGS += $(FLOAT_ABI) $(ARCH_CFLAGS)
LDFLAGS += $(FLOAT_ABI)
CXXFLAGS += $(FLOAT_ABI) $(ARCH_CFLAGS)


#Makefile.linux
CC  ?= gcc
CXX ?= g++
LD  ?= $(CC)



# Launch with "make Q=''" to get full command display
Q=@

OPT ?= 3
# Slightly bigger .elf files but gains the ability to decode macros
DEBUG_FLAGS ?= -ggdb3
CSTANDARD ?= -std=gnu99
CINCS = $(INCLUDES) -I$(PAPARAZZI_SRC)/sw/include

#
# Common compiler flags.
# add then to arch specific CFLAGS already defined in e.g. Makefile.arm-linux
#
CFLAGS += $(CINCS)
CFLAGS += -O$(OPT) -fPIC
CFLAGS += $(DEBUG_FLAGS)
CFLAGS += -fno-short-enums
# CFLAGS += -malignment-traps
CFLAGS += -Wall -Wcast-qual -Wimplicit -Wcast-align
CFLAGS += -Wpointer-arith -Wswitch
CFLAGS += -Wredundant-decls -Wreturn-type -Wshadow -Wunused
#-Wno-unused-result
#CFLAGS += -Wa,-adhlns=$(OBJDIR)/$(notdir $(subst $(suffix $<),.lst,$<))
#CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))

# flags only for C
CFLAGS += -Wstrict-prototypes -Wmissing-declarations
CFLAGS += -Wmissing-prototypes -Wnested-externs
CFLAGS += $(CSTANDARD)
CFLAGS += $($(TARGET).CFLAGS)
CFLAGS += $(USER_CFLAGS)

CXXFLAGS += -O$(OPT) -fPIC
CXXFLAGS += $(DEBUG_FLAGS)
CXXFLAGS += -pipe -fshow-column -ffast-math
CXXFLAGS += -g -ffunction-sections -fdata-sections
CXXFLAGS += -Wall -Wextra
CXXFLAGS += $($(TARGET).CXXFLAGS)
CXXFLAGS += $(USER_CFLAGS)

LDFLAGS	+= $($(TARGET).LDFLAGS) -lm -pthread

$(TARGET).srcsnd = $(notdir $($(TARGET).srcs))
$(TARGET).objso	= $($(TARGET).srcs:%.c=$(OBJDIR)/%.o)
$(TARGET).objsoxx = $($(TARGET).objso:%.cpp=$(OBJDIR)/%.o)
$(TARGET).objs	= $($(TARGET).objsoxx:%.S=$(OBJDIR)/%.o)


printcommands:
	@echo ""
	@echo "Using CC   = $(CC)"
#	@echo "Using LD   = $(LD)"
	@echo "GCC version:"
	@$(CC) --version | head -1
	@echo ""

all: printcommands build

# depend order only for parallel make
build: | printcommands

build: $(OBJDIR) elf

$(OBJDIR):
	@echo CREATING object dir $(OBJDIR)
	@test -d $(OBJDIR) || mkdir -p $(OBJDIR)

elf: $(OBJDIR)/$(TARGET).elf

# Link: create ELF output file from object files.
.SECONDARY : $(OBJDIR)/$(TARGET).elf
.PRECIOUS :   $(OBJ_C_LINUX) $(OBJ_CPP_LINUX)
%.elf: $($(TARGET).objs)
	@echo LD $@
	$(Q)$(LD) -o $@ $($(TARGET).objs) $(LDFLAGS)

# Compile: create object files from C source files
$(OBJDIR)/%.o : %.c $(OBJDIR)/../Makefile.ac
	@echo CC $@
	$(Q)test -d $(dir $@) || mkdir -p $(dir $@)
	$(Q)$(CC) -MMD -c $(CFLAGS) $< -o $@

# Compile: create object files from C++ source files
$(OBJDIR)/%.o : %.cpp $(OBJDIR)/../Makefile.ac
	@echo CXX $@
	$(Q)test -d $(dir $@) || mkdir -p $(dir $@)
	$(Q)$(CXX) -MMD -c $(CXXFLAGS) $< -o $@


# Listing of phony targets.
.PHONY : printcommands all build elf

#
# Dependencies
#
ifneq ($(MAKECMDGOALS),clean)
DEPS = $(addprefix $(OBJDIR)/,$($(TARGET).srcs:.c=.d))
DEPS += $(addprefix $(OBJDIR)/,$($(TARGET).srcs:.cpp=.d))
-include $(DEPS)
endif
