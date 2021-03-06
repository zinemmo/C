library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_textio.all;
use ieee.std_logic_unsigned.all;
use std.textio.all;
use ieee.numeric_std.all;

entity tb is
	generic(
		address_width: integer := 15;
		memory_file : string := "code.txt";
		log_file: string := "out.txt";
		uart_support : string := "no"
	);
end tb;

architecture tb of tb is
	signal clock_in, reset, data, stall, stall_sig: std_logic := '0';
	signal uart_read, uart_write: std_logic;
	signal boot_enable_n, ram_enable_n, ram_dly: std_logic;
	signal address, data_read, data_write, data_read_boot, data_read_ram: std_logic_vector(31 downto 0);
	signal ext_irq: std_logic_vector(7 downto 0);
	signal data_we, data_w_n_ram: std_logic_vector(3 downto 0);

	signal periph, periph_dly, periph_wr, periph_irq: std_logic;
	signal data_read_periph, data_read_periph_s, data_write_periph: std_logic_vector(31 downto 0);
	signal gpioa_in, gpioa_out, gpioa_ddr: std_logic_vector(15 downto 0);
	signal gpiob_in, gpiob_out, gpiob_ddr: std_logic_vector(15 downto 0);
	signal gpio_sig, gpio_sig2, gpio_sig3: std_logic := '0';
	
	signal ext_periph, ext_periph_dly, ready: std_logic;
	signal key: std_logic_vector(127 downto 0) := x"00000000000000000000000000000000";
	signal input, output: std_logic_vector(127 downto 0) := x"00000000000000000000000000000000";
	signal data_read_xtea, data_read_xtea_s: std_logic_vector(31 downto 0);
	signal control: std_logic := 'U';

	-- AES SIGNALS
	signal clear  :  std_logic := '1';
	signal load_i :  std_logic := '0';
	signal enc    :  std_logic := '0';  -- active low (e.g. 0 = encrypt, 1 = decrypt)
	signal key_i  :  std_logic_vector (7 downto 0) := x"00";
	signal key_i_driver : std_logic_vector(31 downto 0) := x"00000000";
	signal key_finished	:	std_logic := '0';
	signal data_i_aes :  std_logic_vector (7 downto 0);
	signal data_i_driver : std_logic_vector(31 downto 0) := x"00000000";
	signal data_o_driver : std_logic_vector(31 downto 0) := x"00000000";
	signal data_finished, output_finished	:	std_logic := '0';
	signal data_o :  std_logic_vector (7 downto 0);
	signal done_o :  std_logic;
	signal done_aes	:	std_logic := '0';
	signal data_read_aes, data_read_aes_s : std_logic_vector (31 downto 0) := x"00000000"; 
  signal driver_has_read : std_logic := '0';
	-- signal data_read_aes_s : std_logic_vector (127 downto 0);

	-- AES RECEIVE HELPERS
	signal rec_key_counter, rec_data_counter : integer := 127;
	-- AES SEND HELPERS
	signal sen_aes_counter : integer := 127;
	-- AES OUTPUT HELPERS
	signal out_aes_counter : integer := 127;
	signal process_output : std_logic := '0';
	-- PARSER SIGNALS
	signal parser_key, parser_input, parser_output : integer := 0;
	signal begin_key, begin_input, begin_output : integer := 127;
	signal end_key, end_input, end_output : integer := 96;
begin

	process						--25Mhz system clock
	begin
		clock_in <= not clock_in;
		wait for 20 ns;
		clock_in <= not clock_in;
		wait for 20 ns;
	end process;

	process
	begin
		wait for 4 ms;
		gpio_sig <= not gpio_sig;
		gpio_sig2 <= not gpio_sig2;
		wait for 100 us;
		gpio_sig <= not gpio_sig;
		gpio_sig2 <= not gpio_sig2;
	end process;

	process
	begin
		wait for 5 ms;
		gpio_sig3 <= not gpio_sig3;
		wait for 5 ms;
		gpio_sig3 <= not gpio_sig3;
	end process;

	gpioa_in <= x"00" & "0000" & gpio_sig & "000";
	gpiob_in <= "10000" & gpio_sig3 & "00" & "00000" & gpio_sig2 & "00";

	process
	begin
		stall <= not stall;
		wait for 123 ns;
		stall <= not stall;
		wait for 123 ns;
	end process;

	reset <= '0', '1' after 5 ns, '0' after 500 ns;
	stall_sig <= '0'; --stall;
	ext_irq <= "0000000" & periph_irq;

	boot_enable_n <= '0' when (address(31 downto 28) = "0000" and stall_sig = '0') or reset = '1' else '1';
	ram_enable_n <= '0' when (address(31 downto 28) = "0100" and stall_sig = '0') or reset = '1' else '1';
	data_read <= data_read_aes when ext_periph = '1' or ext_periph_dly = '1' else data_read_periph when periph = '1' or periph_dly = '1' else data_read_boot when address(31 downto 28) = "0000" and ram_dly = '0' else data_read_ram;
	data_w_n_ram <= not data_we;

	process(clock_in, reset)
	begin
		if reset = '1' then
			ram_dly <= '0';
			periph_dly <= '0';
			ext_periph_dly <= '0';
		elsif clock_in'event and clock_in = '1' then
			ram_dly <= not ram_enable_n;
			periph_dly <= periph;
			ext_periph_dly <= ext_periph;
		end if;
	end process;

	-- HF-RISCV core
	processor: entity work.processor
	port map(	clk_i => clock_in,
			rst_i => reset,
			stall_i => stall_sig,
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
		clk_i => clock_in,
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
	
	data_read_aes <= data_read_aes_s(7 downto 0) & data_read_aes_s(15 downto 8) & data_read_aes_s(23 downto 16) & data_read_aes_s(31 downto 24);
	ext_periph <= '1' when address(31 downto 24) = x"e7" else '0';
	
	read_proc: process (clock_in, reset, address, key_i_driver, data_i_driver, data_o_driver, done_aes)
	begin
		if reset = '1' then
			data_read_aes_s <= (others => '0');
      begin_output <= 127;
		elsif clock_in'event and clock_in = '1' then
			if (ext_periph = '1') then	-- AES is at 0xe7000000
        case address(7 downto 4) is
          when "0000" =>		-- control	0xe7000000	(bit2 - ready (R), bit1 - encrypt (RW), bit0 - start (RW)
            if done_aes = '1' then
              driver_has_read <= '1';
            else
              driver_has_read <= '0';
            end if;
            data_read_aes_s <= x"0000000" & "00" & done_aes & control;
					when "0001" =>
						data_read_aes_s <=  key_i_driver;
					when "0010" =>		-- input	0xe7000020
						data_read_aes_s <= data_i_driver;
					when "0011" =>		-- output	0xe7000030
						data_read_aes_s <= output(begin_output downto begin_output-31);
						begin_output <= begin_output - 32;
						if begin_output = 31 then
							begin_output <= 127;
						end if;
					when others =>
						data_read_aes_s <= (others => '0');
				end case;
			end if;
		end if;
	end process;

	write_proc: process (clock_in, reset, address)
	begin
		if reset = '1' then
			key_i_driver <= (others => '0');
			data_i_driver <= (others => '0');
			enc <= '0';
			control <= '0';
		elsif clock_in'event and clock_in = '1' then
			if (ext_periph = '1' and data_we /= "0000") then	-- AES is at 0xe7000000
				case address(7 downto 4) is
					when "0000" =>		-- control	0xe7000000	(bit1 - encrypt (RW), bit0 - start (RW))
						control <= data_write_periph(0);
						enc <= data_write_periph(0);
					when "0001" =>		-- key 32 bits	0xe7000010
						key_i_driver <= data_write_periph(31 downto 0);
					when "0010" =>		-- data 32 bits	0xe7000020
						data_i_driver <= data_write_periph(31 downto 0);
					when others =>
				end case;
			end if;
		end if;
	end process;

	parse_key_proc: process (reset, key_i_driver, load_i)
	begin
		if load_i = '1' then
			key_finished <= '0';
		end if;

		if reset = '1' then
			rec_key_counter <= 127;
			key <= (others => '0');
			key_finished <= '0';
		elsif reset = '0' and key_i_driver'event then
			key(rec_key_counter downto rec_key_counter-31) <= key_i_driver;
			rec_key_counter <= rec_key_counter - 32;
			if(rec_key_counter = 31) then
				rec_key_counter <= 127;
				key_finished <= '1';
			else
				key_finished <= '0';
			end if;
		end if;
	end process;

	parse_input_proc: process (reset, data_i_driver, load_i)
	begin
		if load_i = '1' then
			data_finished <= '0';
		end if;
		
		if reset = '1' then
			rec_data_counter <= 127;
			input <= (others => '0');
			data_finished <= '0';
		elsif reset = '0' and data_i_driver'event then
			input(rec_data_counter downto rec_data_counter-31) <= data_i_driver;
			rec_data_counter <= rec_data_counter - 32;
			if(rec_data_counter = 31) then
				rec_data_counter <= 127;
				data_finished <= '1';
			else
				data_finished <= '0';
			end if;
		end if;
	end process;

	load_i_proc: process (clock_in, key_finished, data_finished, done_o, output_finished, driver_has_read)
	begin 
		if clock_in'event and clock_in = '1' then
			if load_i = '1' then
				if sen_aes_counter > 0 then
				key_i <= key(sen_aes_counter downto sen_aes_counter-7);
				data_i_aes <= input(sen_aes_counter downto sen_aes_counter-7);
				sen_aes_counter <= sen_aes_counter - 8;
				else
					sen_aes_counter <= 127;
					load_i <= '0';
				end if;
			else
				if key_finished = '1' and data_finished = '1' and done_o /= '1' then
					load_i <= '1';
					clear <= '0';
					key_i <= key(sen_aes_counter downto sen_aes_counter-7);
					data_i_aes <= input(sen_aes_counter downto sen_aes_counter-7);
					sen_aes_counter <= sen_aes_counter - 8;
				elsif output_finished = '1' and done_o = '0' then
					clear <= '1';
					done_aes <= '1';
        elsif output_finished = '0' and done_o = '0' and driver_has_read = '1' then
          done_aes <= '0';
				else
					load_i <= '0';
				end if;
			end if;
		end if;
	end process;

	parse_output_proc: process (clock_in, done_o, data_o)
	begin
		if clock_in'event and clock_in = '1' then
			if done_o = '1' and out_aes_counter > 7 then
				output(out_aes_counter downto out_aes_counter-7) <= data_o;
				out_aes_counter <= out_aes_counter - 8;
				output_finished <= '0';
			elsif done_o = '1' and out_aes_counter = 7 then
				output(out_aes_counter downto out_aes_counter-7) <= data_o;
				output_finished <= '1';
			else
				out_aes_counter <= 127;
				output_finished <= '0';
			end if;
		end if;
	end process;

	-- AES core
	crypto_core: entity work.mini_aes
	port map (
		clock  => clock_in,
		clear  => clear, -- 1 bit of reset
		load_i => load_i,
		enc    => enc, -- active low (e.g. 0 = encrypt, 1 = decrypt)
		key_i  => key_i, -- 8 bits of key input
		data_i => data_i_aes, -- 8 bits of data input
		data_o => data_o, -- 8 bits of data output
		done_o => done_o -- 1 bit of signaling of finishing
	);

	-- boot ROM
	boot0lb: entity work.boot_ram
	generic map (	memory_file => "boot.txt",
					data_width => 8,
					address_width => 12,
					bank => 0)
	port map(
		clk 	=> clock_in,
		addr 	=> address(11 downto 2),
		cs_n 	=> boot_enable_n,
		we_n	=> '1',
		data_i	=> (others => '0'),
		data_o	=> data_read_boot(7 downto 0)
	);

	boot0ub: entity work.boot_ram
	generic map (	memory_file => "boot.txt",
					data_width => 8,
					address_width => 12,
					bank => 1)
	port map(
		clk 	=> clock_in,
		addr 	=> address(11 downto 2),
		cs_n 	=> boot_enable_n,
		we_n	=> '1',
		data_i	=> (others => '0'),
		data_o	=> data_read_boot(15 downto 8)
	);

	boot1lb: entity work.boot_ram
	generic map (	memory_file => "boot.txt",
					data_width => 8,
					address_width => 12,
					bank => 2)
	port map(
		clk 	=> clock_in,
		addr 	=> address(11 downto 2),
		cs_n 	=> boot_enable_n,
		we_n	=> '1',
		data_i	=> (others => '0'),
		data_o	=> data_read_boot(23 downto 16)
	);

	boot1ub: entity work.boot_ram
	generic map (	memory_file => "boot.txt",
					data_width => 8,
					address_width => 12,
					bank => 3)
	port map(
		clk 	=> clock_in,
		addr 	=> address(11 downto 2),
		cs_n 	=> boot_enable_n,
		we_n	=> '1',
		data_i	=> (others => '0'),
		data_o	=> data_read_boot(31 downto 24)
	);

	-- RAM
	memory0lb: entity work.bram
	generic map (	memory_file => memory_file,
					data_width => 8,
					address_width => address_width,
					bank => 0)
	port map(
		clk 	=> clock_in,
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
		clk 	=> clock_in,
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
		clk 	=> clock_in,
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
		clk 	=> clock_in,
		addr 	=> address(address_width -1 downto 2),
		cs_n 	=> ram_enable_n,
		we_n	=> data_w_n_ram(3),
		data_i	=> data_write(31 downto 24),
		data_o	=> data_read_ram(31 downto 24)
	);

	-- debug process
	debug:
	if uart_support = "no" generate
		process(clock_in, address)
			file store_file : text open write_mode is "debug.txt";
			variable hex_file_line : line;
			variable c : character;
			variable index : natural;
			variable line_length : natural := 0;
		begin
			if clock_in'event and clock_in = '1' then
				if address = x"f00000d0" and data = '0' then
					data <= '1';
					index := conv_integer(data_write(30 downto 24));
					if index /= 10 then
						c := character'val(index);
						write(hex_file_line, c);
						line_length := line_length + 1;
					end if;
					if index = 10 or line_length >= 72 then
						writeline(store_file, hex_file_line);
						line_length := 0;
					end if;
				else
					data <= '0';
				end if;
			end if;
		end process;
	end generate;

	process(clock_in, reset, address)
	begin
		if reset = '1' then
		elsif clock_in'event and clock_in = '0' then
			assert address /= x"e0000000" report "end of simulation" severity failure;
			assert (address < x"50000000") or (address >= x"e0000000") report "out of memory region" severity failure;
			assert address /= x"40000104" report "handling IRQ" severity warning;
		end if;
	end process;

end tb;

