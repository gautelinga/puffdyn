import numpy as np

part1 = "./runLta.sh"
Re_ = list(np.linspace(2040, 2100, 61))
L_ = [5e3, 1e4, 2e4, 4e4, 8e4]
rate_amp = 1000.0
stages = 3

ofile = open("parallel_params_ta.txt", "w")
for Re in Re_:
    for L in L_:
        string = "{} {} {} {} {}\n".format(part1, Re, L, rate_amp, stages)
        ofile.write(string)
ofile.close()
