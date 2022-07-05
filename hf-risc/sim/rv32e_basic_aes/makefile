# to install ghdl and gtkwave:
#
# sudo apt install gnat
# git clone https://github.com/ghdl/ghdl.git
# cd ghdl-master
# ./configure --prefix=/usr/local
# make
# sudo make install
# sudo apt install gtkwave

TOP = tb

VHDL_BASE = ../..
SRC_BASE = ../..

VHD_SRC = \
	$(VHDL_BASE)/riscv/core_rv32e/bshifter.vhd \
	$(VHDL_BASE)/riscv/core_rv32e/alu.vhd \
	$(VHDL_BASE)/riscv/core_rv32e/reg_bank.vhd \
	$(VHDL_BASE)/riscv/core_rv32e/control.vhd \
	$(VHDL_BASE)/riscv/core_rv32e/datapath.vhd \
	$(VHDL_BASE)/riscv/core_rv32e/int_control.vhd \
	$(VHDL_BASE)/riscv/core_rv32e/cpu.vhd \
	$(VHDL_BASE)/devices/controllers/uart/uart.vhd \
	$(VHDL_BASE)/devices/peripherals/basic_soc.vhd \
	$(VHDL_BASE)/riscv/sim/boot_ram.vhd \
	$(VHDL_BASE)/riscv/sim/ram.vhd \
	$(VHDL_BASE)/devices/controllers/aes/bram_block_a.vhdl \
	$(VHDL_BASE)/devices/controllers/aes/bram_block_b.vhdl \
	$(VHDL_BASE)/devices/controllers/aes/counter2bit.vhdl \
	$(VHDL_BASE)/devices/controllers/aes/folded_register.vhdl \
	$(VHDL_BASE)/devices/controllers/aes/io_interface.vhdl \
	$(VHDL_BASE)/devices/controllers/aes/key_scheduler.vhdl \
	$(VHDL_BASE)/devices/controllers/aes/mix_column.vhdl \
	$(VHDL_BASE)/devices/controllers/aes/xtime.vhdl \
	$(VHDL_BASE)/devices/controllers/aes/mini_aes.vhdl \
	$(VHDL_BASE)/riscv/sim/hf-riscv_basic_soc_aes_tb.vhd

PROG_SRC = \
	$(SRC_BASE)/riscv/sim/boot.txt \
	$(SRC_BASE)/software/code.txt

all:

# Command line simulation using the free ghdl
# run simulation with 'make ghdl time=10ms' to run for 10ms
ghdl:
	@ cp $(PROG_SRC) .
	@ mkdir -p simu
	@ ghdl -i --ieee=synopsys --warn-no-vital-generic --workdir=simu --work=work -fexplicit $(VHD_SRC)
	@ ghdl -r --ieee=synopsys --warn-no-vital-generic --workdir=simu --work=work -fexplicit $(TOP) --assert-level=failure --stop-time=$(time) --vcdgz=$(TOP).vcdgz

ghdl-gui:
	@ gunzip --stdout $(TOP).vcdgz | gtkwave --vcd

clean-ghdl:
	@ rm -rf simu
	@ rm -f tb
	@ rm -f tb.vcdgz
	@ rm -f *.txt

# Clean all generated files
clean: clean-ghdl
	@ rm -f debug.txt
