# Herring-F
# Target: Tang Nano 20k (Gowin GW2AR-18C)

set_device -name GW2AR-18C GW2AR-LV18QN88C8/I7

add_file herring_f.cst
add_file herring_f.sdc

add_file cores/verilog-6502/cpu.v
add_file cores/verilog-6502/ALU.v

add_file rtl/herring_f_top.v

set_option -top_module herring_f_top
set_option -output_base_name herring_f

run all
