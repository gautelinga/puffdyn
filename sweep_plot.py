import argparse
import matplotlib.pyplot as plt
import glob
import os
import numpy as np


def parse_args():
    parser = argparse.ArgumentParser(description="Plot sweep.")
    parser.add_argument("folder", type=str, help="Folder with data.")
    args = parser.parse_args()
    return args


def split_str(string):
    for i, s in enumerate(string):
        if not s.isalpha():
            break
    key = str(string[:i])
    val = float(string[i:])
    return key, val


def main():
    args = parse_args()

    datasets = dict()
    for foldername in glob.glob(os.path.join(args.folder, "*")):
        if os.path.isdir(foldername):
            last_foldername = foldername.split("/")[-1]
            key, val = split_str(last_foldername)
            assert(key == "R")
            datasets[val] = [np.loadtxt(os.path.join(s, "tdata.dat")) for s in glob.glob(os.path.join(foldername, "*"))]

    #fig, ax = plt.subfigures()    
    R_ = sorted(datasets.keys())
    rho_ = np.zeros(len(R_))
    for i, key in enumerate(R_):
        dsets = datasets[key]
        len_min = np.min([len(dset) for dset in dsets])
        t_list = []
        rho_list = []
        for data in dsets:
            t_list.append(data[:len_min, 0])
            rho_list.append(data[:len_min, 2])
            rho_[i] += data[-1, 2]/float(len(dsets))
        t = np.vstack(tuple(t_list))
        rho = np.vstack(tuple(rho_list))
        t_avg = t.mean(0)
        rho_avg = rho.mean(0)
        #print len(t_avg), len(rho_avg)
        plt.plot(t_avg, rho_avg)

    ax = plt.gca()
    ax.set_xscale("log", nonposx='clip')
    ax.set_yscale("log", nonposy='clip')
    #fig = plt.figure()
    plt.show()

    plt.plot(R_, rho_)
    plt.show()


if __name__ == "__main__":
    main()