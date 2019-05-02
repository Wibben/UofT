# set the working dir, where all compiled verilog goes
vlib work

# compile all verilog modules in mux.v to working dir
# could also have multiple verilog files
vlog MorseEncoder.v

#load simulation using mux as the top level simulation module
vsim MorseEncoder

# log all signals and add some signals to waveform window
log {/*}
# add wave {/*} would add all items in top level simulation module
add wave {/*}

force {CLOCK_50} 0 0ns, 1 {10ns} -r 20ns

# first test case
# set input values using the force command, signal names need to be in {} brackets
# output: 
force {KEY[0]} 1
force {KEY[1]} 1
force {SW[2:0]} 2#000
#run simulation for a few ns
run 500ns

force {KEY[0]} 0
run 250ns

# second test case, change input values and run for another 10ns
# output: 
force {KEY[0]} 0
force {KEY[1]} 0
force {SW[2:0]} 2#000
run 13us

force {KEY[1]} 1
force {SW[2:0]} 2#001
run 100ns

force {KEY[1]} 0
force {SW[2:0]} 2#001
run 6us

force {SW[2:0]} 2#111
run 6.8us

force {KEY[1]} 1
force {SW[2:0]} 2#110
run 100ns

force {KEY[0]} 0
force {KEY[1]} 0
force {SW[2:0]} 2#110
run 5.9us

force {KEY[0]} 1
run 100ns

force {KEY[0]} 0
run 2.5us