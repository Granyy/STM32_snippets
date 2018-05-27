TARGET=LoRaNode

SRC=$(shell find . -name '*.c')

CXXSRC=$(shell find . -name '*.cpp')

ASRC=startup/startup_stm32l432xx.s

INCDIRS=HAL_Driver/Inc/Legacy Utilities/STM32L4xx_Nucleo_32 inc CMSIS/device CMSIS/core HAL_Driver/Inc inc/Test inc/Utils inc/Mesh inc/Nanocapteur External

LDSCRIPT=LinkerScript.ld

CROSS=arm-none-eabi-
CC=$(CROSS)gcc
CXX=$(CROSS)g++
AS=$(CROSS)gcc
LD=$(CROSS)g++
OBJCOPY=$(CROSS)objcopy
SIZE=$(CROSS)size

OPENOCD=openocd
OPENOCD_DIR=../msys64/mingw64/share/openocd/scripts
OPENOCD_CFG=../msys64/mingw64/share/openocd/scripts/board/st_nucleo_l476rg.cfg 

ARCH_FLAGS=-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Wno-psabi

CFLAGS+=-O0 -g3
CFLAGS+=$(ARCH_FLAGS)
CFLAGS+=-ffunction-sections
CFLAGS+=-fdata-sections
CFLAGS+=-MMD
CFLAGS+=-DSTM32 -DSTM32L4 -DSTM32L432KCUx -DNUCLEO_L432KC -DDEBUG -DSTM32L432xx -DUSE_HAL_DRIVER
CFLAGS+=$(foreach i, $(INCDIRS), -I$(i))

CXXFLAGS=$(CFLAGS)
CXXFLAGS+=-fno-rtti -fno-exceptions
CXXFLAGS+=-std=c++11

LDFLAGS=$(ARCH_FLAGS)
LDFLAGS+=-Wl,--gc-sections
LDFLAGS+=$(foreach i, $(LIBDIRS), -L$(i))
LDFLAGS+=-T $(LDSCRIPT) -specs=nano.specs -lm

OBJS=$(SRC:.c=.o) $(ASRC:.s=.o) $(CXXSRC:.cpp=.o)

DEPS = $(SRCS:.c=.d) $(CXXSRC:.cpp=.d)
-include $(DEPS)

all: $(TARGET).elf bin size

%.o: %.c
	@echo " CC      $<"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.cpp
	@echo " CX      $<"
	@mkdir -p $(@D)
	@$(CXX)  $(CXXFLAGS) -o $@ -c $<

%.o: %.s
	@echo " AS      $<"
	@mkdir -p $(@D)
	@$(AS) $(CFLAGS) -o $@ -c $<

$(TARGET).elf: $(OBJS)
	@echo " LD       $@"
	@mkdir -p $(@D)
	@$(LD) -o $@ $(LDFLAGS) $(OBJS)

.PHONY: clean all bin

clean:
	@echo " CLEAN"
	@rm -fR $(OBJS) $(TARGET) $(TARGET:.elf=hex) $(TARGET:.elf=.bin)

bin: $(TARGET).elf
	@echo " OBJCOPY  $(TARGET:.elf=.hex)"
	$(OBJCOPY) -O ihex $(TARGET).elf $(TARGET).hex
	@echo " OBJCOPY  $(TARGET:.elf=.bin)"
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

size: $(TARGET).elf
	@echo " SIZE     $(TARGET)"
	@$(SIZE) --format=berkeley $(TARGET).elf

flash:
	$(OPENOCD) -s $(OPENOCD_DIR) -f $(OPENOCD_CFG) -c "init;reset halt;flash write_image erase $(TARGET).bin 0x08000000;reset;shutdown"
	
