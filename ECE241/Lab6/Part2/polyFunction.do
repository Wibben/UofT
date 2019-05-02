# set the working dir, where all compiled verilog goes
vlib work

# compile all verilog modules in mux.v to working dir
# could also have multiple verilog files
vlog polyFunction.v

#load simulation using mux as the top level simulation module
vsim polyFunction

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

# Set up A 
force {SW[7:0]} 2#00000101
force {KEY[1]} 0 0us, 1 {0.5us} -r 1us
run 1us

# Set up B 
force {SW[7:0]} 2#00000100
run 1us

# Set up C 
force {SW[7:0]} 2#00000011
run 1us

# Set up x
force {SW[7:0]} 2#00000010
run 1us

#see results
force {KEY[1]} 0 0.5us
run 1us

# Set up A 
force {SW[7:0]} 2#00000001
force {KEY[1]} 0 0us, 1 {0.5us} -r 1us
run 1us

# Set up B 
force {SW[7:0]} 2#00000010
run 1us

# Set up C 
force {SW[7:0]} 2#00000011
run 1us

# Set up x
force {SW[7:0]} 2#00000100
run 1us

# See results
force {KEY[1]} 0 0.5us
run 1us