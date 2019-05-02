# set the working dir, where all compiled verilog goes
vlib work

# compile all verilog modules in mux.v to working dir
# could also have multiple verilog files
vlog drawSquare.v

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
force {DATA_IN[6:0]} 2#1100100
force {COLOUR_DATA[2:0]} 2#111
force {ld_x} 0
force {ld_y} 0
force {ld_colour} 0
force {ld_xpos} 0
force {ld_ypos} 0
force {set_black} 0
force {dx[7:0]} 2#00000000
force {dy[7:0]} 2#00000000
#run simulation for a few ns
run 40ns

# load values
force {ld_x} 1 0ns, 0 {20ns}
force {ld_y} 1 20ns, 0 {40ns}
force {ld_colour} 1 40ns, 0 {60ns}
force {ld_xpos} 0
force {ld_ypos} 0
force {set_black} 0
force {dx[7:0]} 2#00000000
force {dy[7:0]} 2#00000000
run 60ns

# drawing
force {ld_xpos} 1 0ns, 0 {20ns}
force {ld_xpos} 1 60ns, 0 {80ns}
force {ld_xpos} 1 120ns, 0 {140ns}
force {ld_xpos} 1 180ns, 0 {200ns}
force {ld_ypos} 1 0ns, 0 {20ns}
force {ld_ypos} 1 60ns, 0 {80ns}
force {ld_ypos} 1 120ns, 0 {140ns}
force {ld_ypos} 1 180ns, 0 {200ns}
force {set_black} 0
force {dx[7:0]} 2#00000000 0ns, 2#00000001 {60ns}
force {dx[7:0]} 2#00000010 120ns, 2#00000011 {180ns}
force {dx[7:0]} 2#00000100 240ns
force {dy[7:0]} 2#00000001 260ns
run 280ns

# drawing
force {ld_xpos} 1 0ns, 0 {20ns}
force {ld_xpos} 1 60ns, 0 {80ns}
force {ld_xpos} 1 120ns, 0 {140ns}
force {ld_xpos} 1 180ns, 0 {200ns}
force {ld_ypos} 1 0ns, 0 {20ns}
force {ld_ypos} 1 60ns, 0 {80ns}
force {ld_ypos} 1 120ns, 0 {140ns}
force {ld_ypos} 1 180ns, 0 {200ns}
force {set_black} 0
force {dx[7:0]} 2#00000000 0ns, 2#00000001 {60ns}
force {dx[7:0]} 2#00000010 120ns, 2#00000011 {180ns}
force {dx[7:0]} 2#00000100 240ns
force {dy[7:0]} 2#00000010 260ns
run 280ns

force {ld_xpos} 1 0ns, 0 {20ns}
force {ld_xpos} 1 60ns, 0 {80ns}
force {ld_xpos} 1 120ns, 0 {140ns}
force {ld_xpos} 1 180ns, 0 {200ns}
force {ld_ypos} 1 0ns, 0 {20ns}
force {ld_ypos} 1 60ns, 0 {80ns}
force {ld_ypos} 1 120ns, 0 {140ns}
force {ld_ypos} 1 180ns, 0 {200ns}
force {set_black} 0
force {dx[7:0]} 2#00000000 0ns, 2#00000001 {60ns}
force {dx[7:0]} 2#00000010 120ns, 2#00000011 {180ns}
force {dx[7:0]} 2#00000100 240ns
force {dy[7:0]} 2#00000011 260ns
run 280ns

force {ld_xpos} 1 0ns, 0 {20ns}
force {ld_xpos} 1 60ns, 0 {80ns}
force {ld_xpos} 1 120ns, 0 {140ns}
force {ld_xpos} 1 180ns, 0 {200ns}
force {ld_ypos} 1 0ns, 0 {20ns}
force {ld_ypos} 1 60ns, 0 {80ns}
force {ld_ypos} 1 120ns, 0 {140ns}
force {ld_ypos} 1 180ns, 0 {200ns}
force {set_black} 0
force {dx[7:0]} 2#00000000 0ns, 2#00000001 {60ns}
force {dx[7:0]} 2#00000010 120ns, 2#00000011 {180ns}
force {dx[7:0]} 2#00000100 240ns
force {dy[7:0]} 2#00000100 260ns
run 280ns

#set colour to black
force {set_black} 1
run 60ns