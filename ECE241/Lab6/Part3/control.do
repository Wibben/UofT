# set the working dir, where all compiled verilog goes
vlib work

# compile all verilog modules in mux.v to working dir
# could also have multiple verilog files
vlog restoringDivider4bit.v

#load top level simulation module
vsim control

# log all signals and add some signals to waveform window
log {/*}
# add wave {/*} would add all items in top level simulation module
add wave {/*}

#clock
force {clock} 0 0ns, 1 {10ns} -r 20ns

# reset
force {reset} 1 0us, 0 {0.025us}
force {go} 0 0us
force {a4} 0
#run simulation for a few ns
run 0.05us

# Set up value and see results
force {go} 1 0us, 0 {0.05us}
force {a4} 1 0.12 us, 0 {0.16us}
force {a4} 1 0.26 us, 0 {0.30us}
force {a4} 1 0.52 us, 0 {0.56us}
run 0.65us
