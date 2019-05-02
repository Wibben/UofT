# set the working dir, where all compiled verilog goes
vlib work

# compile all verilog modules in mux.v to working dir
# could also have multiple verilog files
vlog restoringDivider4bit.v

#load top level simulation module
vsim datapath

# log all signals and add some signals to waveform window
log {/*}
# add wave {/*} would add all items in top level simulation module
add wave {/*}

#clock
force {clock} 0 0ns, 1 {10ns} -r 20ns

# reset
force {reset} 1 0us, 0 {20ns}
force {DATA_IN[7:0]} 2#01110011
force {ld_m} 0
force {ld_a} 0
force {ld_q} 0
force {ld_a4} 0
force {ld_r} 0
force {ld_Q} 0
force {ld_dividend} 0
force {ld_alu_out} 0
force {alu_op} 0
force {enable_shift} 0
force {Pload} 0
force {ld_shift} 0
force {set_a} 0
#run simulation for a few ns
run 40ns

#load values
force {ld_m} 1
force {ld_a} 0
force {ld_q} 1
force {ld_a4} 0
force {ld_r} 0
force {ld_Q} 0
force {ld_dividend} 1
force {ld_alu_out} 0
force {alu_op} 0
force {enable_shift} 0
force {Pload} 0
force {ld_shift} 0
force {set_a} 1
#run simulation for a few ns
run 20ns

#step 1
force {ld_m} 0
force {ld_a} 0
force {ld_q} 0
force {ld_a4} 0
force {ld_r} 0
force {ld_Q} 0
force {ld_dividend} 0
force {ld_alu_out} 0
force {alu_op} 0
force {enable_shift} 0
force {Pload} 1
force {ld_shift} 0
force {set_a} 0
#run simulation for a few ns
run 20ns

#step 2
force {ld_m} 0
force {ld_a} 0
force {ld_q} 0
force {ld_a4} 0
force {ld_r} 0
force {ld_Q} 0
force {ld_dividend} 0
force {ld_alu_out} 0
force {alu_op} 0
force {enable_shift} 1
force {Pload} 0
force {ld_shift} 0
force {set_a} 0
#run simulation for a few ns
run 20ns

#step 3
force {ld_m} 0
force {ld_a} 1
force {ld_q} 1
force {ld_a4} 0
force {ld_r} 0
force {ld_Q} 0
force {ld_dividend} 0
force {ld_alu_out} 0
force {alu_op} 0
force {enable_shift} 0
force {Pload} 0
force {ld_shift} 1
force {set_a} 0
#run simulation for a few ns
run 20ns

#step 4
force {ld_m} 0
force {ld_a} 1
force {ld_q} 0
force {ld_a4} 0
force {ld_r} 0
force {ld_Q} 0
force {ld_dividend} 0
force {ld_alu_out} 1
force {alu_op} 1
force {enable_shift} 0
force {Pload} 0
force {ld_shift} 0
force {set_a} 0
#run simulation for a few ns
run 20ns

#step 5
force {ld_m} 0
force {ld_a} 0
force {ld_q} 0
force {ld_a4} 1
force {ld_r} 0
force {ld_Q} 0
force {ld_dividend} 0
force {ld_alu_out} 0
force {alu_op} 0
force {enable_shift} 0
force {Pload} 0
force {ld_shift} 0
force {set_a} 0
#run simulation for a few ns
run 20ns

#step 6
force {ld_m} 0
force {ld_a} 1
force {ld_q} 0
force {ld_a4} 0
force {ld_r} 0
force {ld_Q} 0
force {ld_dividend} 0
force {ld_alu_out} 1
force {alu_op} 0
force {enable_shift} 0
force {Pload} 0
force {ld_shift} 0
force {set_a} 0
#run simulation for a few ns
run 20ns

#step 7, counter increment
force {ld_m} 0
force {ld_a} 0
force {ld_q} 0
force {ld_a4} 0
force {ld_r} 0
force {ld_Q} 0
force {ld_dividend} 0
force {ld_alu_out} 0
force {alu_op} 0
force {enable_shift} 0
force {Pload} 0
force {ld_shift} 0
force {set_a} 0
#run simulation for a few ns
run 20ns

#step 8
force {ld_m} 0
force {ld_a} 0
force {ld_q} 0
force {ld_a4} 0
force {ld_r} 0
force {ld_Q} 0
force {ld_dividend} 0
force {ld_alu_out} 0
force {alu_op} 0
force {enable_shift} 0
force {Pload} 0
force {ld_shift} 0
force {set_a} 0
#run simulation for a few ns
run 20ns

#step 9
force {ld_m} 0
force {ld_a} 0
force {ld_q} 0
force {ld_a4} 0
force {ld_r} 0
force {ld_Q} 0
force {ld_dividend} 0
force {ld_alu_out} 0
force {alu_op} 0
force {enable_shift} 0
force {Pload} 0
force {ld_shift} 0
force {set_a} 0
#run simulation for a few ns
run 20ns
