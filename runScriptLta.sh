#!/bin/bash
#
#--------------------------------------------------
#example script for running a single Re simulation:
#1st argument: Re
#2nd argument: L
#3rd argument: rate_amplification
#4th argument: restart option:
#		0: start from equally spaced puffs for t=[0 1e7]
#		1: restart from previous run for t=[1e7 1e8]
#		2: restart from previous run for t=[1e8 1e9]
#		3: restart from previous run for t=[1e9 1e10]
#--------------------------------------------------
#
#
FOLDER="results_ta"
DIFF="0.02"
TOME_MOD="true"
STEP_MOD="true"

if [ $4 == "0" ]; then
    ./puff L=$2 dt=10 D=0.02 T=1e7 stat_intv=1000 Re=$1 tome_mod=$TOME_MOD results_folder=$FOLDER/L$2_Re$1 rate_amplification=$3 step_mod=$STEP_MOD
elif [ $4 == "1" ]; then
    ./puff L=$2 dt=10 D=0.02 T=9e7 stat_intv=10000 Re=$1 tome_mod=$TOME_MOD results_folder=$FOLDER/L$2_Re$1_1 init_mode=fromFile importFile=$FOLDER/L$2_Re$1/puffsPosition_final.dat rate_amplification=$3 step_mod=$STEP_MOD
elif [ $4 == "2" ]; then
    ./puff L=$2 dt=10 D=0.02 T=9e8 stat_intv=100000 Re=$1 tome_mod=$TOME_MOD results_folder=$FOLDER/L$2_Re$1_2 init_mode=fromFile importFile=$FOLDER/L$2_Re$1_1/puffsPosition_final.dat rate_amplification=$3 step_mod=$STEP_MOD
elif [ $4 == "3" ]; then
    ./puff L=$2 dt=10 D=0.02 T=9e9 stat_intv=1000000 Re=$1 tome_mod=$TOME_MOD results_folder=$FOLDER/L$2_Re$1_3 init_mode=fromFile importFile=$FOLDER/L$2_Re$1_2/puffsPosition_final.dat rate_amplification=$3 step_mod=$STEP_MOD
elif [ $4 == "4" ]; then
    ./puff L=$2 dt=10 D=0.02 T=9e10 stat_intv=1000000 Re=$1 tome_mod=$TOME_MOD results_folder=$FOLDER/L$2_Re$1_4 init_mode=fromFile importFile=$FOLDER/L$2_Re$1_3/puffsPosition_final.dat rate_amplification=$3 step_mod=$STEP_MOD
elif [ $4 == "5" ]; then
    ./puff L=$2 dt=10 D=0.02 T=9e11 stat_intv=10000000 Re=$1 tome_mod=$TOME_MOD results_folder=$FOLDER/L$2_Re$1_5 init_mode=fromFile importFile=$FOLDER/L$2_Re$1_4/puffsPosition_final.dat rate_amplification=$3 step_mod=$STEP_MOD
else
    echo "Fourth argument is missing or invalid."
fi
