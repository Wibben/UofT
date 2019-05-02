# set the working dir, where all compiled verilog goes
vlib work

# compile all verilog modules in mux.v to working dir
# could also have multiple verilog files
vlog multiSpeedCounter4bit.v

#load simulation using mux as the top level simulation module
vsim multiSpeedCounter4bit

# log all signals and add some signals to waveform window
log {/*}
# add wave {/*} would add all items in top level simulation module
add wave {/*}

force {CLOCK_50} 0 0ns, 1 {10ns} -r 20ns

# first test case
# set input values using the force command, signal names need to be in {} brackets
# output: 
force {SW[2]} 0
force {SW[1:0]} 2#11
#run simulation for a few ns
run 1us

# second test case, change input values and run for another 10ns
# output: 
force {SW[2]} 1
force {SW[1:0]} 2#11
run 25us

force {SW[1:0]} 2#10
run 28us

force {SW[1:0]} 2#01
run 26us

force {SW[1:0]} 2#00
run 5us
