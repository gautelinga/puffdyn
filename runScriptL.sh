#!/bin/bash
#
#--------------------------------------------------
#example script for running a single Re simulation:
#1st argument: Re
#2nd argument: L
#3rd argument: restart option:
#		0: start from equally spaced puffs for t=[0 1e7]
#		1: restart from previous run for t=[1e7 1e8]
#		2: restart from previous run for t=[1e8 1e9]
#		3: restart from previous run for t=[1e9 1e10]
#--------------------------------------------------
#
#
if [ $3 == "0" ]; then
    ./puff L=$2 dt=10 D=0.02 T=1e7 stat_intv=1000 Re=$1 results_folder=results/L$2_Re$1
elif [ $3 == "1" ]; then
    ./puff L=$2 dt=10 D=0.02 T=9e7 stat_intv=10000 Re=$1 results_folder=results/L$2_Re$1_1 init_mode=fromFile importFile=results/L$2_Re$1/puffsPosition_final.dat
elif [ $3 == "2" ]; then
    ./puff L=$2 dt=10 D=0.02 T=9e8 stat_intv=100000 Re=$1 results_folder=results/L$2_Re$1_2 init_mode=fromFile importFile=results/L$2_Re$1_1/puffsPosition_final.dat
elif [ $3 == "3" ]; then
    ./puff L=$2 dt=10 D=0.02 T=9e9 stat_intv=1000000 Re=$1 results_folder=results/L$2_Re$1_3 init_mode=fromFile importFile=results/L$2_Re$1_2/puffsPosition_final.dat
else
    echo "Third argument is missing or invalid."
fi
