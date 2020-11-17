#!/bin/bash
#
#--------------------------------------------------
#example script for running a whole single Re simulation:
#1st argument: Re
#2nd argument: L
#3rd argument: rate_amplification
#4th argument: how many stages of simulation?
#--------------------------------------------------
#
for ((i=0; i<=$4; i++))
do
    ./runScriptLta.sh $1 $2 $3 $i
done
