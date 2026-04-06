# DEVICE #
set_device -name GW5A-25A GW5A-LV25MG121NES 

# SYSTEM VERILOG 2017 #
set_option -verilog_std sysv2017

# SRC CODE #
add_file src/top.sv
add_file src/rising_edge_detector.sv
add_file src/debouncer.sv
add_file src/sync.sv
add_file src/uart_rx.sv

# SRC CONSTRAINTS #
add_file cst/physical.cst
add_file cst/timing.sdc

# OPTION #
set_option -top_module top
set_option -output_base_name micro_rusarpion_fpga

# USE GPIO #
set_option -use_done_as_gpio 1
set_option -use_ready_as_gpio 1
set_option -use_sspi_as_gpio 1
set_option -use_i2c_as_gpio 1
set_option -use_cpu_as_gpio 1

# RUN #
run all