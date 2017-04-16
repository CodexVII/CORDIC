#!/bin/bash

# Bash script to compile and run the verilog CORDIC code

iverilog -o ./Verilog_Code/CORDIC ./Verilog_Code/cordic_testbench.v ./Verilog_Code/cordic.v

vvp ./Verilog_Code/CORDIC

rm -f ./cordic_testbench.vcd*
