vlib work
vlog *.sv
vsim -novopt tbBooth8x8
add wave /tbBooth8x8/*
add wave -group multiplier /tbBooth8x8/DUT/*
add wave -group multiplier /tbBooth8x8/DUT/s
run -all
