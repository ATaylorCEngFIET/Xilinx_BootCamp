library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity average_tb is
--  Port ( );
end average_tb;

architecture Behavioral of average_tb is

type mem is array (0 to 31) of integer;

constant stim : mem := ( 35,166,5,233,25,240,41,80,110,77,117,94,220,160,160,139,
                         30,117,103,55,23,31,109,115,202,22,101,24,235,61,59,249);

constant clk_period : time := 10ns;

component average is 
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
end component; 

signal clk : std_logic :='0';
signal rst : std_logic :='0';

signal empty   :  std_logic;
signal data_in :  std_logic_vector(7 downto 0); 
        
signal read   :  std_logic;
signal valid  :  std_logic;
signal result :  std_logic_vector(11 downto 0);

begin

clk_gen: clk <= not clk after (clk_period/2);

uut : average port map (clk,rst,empty,data_in,read,valid,result);

stimulus: process
begin
    rst <= '1';
    data_in <= (others =>'0');
    empty <= '1';
    wait for 32 ns;
    wait until falling_edge(clk);
    rst <= '0';
    wait for 100 ns;
    wait until rising_edge(clk);
    empty <= '0';
    wait until rising_edge(clk);
    wait until read = '1';
    for i in 0 to 15 loop
       
            wait until rising_edge(clk);
            data_in <= std_logic_vector(to_unsigned(stim(i),8));      
    
    end loop;
    wait until rising_edge(clk);
    wait until read = '1';
    for i in 16 to 31 loop
       
            wait until rising_edge(clk);
            data_in <= std_logic_vector(to_unsigned(stim(i),8));      
    
    end loop;
    wait until rising_edge(valid);
    wait for 100 ns;
    report "simulation complete" severity failure;

end process;
 


end Behavioral;
