#!/bin/bash
#
#--------------------------------------------------
#example script for running a whole single Re simulation:
#1st argument: Re
#2nd argument: L
#--------------------------------------------------
#
./runScriptLtome.sh $1 $2 0 && ./runScriptLtome.sh $1 $2 1 && ./runScriptLtome.sh $1 $2 2 && ./runScriptLtome.sh $1 $2 3 && ./runScriptLtome.sh $1 $2 4
