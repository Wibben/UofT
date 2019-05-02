# set the working dir, where all compiled verilog goes
vlib work

# compile all verilog modules in mux.v to working dir
# could also have multiple verilog files
vlog memory.v

#load top level simulation module
vsim -L altera_mf_ver memory

# log all signals and add some signals to waveform window
log {/*}
# add wave {/*} would add all items in top level simulation module
add wave {/*}

#clock
force {KEY[0]} 0 0ns, 1 {10ns} -r 20ns

# first test case
# set input values using the force command, signal names need to be in {} brackets
# output: 
force {SW[9]} 0
force {SW[8:4]} 2#00000
force {SW[3:0]} 2#0000
#run simulation for a few ns
run 25ns

# Writes
force {SW[9]} 1
force {SW[8:4]} 2#00000
force {SW[3:0]} 2#1111
run 20ns

force {SW[9]} 1
force {SW[8:4]} 2#10000
force {SW[3:0]} 2#1100
run 20ns

force {SW[9]} 1
force {SW[8:4]} 2#01111
force {SW[3:0]} 2#1010
run 20ns

# Reads
force {SW[9]} 0
force {SW[8:4]} 2#00000
force {SW[3:0]} 2#1101
run 40ns

force {SW[9]} 0
force {SW[8:4]} 2#10000
force {SW[3:0]} 2#0101
run 40ns

force {SW[9]} 0
force {SW[8:4]} 2#01111
force {SW[3:0]} 2#0011
run 40ns

