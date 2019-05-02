# set the working dir, where all compiled verilog goes
vlib work

# compile all verilog modules in mux.v to working dir
# could also have multiple verilog files
vlog rotatingRegister8bit.v

#load simulation using mux as the top level simulation module
vsim rotatingRegister8bit

# log all signals and add some signals to waveform window
log {/*}
# add wave {/*} would add all items in top level simulation module
add wave {/*}

# clock
# switch every 5 ns
force {KEY[0]} 0 0ns, 1 {5ns} -r 10ns

# initial DATA_IN
force {SW[7:0]} 2#10110111

# first test case
# set input values using the force command, signal names need to be in {} brackets
# output: 
force {KEY[3:1]} 2#000
force {SW[9]} 1
#run simulation for a few ns
run 10ns

# output: 
force {KEY[3:1]} 2#000
force {SW[9]} 0
#run simulation for a few ns
run 10ns

# output: 
force {KEY[3:1]} 2#011
force {SW[9]} 0
run 30ns

# output: 
force {KEY[3:1]} 2#111
force {SW[9]} 0
run 20ns

# output: 
force {KEY[3:1]} 2#001
force {SW[9]} 0
run 20ns

# output: 
force {KEY[3:1]} 2#101
force {SW[9]} 0
run 10ns

# output: 
force {KEY[3:1]} 2#011
force {SW[9]} 1
run 10ns

# output: 
force {KEY[3:1]} 2#011
force {SW[9]} 0
run 10ns

# output: 
force {SW[7:0]} 2#01001000
force {KEY[3:1]} 2#110
force {SW[9]} 0
run 10ns

# output: 
force {KEY[3:1]} 2#001
force {SW[9]} 0
run 20ns
