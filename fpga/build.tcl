# Herring-F
# Target: Tang Nano 9k (Gowin GW1NR-9C)
set_device -name GW1NR-9C GW1NR-LV9QN88PC6/I5

add_file herring_f.cst
add_file herring_f.sdc

add_file cores/verilog-6502/cpu.v
add_file cores/verilog-6502/ALU.v

add_file rtl/herring_f_top.v

set_option -top_module herring_f_top
set_option -output_base_name herring_f

run all
