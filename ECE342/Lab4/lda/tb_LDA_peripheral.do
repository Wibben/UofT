vlib work
vlog -sv *.sv
vsim -novopt tb_LDA_peripheral
add wave /tb_LDA_peripheral/*
add wave -group LDA_peripheral /tb_LDA_peripheral/DUT/*
run -all
