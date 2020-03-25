vlib work
vlog *.sv
vsim -novopt tb
add wave /tb/*
add wave -group cpu /tb/dut/*
add wave -group control /tb/dut/cpu_ctrl/*
add wave -group datapath /tb/dut/cpu_dp/*
run -all
