# set the working dir, where all compiled verilog goes
vlib work

# compile all verilog modules in mux.v to working dir
# could also have multiple verilog files
vlog sequenceDetector.v

#load simulation using mux as the top level simulation module
vsim sequenceDetector

# log all signals and add some signals to waveform window
log {/*}
# add wave {/*} would add all items in top level simulation module
add wave {/*}

#clock
force {KEY[0]} 0 0ns, 1 {5ns} -r 10ns

# reset
force {KEY[0]} 0
force {SW[0]} 0 0ns, 1 {10ns}
force {SW[1]} 0
#run simulation for a few ns
run 10ns

# 111111010001011
# output: 
force {SW[1]} 1 0ns, 0 {60ns}
force {SW[1]} 1 70ns, 0 {80ns}
force {SW[1]} 1 110ns, 0 {120ns}
force {SW[1]} 1 130ns
run 150ns