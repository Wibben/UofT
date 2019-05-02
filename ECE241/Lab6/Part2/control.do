# set the working dir, where all compiled verilog goes
vlib work

# compile all verilog modules in mux.v to working dir
# could also have multiple verilog files
vlog polyFunction.v

#load top level simulation module
vsim control

# log all signals and add some signals to waveform window
log {/*}
# add wave {/*} would add all items in top level simulation module
add wave {/*}

#clock
force {clk} 0 0ns, 1 {10ns} -r 20ns

# reset
force {resetn} 0 0ns, 1 {50ns}
force {go} 0 0ns
#run simulation for a few ns
run 95ns

# Set up values
force {go} 0 0ns, 1 {50ns} -r 100ns
run 400ns

#see results
force {go} 0 0ns, 0 {50ns} -r 100ns
run 200ns