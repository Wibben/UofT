# set the working dir, where all compiled verilog goes
vlib work

# compile all verilog modules in mux.v to working dir
# could also have multiple verilog files
vlog TFFcounter8bit.v

#load simulation using mux as the top level simulation module
vsim TFFcounter8bit

# log all signals and add some signals to waveform window
log {/*}
# add wave {/*} would add all items in top level simulation module
add wave {/*}

# clock
# switch every 5 ns
force {KEY[0]} 0 0ns, 1 {5ns} -r 10ns

# first test case
# set input values using the force command, signal names need to be in {} brackets
# output: 
force {SW[1]} 0
force {SW[0]} 0
#run simulation for a few ns
run 10ns

# second test case, change input values and run for another 10ns
# output: 
force {SW[1]} 1
force {SW[0]} 1
run 170ns
