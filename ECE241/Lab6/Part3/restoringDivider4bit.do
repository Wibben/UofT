# set the working dir, where all compiled verilog goes
vlib work

# compile all verilog modules in mux.v to working dir
# could also have multiple verilog files
vlog restoringDivider4bit.v

#load top level simulation module
vsim restoringDivider4bit

# log all signals and add some signals to waveform window
log {/*}
# add wave {/*} would add all items in top level simulation module
add wave {/*}

#clock
force {CLOCK_50} 0 0ns, 1 {10ns} -r 20ns

# reset
force {KEY[0]} 1 0us, 0 {0.5us}
force {KEY[1]} 0 0us
force {SW[7:0]} 2#00000000
#run simulation for a few ns
run 1us

# Set up Values (7/3)
force {SW[7:0]} 2#01110011
force {KEY[1]} 1 0us, 0 {0.5us}
run 2us

# Set up Values (15/5)
force {SW[7:0]} 2#11110101
force {KEY[1]} 1 0us, 0 {0.5us}
run 2us
