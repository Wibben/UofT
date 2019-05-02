# set the working dir, where all compiled verilog goes
vlib work

# compile all verilog modules in mux.v to working dir
# could also have multiple verilog files
vlog fullAdder4bit.v

#load simulation using mux as the top level simulation module
vsim fullAdder4bit

# log all signals and add some signals to waveform window
log {/*}
# add wave {/*} would add all items in top level simulation module
add wave {/*}

# first test case
# set input values using the force command, signal names need to be in {} brackets
# output: 0 0000
force {SW[8]} 0

force {SW[7]} 0
force {SW[6]} 0
force {SW[5]} 0
force {SW[4]} 0

force {SW[3]} 0
force {SW[2]} 0
force {SW[1]} 0
force {SW[0]} 0
#run simulation for a few ns
run 10ns

# second test case, change input values and run for another 10ns
# output: 1 0010
force {SW[8]} 0

force {SW[7]} 1
force {SW[6]} 1
force {SW[5]} 0
force {SW[4]} 1

force {SW[3]} 0
force {SW[2]} 1
force {SW[1]} 0
force {SW[0]} 1
run 10ns

# output: 1 1111
force {SW[8]} 1

force {SW[7]} 1
force {SW[6]} 1
force {SW[5]} 1
force {SW[4]} 1

force {SW[3]} 1
force {SW[2]} 1
force {SW[1]} 1
force {SW[0]} 1
run 10ns

# output: 1 1110
force {SW[8]} 0

force {SW[7]} 1
force {SW[6]} 1
force {SW[5]} 1
force {SW[4]} 1

force {SW[3]} 1
force {SW[2]} 1
force {SW[1]} 1
force {SW[0]} 1
run 10ns

# output: 0 1111
force {SW[8]} 0

force {SW[7]} 0
force {SW[6]} 0
force {SW[5]} 1
force {SW[4]} 0

force {SW[3]} 1
force {SW[2]} 1
force {SW[1]} 0
force {SW[0]} 1
run 10ns

# output: 0 0101
force {SW[8]} 0

force {SW[7]} 0
force {SW[6]} 0
force {SW[5]} 0
force {SW[4]} 0

force {SW[3]} 0
force {SW[2]} 1
force {SW[1]} 0
force {SW[0]} 1
run 10ns

# output: 1 0100
force {SW[8]} 0

force {SW[7]} 1
force {SW[6]} 0
force {SW[5]} 1
force {SW[4]} 0

force {SW[3]} 1
force {SW[2]} 0
force {SW[1]} 1
force {SW[0]} 0
run 10ns

# output: 0 1110
force {SW[8]} 1

force {SW[7]} 1
force {SW[6]} 1
force {SW[5]} 0
force {SW[4]} 1

force {SW[3]} 0
force {SW[2]} 0
force {SW[1]} 0
force {SW[0]} 0
run 10ns
