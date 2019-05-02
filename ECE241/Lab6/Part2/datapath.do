# set the working dir, where all compiled verilog goes
vlib work

# compile all verilog modules in mux.v to working dir
# could also have multiple verilog files
vlog polyFunction.v

#load top level simulation module
vsim datapath

# log all signals and add some signals to waveform window
log {/*}
# add wave {/*} would add all items in top level simulation module
add wave {/*}

#clock
force {clk} 0 0ns, 1 {10ns} -r 20ns

# reset
force {resetn} 0 0ns, 1 {20ns}
force {data_in[7:0]} 2#00000000
force {ld_alu_out} 0
force {ld_a} 0
force {ld_b} 0
force {ld_c} 0
force {ld_x} 0
force {ld_r} 0
force {alu_op} 0
force {alu_select_a[1:0]} 2#00
force {alu_select_b[1:0]} 2#00
#run simulation for a few ns
run 25ns

# step 1
force {data_in[7:0]} 2#00000001
force {ld_alu_out} 0 0ns, 0 {10ns}
force {ld_a} 1 0ns, 0 {10ns}
force {ld_b} 0 0ns, 0 {10ns}
force {ld_c} 0 0ns, 0 {10ns}
force {ld_x} 0 0ns, 0 {10ns}
force {ld_r} 0 0ns, 0 {10ns}
force {alu_op} 0 0ns, 0 {10ns}
force {alu_select_a[1:0]} 2#00
force {alu_select_b[1:0]} 2#00
run 20ns

# step 2
force {data_in[7:0]} 2#00000010
force {ld_alu_out} 0 0ns, 0 {10ns}
force {ld_a} 0 0ns, 0 {10ns}
force {ld_b} 1 0ns, 0 {10ns}
force {ld_c} 0 0ns, 0 {10ns}
force {ld_x} 0 0ns, 0 {10ns}
force {ld_r} 0 0ns, 0 {10ns}
force {alu_op} 0 0ns, 0 {10ns}
force {alu_select_a[1:0]} 2#00
force {alu_select_b[1:0]} 2#00
run 20ns

# step 3
force {data_in[7:0]} 2#00000011
force {ld_alu_out} 0 0ns, 0 {10ns}
force {ld_a} 0 0ns, 0 {10ns}
force {ld_b} 0 0ns, 0 {10ns}
force {ld_c} 1 0ns, 0 {10ns}
force {ld_x} 0 0ns, 0 {10ns}
force {ld_r} 0 0ns, 0 {10ns}
force {alu_op} 0 0ns, 0 {10ns}
force {alu_select_a[1:0]} 2#00
force {alu_select_b[1:0]} 2#00
run 20ns

# step 4
force {data_in[7:0]} 2#00000100
force {ld_alu_out} 0 0ns, 0 {10ns}
force {ld_a} 0 0ns, 0 {10ns}
force {ld_b} 0 0ns, 0 {10ns}
force {ld_c} 0 0ns, 0 {10ns}
force {ld_x} 1 0ns, 0 {10ns}
force {ld_r} 0 0ns, 0 {10ns}
force {alu_op} 0 0ns, 0 {10ns}
force {alu_select_a[1:0]} 2#00
force {alu_select_b[1:0]} 2#00
run 20ns

# step 5
force {data_in[7:0]} 2#00000100
force {ld_alu_out} 1 0ns, 0 {10ns}
force {ld_a} 1 0ns, 0 {10ns}
force {ld_b} 0 0ns, 0 {10ns}
force {ld_c} 0 0ns, 0 {10ns}
force {ld_x} 0 0ns, 0 {10ns}
force {ld_r} 0 0ns, 0 {10ns}
force {alu_op} 1 0ns, 0 {10ns}
force {alu_select_a[1:0]} 2#00
force {alu_select_b[1:0]} 2#11
run 20ns

# step 6
force {data_in[7:0]} 2#00000100
force {ld_alu_out} 1 0ns, 0 {10ns}
force {ld_a} 0 0ns, 0 {10ns}
force {ld_b} 1 0ns, 0 {10ns}
force {ld_c} 0 0ns, 0 {10ns}
force {ld_x} 0 0ns, 0 {10ns}
force {ld_r} 0 0ns, 0 {10ns}
force {alu_op} 1 0ns, 0 {10ns}
force {alu_select_a[1:0]} 2#01
force {alu_select_b[1:0]} 2#11
run 20ns

# step 7
force {data_in[7:0]} 2#00000100
force {ld_alu_out} 1 0ns, 0 {10ns}
force {ld_a} 0 0ns, 0 {10ns}
force {ld_b} 1 0ns, 0 {10ns}
force {ld_c} 0 0ns, 0 {10ns}
force {ld_x} 0 0ns, 0 {10ns}
force {ld_r} 0 0ns, 0 {10ns}
force {alu_op} 1 0ns, 0 {10ns}
force {alu_select_a[1:0]} 2#01
force {alu_select_b[1:0]} 2#11
run 20ns

# step 8
force {data_in[7:0]} 2#00000100
force {ld_alu_out} 1 0ns, 0 {10ns}
force {ld_a} 1 0ns, 0 {10ns}
force {ld_b} 0 0ns, 0 {10ns}
force {ld_c} 0 0ns, 0 {10ns}
force {ld_x} 0 0ns, 0 {10ns}
force {ld_r} 0 0ns, 0 {10ns}
force {alu_op} 0 0ns, 0 {10ns}
force {alu_select_a[1:0]} 2#00
force {alu_select_b[1:0]} 2#01
run 20ns

# step 9 and done
force {data_in[7:0]} 2#00000100
force {ld_alu_out} 0 0ns, 0 {10ns}
force {ld_a} 0 0ns, 0 {10ns}
force {ld_b} 0 0ns, 0 {10ns}
force {ld_c} 0 0ns, 0 {10ns}
force {ld_x} 0 0ns, 0 {10ns}
force {ld_r} 1 0ns, 0 {10ns}
force {alu_op} 0 0ns, 0 {10ns}
force {alu_select_a[1:0]} 2#00
force {alu_select_b[1:0]} 2#10
run 20ns
