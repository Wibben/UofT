# set the working dir, where all compiled verilog goes
vlib work

# compile all verilog modules in mux.v to working dir
# could also have multiple verilog files
vlog animateSquare.v

#load top level simulation module
vsim datapath

# log all signals and add some signals to waveform window
log {/*}
# add wave {/*} would add all items in top level simulation module
add wave {/*}

#clock
force {clock} 0 0ns, 1 {10ns} -r 20ns

# reset
force {resetn} 0 0 ns, 1 {20ns}
force {X_IN[7:0]} 2#10011100
force {Y_IN[7:0]} 2#00000000
force {COLOUR_DATA[2:0]} 2#111
force {ld_xy} 0
force {ld_colour} 0
force {ld_pos} 0
force {set_black} 0
force {dx[7:0]} 2#00000000
force {dy[7:0]} 2#00000000
#run simulation for a few ns
run 40ns

# load values
force {ld_xy} 1 0ns, 0 {20ns}
force {ld_colour} 1 0ns, 0 {20ns}
force {ld_pos} 0
force {set_black} 0
force {dx[7:0]} 2#00000000
force {dy[7:0]} 2#00000000
run 40ns

# drawing
force {ld_pos} 1 0ns, 0 {20ns}
force {ld_pos} 1 60ns, 0 {80ns}
force {ld_pos} 1 120ns, 0 {140ns}
force {ld_pos} 1 180ns, 0 {200ns}
force {set_black} 0
force {dx[7:0]} 2#00000000 0ns, 2#00000001 {60ns}
force {dx[7:0]} 2#00000010 120ns, 2#00000011 {180ns}
force {dx[7:0]} 2#00000100 240ns
force {dy[7:0]} 2#00000001 260ns
run 280ns

# drawing
force {ld_pos} 1 0ns, 0 {20ns}
force {ld_pos} 1 60ns, 0 {80ns}
force {ld_pos} 1 120ns, 0 {140ns}
force {ld_pos} 1 180ns, 0 {200ns}
force {set_black} 0
force {dx[7:0]} 2#00000000 0ns, 2#00000001 {60ns}
force {dx[7:0]} 2#00000010 120ns, 2#00000011 {180ns}
force {dx[7:0]} 2#00000100 240ns
force {dy[7:0]} 2#00000010 260ns
run 280ns

#set colour to black
force {set_black} 1 100 ns, 0 {120ns}
force {dy[7:0]} 2#00000000 120ns
run 140ns

#erase

force {ld_pos} 1 0ns, 0 {20ns}
force {ld_pos} 1 60ns, 0 {80ns}
force {ld_pos} 1 120ns, 0 {140ns}
force {ld_pos} 1 180ns, 0 {200ns}
force {set_black} 0
force {dx[7:0]} 2#00000000 0ns, 2#00000001 {60ns}
force {dx[7:0]} 2#00000010 120ns, 2#00000011 {180ns}
force {dx[7:0]} 2#00000100 240ns
force {dy[7:0]} 2#00000001 260ns
run 280ns

# drawing
force {ld_pos} 1 0ns, 0 {20ns}
force {ld_pos} 1 60ns, 0 {80ns}
force {ld_pos} 1 120ns, 0 {140ns}
force {ld_pos} 1 180ns, 0 {200ns}
force {set_black} 0
force {dx[7:0]} 2#00000000 0ns, 2#00000001 {60ns}
force {dx[7:0]} 2#00000010 120ns, 2#00000011 {180ns}
force {dx[7:0]} 2#00000100 240ns
force {dy[7:0]} 2#00000010 260ns
run 300ns

# load values
force {COLOUR_DATA[2:0]} 2#100
force {X_IN[7:0]} 2#10011011
force {Y_IN[7:0]} 2#00000000
force {ld_xy} 1 0ns, 0 {20ns}
force {ld_colour} 1 0ns, 0 {20ns}
force {ld_pos} 0
force {set_black} 0
force {dx[7:0]} 2#00000000
force {dy[7:0]} 2#00000000
run 40ns

# drawing
force {ld_pos} 1 0ns, 0 {20ns}
force {ld_pos} 1 60ns, 0 {80ns}
force {ld_pos} 1 120ns, 0 {140ns}
force {ld_pos} 1 180ns, 0 {200ns}
force {set_black} 0
force {dx[7:0]} 2#00000000 0ns, 2#00000001 {60ns}
force {dx[7:0]} 2#00000010 120ns, 2#00000011 {180ns}
force {dx[7:0]} 2#00000100 240ns
force {dy[7:0]} 2#00000001 260ns
run 280ns
