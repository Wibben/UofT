# set the working dir, where all compiled verilog goes
vlib work

# compile all verilog modules in mux.v to working dir
# could also have multiple verilog files
vlog drawSquare.v

#load top level simulation module
vsim control

# log all signals and add some signals to waveform window
log {/*}
# add wave {/*} would add all items in top level simulation module
add wave {/*}

#clock
force {clock} 0 0ns, 1 {10ns} -r 20ns

# reset
force {resetn} 0 0 ns, 1 {25ns}
force {go} 0
force {plot} 0
force {clear} 0
#run simulation for a few ns
run 50ns

# second test case, change input values and run for another 10ns
# output: 
force {go} 1 0ns, 0 {50ns}
force {go} 1 100ns, 0 {150ns}
force {go} 1 200ns, 0 {250ns}
force {plot} 1 300ns, 0 {350ns}
force {clear} 1 1600ns, 0 {1650ns}
run 2500ns