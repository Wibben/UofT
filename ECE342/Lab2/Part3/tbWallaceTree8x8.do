vlib work
vlog *.sv
vsim -novopt tbWallaceTree8x8
add wave /tbWallaceTree8x8/*
add wave -group multiplier /tbWallaceTree8x8/DUT/*
run -all
