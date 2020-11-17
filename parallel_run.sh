#!/bin/bash
#
# run all command in file parallel_parmams.txt using N nodes (here 11)
#
parallel -j 11 :::: parallel_params.txt
