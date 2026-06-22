# Herring-F timing constraints - Tang Nano 9k

# 27 MHz onboard oscillator. 1/27 MHz = 37.037 ns
create_clock -name clk_27 -period 37.037 [get_ports {clk_27}]
