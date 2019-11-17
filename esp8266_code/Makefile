.PHONY: all user1
all: user1

SDK = $(HOME)/ESP/sdk
CC = xtensa-lx106-elf-gcc
AR = xtensa-lx106-elf-ar
OBJCOPY = xtensa-lx106-elf-objcopy
NM = xtensa-lx106-elf-nm

APP = 1
FLASH_MODE = 0
FLASH_CLKDIV = 0
FLASH_SIZE_MAP = 6

CPPFLAGS = \
	-I$(SDK)/include \
	-I$(SDK)/third_party/include \
	-Iinclude \
	-I.  \
	-DLWIP_OPEN_SRC \
	-DICACHE_FLASH \
	-DUSE_OPTIMIZE_PRINTF \
	-DSPI_FLASH_SIZE_MAP=6 \
	 

CFLAGS = \
	-g \
	-Os \
	$(CPPFLAGS) \
	-Wpointer-arith \
	-Wundef \
	-Werror \
	-Wl,-EL \
	-fno-inline-functions \
	-nostdlib \
	-mlongcalls \
	-mtext-section-literals \
	-ffunction-sections \
	-fdata-sections \
	-fno-builtin-printf

SYSLIBS = \
	-lc \
	-lgcc \
	-lhal \
	-lphy \
	-lpp \
	-lnet80211 \
	-llwip \
	-lwpa \
	-lwpa2 \
	-lcrypto \
	-lmain \
	-ljson \
	-lupgrade \
	-lmbedtls \
	-lwps \
	-lsmartconfig \
	-lairkiss

LDFLAGS = \
	-nostdlib \
	-L$(SDK)/lib \
	-T../ld/eagle.app.v6.new.2048.ld \
	-Wl,--no-check-sections \
	-Wl,--gc-sections \
	-u call_user_start \
	-Wl,-static -Wl,--start-group $(SYSLIBS) $(OBJS) -Wl,--end-group

SRC = udp_test.c

OUT = output

OBJS = $(SRC:%.c=$(OUT)/%.o)

ELF = gate_alarm.app.v6.out
OUT_ELF = $(OUT)/$(ELF)
OUT_FLASHBIN = $(OUT)/gate_alarm.user1.app.flash.bin

$(OUT):
	mkdir -p $(OUT)

$(OBJS): | $(OUT)

user1: $(OUT_FLASHBIN)

$(OUT_FLASHBIN): $(OUT_ELF)
	$(OBJCOPY) --only-section .text -O binary $< $(OUT)/eagle.app.v6.text.bin
	$(OBJCOPY) --only-section .data -O binary $< $(OUT)/eagle.app.v6.data.bin
	$(OBJCOPY) --only-section .rodata -O binary $< $(OUT)/eagle.app.v6.rodata.bin
	$(OBJCOPY) --only-section .irom0.text -O binary $< $(OUT)/eagle.app.v6.irom0text.bin	
	cd $(OUT) && \
		COMPILE=gcc python $(SDK)/tools/gen_appbin.py $(ELF) 2 $(FLASH_MODE) $(FLASH_CLKDIV) $(FLASH_SIZE_MAP) $(APP)
	mv $(OUT)/eagle.app.flash.bin $(OUT_FLASHBIN)

$(OUT_ELF): $(OBJS)
	$(CC) $(LDFLAGS) -o $(OUT_ELF)

$(OUT)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

.PHONY: clean
clean:
	rm -rf $(OUT)
