# set the working dir, where all compiled verilog goes
vlib work

# compile all verilog modules in mux.v to working dir
# could also have multiple verilog files
vlog [filename].v

#load top level simulation module
vsim [top level module(same as filename)]

# log all signals and add some signals to waveform window
log {/*}
# add wave {/*} would add all items in top level simulation module
add wave {/*}

#clock
force {CLOCK_50} 0 0ns, 1 {10ns} -r 20ns

# first test case
# set input values using the force command, signal names need to be in {} brackets
# output: 
force {KEY[3:0]} 2#0000
force {SW[9:0]} 2#0000000000
#run simulation for a few ns
run 10ns

# second test case, change input values and run for another 10ns
# output: 
force {SW[9]} 0
force {SW[8]} 0
force {SW[7]} 0
force {SW[6]} 0
force {SW[5]} 0
force {SW[4]} 0
force {SW[3]} 0
force {SW[2]} 0
force {SW[1]} 0
force {SW[0]} 0
run 10ns