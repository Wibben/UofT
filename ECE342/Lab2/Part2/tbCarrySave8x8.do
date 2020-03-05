vlib work
vlog *.sv
vsim -novopt tbCarrySave8x8
add wave /tbCarrySave8x8/*
add wave -group multiplier /tbCarrySave8x8/DUT/*
add wave -group multiplier /tbCarrySave8x8/DUT/s
add wave -group multiplier /tbCarrySave8x8/DUT/cin
run -all
