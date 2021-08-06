library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity average is 
    generic(
        average : integer := 16  --range 0 to 16
    );
    port(
        clk : in std_logic;
        rst : in std_logic;
        
        empty : in std_logic;
        data_in : in std_logic_vector(7 downto 0); 
        
        read : out std_logic;
        valid : out std_logic;
        result : out std_logic_vector(11 downto 0));
end entity; 

architecture rtl of average is 



constant reciprocal : unsigned(7 downto 0) := to_unsigned(average,8);

signal accumulator : unsigned(11 downto 0) := (others => '0');
signal counter : integer range 0 to 31;
signal output : unsigned(19 downto 0) := (others => '0');
signal rd_int : std_logic:= '0';
signal rd_int_del : std_logic := '0';
begin

process(clk,rst)
begin
    if rst  = '1' then
        accumulator <= (others => '0');
        valid  <= '0';
        output <= (others =>'0');
        rd_int <= '0';
    elsif rising_edge(clk) then 
        valid <= '0'; 
        if empty = '0' then -- there is data in the fifo
            rd_int <= '1';
            rd_int_del <= rd_int;
            accumulator <= accumulator + unsigned(data_in);
            if rd_int_del = '1' then --accounts for latency of 1 clock period of fifo output   
                if counter = average then 
                    counter <= 0;
                    rd_int <= '0';
                    output <= (accumulator) * reciprocal; -- 20 bit result, 12 integer, 8 fractional
                    accumulator <= (others => '0');
                    valid <= '1';
                else
                    counter <= counter + 1;
                end if;  
             else
                accumulator <= (others => '0');
                counter <= 0;
             end if;      
        else
            accumulator <= (others => '0');
            counter <= 0;
            rd_int <= '0';                 
        end if;
    end if;
end process;

result <= std_logic_vector(output(output'high downto output'high-11));
read <= rd_int;

end architecture;
        