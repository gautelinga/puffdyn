#!/bin/bash
#
# run all command in file parallel_parmams.txt using N nodes
#
parallel -j 24 :::: parallel_params_tome.txt
