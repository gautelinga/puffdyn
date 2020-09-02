#!/bin/bash
#
#--------------------------------------------------
#example script for running a single Re simulation:
#1st argument: Re
#2nd argument: restart option:
#		0: start from equally spaced puffs for t=[0 1e7]
#		1: restart from previous run for t=[1e7 1e8]
#		2: restart from previous run for t=[1e8 1e9]
#		3: restart from previous run for t=[1e9 1e10]
#--------------------------------------------------
#
#
if [ $2 == "0" ]; then
  ../puffDyn/build/puffDyn L=1e6 dt=10 D=0.02 T=1e7 stat_intv=1000 Re=$1 results_folder=results/Re=$1
elif [ $2 == "1" ]; then
  ../puffDyn/build/puffDyn L=1e6 dt=10 D=0.02 T=9e7 stat_intv=10000 Re=$1 results_folder=results/Re=$1_1 init_mode=fromFile importFile=results/Re=$1/puffsPosition_final.dat
elif [ $2 == "2" ]; then
  ../puffDyn/build/puffDyn L=1e6 dt=10 D=0.02 T=9e8 stat_intv=100000 Re=$1 results_folder=results/Re=$1_2 init_mode=fromFile importFile=results/Re=$1_1/puffsPosition_final.dat
elif [ $2 == "3" ]; then
  ../puffDyn/build/puffDyn L=1e6 dt=10 D=0.02 T=9e9 stat_intv=1000000 Re=$1 results_folder=results/Re=$1_3 init_mode=fromFile importFile=results/Re=$1_2/puffsPosition_final.dat
else
  echo "Second argument is missing or invalid."
fi
