library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity hfrisc_soc is
	generic(
		address_width: integer := 15;
		memory_file : string := "code.txt"
	);
	port (	clk_i:		in std_logic;
		rst_i:		in std_logic;

		gpioa_in:	in std_logic_vector(15 downto 0);
		gpioa_out:	out std_logic_vector(15 downto 0);
		gpioa_ddr:	out std_logic_vector(15 downto 0);
		gpiob_in:	in std_logic_vector(15 downto 0);
		gpiob_out:	out std_logic_vector(15 downto 0);
		gpiob_ddr:	out std_logic_vector(15 downto 0)
	);
end hfrisc_soc;

architecture top_level of hfrisc_soc is
	signal clock, boot_enable, ram_enable_n, stall, ram_dly, rff1, reset: std_logic;
	signal address, data_read, data_write, data_read_boot, data_read_ram: std_logic_vector(31 downto 0);
	signal ext_irq: std_logic_vector(7 downto 0);
	signal data_we, data_w_n_ram: std_logic_vector(3 downto 0);

	signal periph, periph_dly, periph_wr, periph_irq: std_logic;
	signal data_read_periph, data_read_periph_s, data_write_periph: std_logic_vector(31 downto 0);
	
	signal ext_periph, ext_periph_dly, ready: std_logic;
	signal key: std_logic_vector(127 downto 0);
	signal input, output: std_logic_vector(63 downto 0);
	signal data_read_xtea, data_read_xtea_s: std_logic_vector(31 downto 0);
	signal control: std_logic_vector(1 downto 0);
begin
	-- clock divider (25MHz clock from 50MHz main clock for Spartan3 Starter Kit)
	process (rst_i, clk_i, clock)
	begin
		if rst_i = '1' then
			clock <= '0';
		else
			if clk_i'event and clk_i = '1' then
				clock <= not clock;
			end if;
		end if;
	end process;

	-- reset synchronizer
	process (clock, rst_i)
	begin
		if (rst_i = '1') then
			rff1 <= '1';
			reset <= '1';
		elsif (clock'event and clock = '1') then
			rff1 <= '0';
			reset <= rff1;
		end if;
	end process;


	process (reset, clock, ext_irq, ram_enable_n)
	begin
		if reset = '1' then
			ram_dly <= '0';
			periph_dly <= '0';
			ext_periph_dly <= '0';
		elsif clock'event and clock = '1' then
			ram_dly <= not ram_enable_n;
			periph_dly <= periph;
			ext_periph_dly <= ext_periph;
		end if;
	end process;

	stall <= '0';
	boot_enable <= '1' when address(31 downto 28) = "0000" else '0';
	ram_enable_n <= '0' when address(31 downto 28) = "0100" else '1';
	data_read <= data_read_xtea when ext_periph = '1' or ext_periph_dly = '1' else data_read_periph when periph = '1' or periph_dly = '1' else data_read_boot when address(31 downto 28) = "0000" and ram_dly = '0' else data_read_ram;
	data_w_n_ram <= not data_we;
	ext_irq <= "0000000" & periph_irq;

	-- HF-RISCV core
	processor: entity work.processor
	port map(	clk_i => clock,
			rst_i => reset,
			stall_i => stall,
			addr_o => address,
			data_i => data_read,
			data_o => data_write,
			data_w_o => data_we,
			data_mode_o => open,
			extio_in => ext_irq,
			extio_out => open
	);

	data_read_periph <= data_read_periph_s(7 downto 0) & data_read_periph_s(15 downto 8) & data_read_periph_s(23 downto 16) & data_read_periph_s(31 downto 24);
	data_write_periph <= data_write(7 downto 0) & data_write(15 downto 8) & data_write(23 downto 16) & data_write(31 downto 24);
	periph_wr <= '1' when data_we /= "0000" else '0';
	periph <= '1' when address(31 downto 24) = x"e1" else '0';

	peripherals: entity work.peripherals
	port map(
		clk_i => clock,
		rst_i => reset,
		addr_i => address,
		data_i => data_write_periph,
		data_o => data_read_periph_s,
		sel_i => periph,
		wr_i => periph_wr,
		irq_o => periph_irq,
		gpioa_in => gpioa_in,
		gpioa_out => gpioa_out,
		gpioa_ddr => gpioa_ddr,
		gpiob_in => gpiob_in,
		gpiob_out => gpiob_out,
		gpiob_ddr => gpiob_ddr
	);
	
	data_read_xtea <= data_read_xtea_s(7 downto 0) & data_read_xtea_s(15 downto 8) & data_read_xtea_s(23 downto 16) & data_read_xtea_s(31 downto 24);
	ext_periph <= '1' when address(31 downto 24) = x"e7" else '0';

	process (clock, reset, address, key, input, output)
	begin
		if reset = '1' then
			data_read_xtea_s <= (others => '0');
		elsif clock'event and clock = '1' then
			if (ext_periph = '1') then	-- XTEA is at 0xe7000000
				case address(7 downto 4) is
					when "0000" =>		-- control	0xe7000000	(bit2 - ready (R), bit1 - encrypt (RW), bit0 - start (RW)
						data_read_xtea_s <= x"000000" & "00000" & ready & control;
					when "0001" =>		-- key[0]	0xe7000010
						data_read_xtea_s <= key(127 downto 96);
					when "0010" =>		-- key[1]	0xe7000020
						data_read_xtea_s <= key(95 downto 64);
					when "0011" =>		-- key[2]	0xe7000030
						data_read_xtea_s <= key(63 downto 32);
					when "0100" =>		-- key[3]	0xe7000040
						data_read_xtea_s <= key(31 downto 0);
					when "0101" =>		-- input[0]	0xe7000050
						data_read_xtea_s <= input(63 downto 32);
					when "0110" =>		-- input[1]	0xe7000060
						data_read_xtea_s <= input(31 downto 0);
					when "0111" =>		-- output[0]	0xe7000070
						data_read_xtea_s <= output(63 downto 32);
					when "1000" =>		-- output[1]	0xe7000080
						data_read_xtea_s <= output(31 downto 0);
					when others =>
						data_read_xtea_s <= (others => '0');
				end case;
			end if;
		end if;
	end process;

	process (clock, reset, address, control, key, input, output)
	begin
		if reset = '1' then
			key <= (others => '0');
			input <= (others => '0');
			control <= "00";
		elsif clock'event and clock = '1' then
			if (ext_periph = '1' and data_we /= "0000") then	-- XTEA is at 0xe7000000
				case address(7 downto 4) is
					when "0000" =>		-- control	0xe7000000	(bit2 - ready (R), bit1 - encrypt (RW), bit0 - start (RW)
						control <= data_write_periph(1 downto 0);
					when "0001" =>		-- key[0]	0xe7000010
						key(127 downto 96) <= data_write_periph;
					when "0010" =>		-- key[1]	0xe7000020
						key(95 downto 64) <= data_write_periph;
					when "0011" =>		-- key[2]	0xe7000030
						key(63 downto 32) <= data_write_periph;
					when "0100" =>		-- key[3]	0xe7000040
						key(31 downto 0) <= data_write_periph;
					when "0101" =>		-- input[0]	0xe7000050
						input(63 downto 32) <= data_write_periph;
					when "0110" =>		-- input[1]	0xe7000060
						input(31 downto 0) <= data_write_periph;
					when others =>
				end case;
			end if;
		end if;
	end process;

	-- XTEA core
	crypto_core: entity work.xtea
	port map(	clock => clock,
			reset => reset,
			start => control(0),
			encrypt => control(1),
			key => key,
			input => input,
			output => output,
			ready => ready
	);

	-- instruction and data memory (boot RAM)
	boot_ram: entity work.ram
	generic map (memory_type => "DEFAULT")
	port map (
		clk			=> clock,
		enable			=> boot_enable,
		write_byte_enable	=> "0000",
		address			=> address(31 downto 2),
		data_write		=> (others => '0'),
		data_read		=> data_read_boot
	);

	-- instruction and data memory (external RAM)
	memory0lb: entity work.bram
	generic map (	memory_file => memory_file,
					data_width => 8,
					address_width => address_width,
					bank => 0)
	port map(
		clk 	=> clock,
		addr 	=> address(address_width -1 downto 2),
		cs_n 	=> ram_enable_n,
		we_n	=> data_w_n_ram(0),
		data_i	=> data_write(7 downto 0),
		data_o	=> data_read_ram(7 downto 0)
	);

	memory0ub: entity work.bram
	generic map (	memory_file => memory_file,
					data_width => 8,
					address_width => address_width,
					bank => 1)
	port map(
		clk 	=> clock,
		addr 	=> address(address_width -1 downto 2),
		cs_n 	=> ram_enable_n,
		we_n	=> data_w_n_ram(1),
		data_i	=> data_write(15 downto 8),
		data_o	=> data_read_ram(15 downto 8)
	);

	memory1lb: entity work.bram
	generic map (	memory_file => memory_file,
					data_width => 8,
					address_width => address_width,
					bank => 2)
	port map(
		clk 	=> clock,
		addr 	=> address(address_width -1 downto 2),
		cs_n 	=> ram_enable_n,
		we_n	=> data_w_n_ram(2),
		data_i	=> data_write(23 downto 16),
		data_o	=> data_read_ram(23 downto 16)
	);

	memory1ub: entity work.bram
	generic map (	memory_file => memory_file,
					data_width => 8,
					address_width => address_width,
					bank => 3)
	port map(
		clk 	=> clock,
		addr 	=> address(address_width -1 downto 2),
		cs_n 	=> ram_enable_n,
		we_n	=> data_w_n_ram(3),
		data_i	=> data_write(31 downto 24),
		data_o	=> data_read_ram(31 downto 24)
	);

end top_level;

