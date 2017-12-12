from __future__ import print_function
import argparse
import matplotlib.pyplot as plt
import glob
import os
import numpy as np
import cPickle as pickle
import h5py


def parse_args():
    parser = argparse.ArgumentParser(description="Plot sweep.")
    parser.add_argument("folder", type=str, help="Folder with data.")
    parser.add_argument("-S", type=int, default=None, help="Samples per R.")
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
    beta = 0.276486
    nu_t = 1.733847
    alpha = beta/nu_t

    args = parse_args()

    datasets = dict()
    print("Looking for files.")
    dump_fname = os.path.join(args.folder, "temp_S{}.hdf".format(args.S))
    if os.path.exists(dump_fname):
        with h5py.File(dump_fname, "r") as h5f:
            for grp in h5f:
                datasets[float(grp)] = [np.array(h5f[grp][key])
                                        for key in h5f[grp]]
    else:
        for foldername in glob.glob(os.path.join(args.folder, "*")):
            if os.path.isdir(foldername):
                last_foldername = foldername.split("/")[-1]
                key, val = split_str(last_foldername)
                assert(key == "R")
                datasets[val] = []
                for i, s in enumerate(glob.glob(
                        os.path.join(foldername, "*"))):
                    if args.S is None or i < args.S:
                        data_loc = np.loadtxt(os.path.join(s, "tdata.dat"))
                        datasets[val].append(data_loc)
        with h5py.File(dump_fname, "w") as h5f:
            for R, dataset in datasets.iteritems():
                grp = h5f.create_group(str(R))
                for i, data_loc in enumerate(dataset):
                    h5f.create_dataset(str(R) + "/" + str(i), data=data_loc)

    len_glob = 0
    for dataset in datasets.itervalues():
        for data_loc in dataset:
            len_glob = max(len_glob, len(data_loc))

    print("len_glob =", len_glob)
    num_avg = int(len_glob/50)
                    
    #fig, ax = plt.subfigures()
    print("Computing...")
    R_ = np.array(sorted(datasets.keys()))
    rho_ = np.zeros(len(R_))
    rho_var_ = np.zeros(len(R_))
    t_ = np.zeros(len(R_))
    t_var_ = np.zeros(len(R_))
    rhot = []
    for i, R in enumerate(R_):
        dsets = datasets[R]
        len_min = np.min([len(dset) for dset in dsets])
        t_list = []
        rho_list = []
        rho_mean = np.zeros(len(dsets))
        rho_var = np.zeros(len(dsets))
        t_loc = np.zeros(len(dsets))
        for j, data in enumerate(dsets):
            t_list.append(data[:len_min, 0])
            t_loc[j] = data[-1, 0]
            rho_list.append(data[:len_min, 2])
            if len(data) < len_glob-num_avg:
                rho_mean[j] = 0.
            else:
                j_start = min(len(data)-1, len_glob-num_avg-1)
                rho_loc = data[j_start:len(data), 2]
                rho_mean[j] = np.sum(rho_loc)/float(num_avg)
        t_[i] = np.mean(t_loc)
        t_var_[i] = np.var(t_loc)
        if True or np.all(rho_mean > 0.0):
            rho_[i] = np.mean(rho_mean)
            rho_var_[i] = np.var(rho_mean)
        t = np.vstack(tuple(t_list))
        rho = np.vstack(tuple(rho_list))
        t_avg = t.mean(0)
        rho_avg = rho.mean(0)
        rhot.append((R, t_avg, rho_avg))

    id_c = t_ == t_.max()
    R_c = np.mean(R_[id_c][1:2])
    print("R_c =", R_c)

    for R, t_avg, rho_avg in rhot:
        #print(len(t_avg), len(rho_avg))
        plt.plot(t_avg, rho_avg)

    ax = plt.gca()
    ax.set_xscale("log", nonposx='clip')
    ax.set_yscale("log", nonposy='clip')
    plt.xlabel("$t$")
    plt.ylabel("$\\rho_t$")
    plt.show()

    for R, t_avg, rho_avg in rhot:
        t = t_avg[1000:]
        rho = rho_avg[1000:]
        abs_eps = abs(R-R_c)
        plt.plot(t*abs_eps**nu_t, t**alpha*rho)

    ax = plt.gca()
    ax.set_xscale("log", nonposx='clip')
    ax.set_yscale("log", nonposy='clip')
    plt.xlabel("$t|\\epsilon|^{\\nu_t}$")
    plt.ylabel("$t^{\\alpha} \\rho_t$")
    plt.show()

    t_std_ = np.sqrt(t_var_)
    fig = plt.figure()
    ax = plt.gca()
    plt.plot(R_, (np.log10(t_-t_std_)))
    plt.plot(R_, (np.log10(t_)))
    plt.plot(R_, (np.log10(t_+t_std_)))
    plt.plot([R_c, R_c], [np.log10(t_.min()), np.log10(t_.max())])
    ax.set_yscale("log")
    plt.xlabel("$R$")
    plt.ylabel("$\\log_{10} t$")
    plt.show()
    
    plt.errorbar(R_, rho_, np.sqrt(rho_var_))
    plt.xlabel("$R$")
    plt.ylabel("$\\rho$")
    plt.plot([R_c, R_c], [0., rho_.max()])
    plt.show()

    plt.plot(R_, rho_**(1./beta))
    plt.plot([R_c, R_c], [0., rho_.max()**(1./beta)])
    plt.xlabel("$R$")
    plt.ylabel("$\\rho^{1/\\beta}$")
    plt.show()

    eps_ = R_-R_c
    rho__ = rho_[eps_ > 0.]
    eps__ = eps_[eps_ > 0.]
    log_rho__ = np.log10(rho__)
    log_eps__ = np.log10(eps__)

    pp = np.polyfit(log_eps__, log_rho__, 1)
    plt.plot(log_eps__, log_rho__)
    plt.plot(log_eps__, pp[0]*log_eps__ + pp[1])
    plt.xlabel("$\\log_{10} R$")
    plt.ylabel("$\\log_{10} \\epsilon$")
    plt.show()

    print("beta_est =", pp[0])


if __name__ == "__main__":
    main()
