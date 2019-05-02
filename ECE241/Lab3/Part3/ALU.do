# set the working dir, where all compiled verilog goes
vlib work

# compile all verilog modules in mux.v to working dir
# could also have multiple verilog files
vlog ALU.v

#load simulation using mux as the top level simulation module
vsim ALU

# log all signals and add some signals to waveform window
log {/*}
# add wave {/*} would add all items in top level simulation module
add wave {/*}

# first test case
# set input values using the force command, signal names need to be in {} brackets
# output: 0001 1110 1E0F0F
force {KEY} 2#0000
force {SW[7:4]} 2#1111
force {SW[3:0]} 2#1111
#run simulation for a few ns
run 10ns

# second test case, change input values and run for another 10ns
# output: 0001 1110 1E0F0F
force {KEY} 2#0001
force {SW[7:4]} 2#1111
force {SW[3:0]} 2#1111
run 10ns

# output: 1000 1110 8E0305
force {KEY} 2#0010
force {SW[7:4]} 2#0011
force {SW[3:0]} 2#0101
run 10ns

# output: 1100 0000 C00305
force {KEY} 2#0011
force {SW[7:4]} 2#0011
force {SW[3:0]} 2#0101
run 10ns

# output: 0000 0000 000000
force {KEY} 2#0011
force {SW[7:4]} 2#0000
force {SW[3:0]} 2#0000
run 10ns

# output: 0011 1111 3F0307
force {KEY} 2#0100
force {SW[7:4]} 2#0011
force {SW[3:0]} 2#0111
run 10ns

# output: 0000 0000 000305
force {KEY} 2#0100
force {SW[7:4]} 2#0011
force {SW[3:0]} 2#0101
run 10ns

# output: 0101 1100 5C0305
force {KEY} 2#0101
force {SW[7:4]} 2#0011
force {SW[3:0]} 2#0101
run 10ns

# output: 0110 1001 690305
force {KEY} 2#0110
force {SW[7:4]} 2#0011
force {SW[3:0]} 2#0101
run 10ns

# output: 0000 0000 000305
force {KEY} 2#0111
force {SW[7:4]} 2#0011
force {SW[3:0]} 2#0101
run 10ns
