import numpy as np
import argparse
import os
import matplotlib.pyplot as plt
import pickle


parser = argparse.ArgumentParser(description="test")
parser.add_argument("folder", type=str, help="Folder to look in")
parser.add_argument("outfile", type=str, help="File to store")
args = parser.parse_args()

subfolders = os.listdir(args.folder)

out = dict()
for subfolder in subfolders:
    if "_" in subfolder:
        parts = subfolder.split("_")
        if len(parts) == 3 and parts[0][0] == "L" and parts[1][:2] == "Re" and parts[2] == "4":
            L = float(parts[0][1:])
            Re = float(parts[1][2:])
            print(subfolder, L, Re)
            if L not in out:
                out[L] = dict()
            data = np.loadtxt(os.path.join(args.folder, subfolder, "tdata.dat"))
            # plt.plot(data[:, 0], data[:, 2]/L)
            # plt.show()

            N = len(data[:, 0])
            m1 = np.mean(data[:, 2])
            m2 = np.mean(data[:, 2]**2)
            m4 = np.mean(data[:, 2]**4)
            out[L][Re] = np.array([N, m1, m2, m4])

pickle.dump(out, open(args.outfile, "w"))
