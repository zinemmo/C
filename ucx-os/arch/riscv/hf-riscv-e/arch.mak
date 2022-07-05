# this is stuff specific to this architecture
ARCH_DIR = $(SRC_DIR)/arch/$(ARCH)
INC_DIRS  = -I $(ARCH_DIR)

F_CLK=25000000

#remove unreferenced functions
CFLAGS_STRIP = -fdata-sections -ffunction-sections
LDFLAGS_STRIP = --gc-sections

# this is stuff used everywhere - compiler and flags should be declared (ASFLAGS, CFLAGS, LDFLAGS, LD_SCRIPT, CC, AS, LD, DUMP, READ, OBJ and SIZE).
ASFLAGS = -march=rv32e -mabi=ilp32e #-fPIC
CFLAGS = -Wall -march=rv32e -mabi=ilp32e -O2 -c -mstrict-align -ffreestanding -nostdlib -ffixed-a5 -fomit-frame-pointer $(INC_DIRS) -DCPU_SPEED=${F_CLK} -DLITTLE_ENDIAN $(CFLAGS_STRIP) #-mrvc -fPIC -DDEBUG_PORT

LDFLAGS = -melf32lriscv $(LDFLAGS_STRIP)
LDSCRIPT = $(ARCH_DIR)/hf-risc.ld

CC = riscv32-unknown-elf-gcc
AS = riscv32-unknown-elf-as
LD = riscv32-unknown-elf-ld
DUMP = riscv32-unknown-elf-objdump -Mno-aliases
READ = riscv32-unknown-elf-readelf
OBJ = riscv32-unknown-elf-objcopy
SIZE = riscv32-unknown-elf-size

hal:
	$(AS) $(ASFLAGS) -o crt0.o $(ARCH_DIR)/crt0.s
	$(CC) $(CFLAGS) \
		$(ARCH_DIR)/hal.c \
		$(ARCH_DIR)/interrupt.c
