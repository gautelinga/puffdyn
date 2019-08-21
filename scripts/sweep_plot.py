from __future__ import print_function
import argparse
import matplotlib.pyplot as plt
import glob
import os
import numpy as np
import pickle
import h5py
from rates import Re_c, l_c, alpha_d, alpha_s, beta_d, beta_s


def parse_args():
    parser = argparse.ArgumentParser(description="Plot sweep.")
    parser.add_argument("folder", type=str, help="Folder with data.")
    parser.add_argument("-S", type=int, default=None, help="Samples per R.")
    parser.add_argument("-Re_c", type=float, default=Re_c, help="Overrule R_c")
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

    params = dict([split_str(elem) for elem in
                   os.path.basename(os.path.dirname(args.folder)).split("_")])
    L = params["L"]
    T = params["T"]

    datasets = dict()
    print("Looking for files.")
    dump_fname = os.path.join(args.folder, "temp_S{}.hdf".format(args.S))
    if os.path.exists(dump_fname) and False:
        with h5py.File(dump_fname, "r") as h5f:
            for grp in h5f:
                datasets[float(grp)] = [np.array(h5f[grp][key])
                                        for key in h5f[grp]]
    else:
        for foldername in glob.glob(os.path.join(args.folder, "*")):
            if os.path.isdir(foldername):
                last_foldername = foldername.split("/")[-1]
                key, val = split_str(last_foldername)
                assert(key == "Re")
                datasets[val] = []
                for i, s in enumerate(glob.glob(
                        os.path.join(foldername, "*"))):
                    if args.S is None or i < args.S:
                        data_loc = np.loadtxt(os.path.join(s, "tdata.dat"))
                        datasets[val].append(data_loc)
        with h5py.File(dump_fname, "w") as h5f:
            for Re, dataset in datasets.items():
                grp = h5f.create_group(str(Re))
                for i, data_loc in enumerate(dataset):
                    h5f.create_dataset(str(Re) + "/" + str(i), data=data_loc)

    len_glob = 0
    for dataset in datasets.values():
        for data_loc in dataset:
            len_glob = max(len_glob, len(data_loc))

    print("len_glob =", len_glob)
    num_avg = int(len_glob/10)
    print("num_avg =", num_avg)
    
    #fig, ax = plt.subfigures()
    print("Computing...")
    Re_ = np.array(sorted(datasets.keys()))

    rho_ = np.zeros(len(Re_))
    rho_var_ = np.zeros(len(Re_))
    t_ = np.zeros(len(Re_))
    t_var_ = np.zeros(len(Re_))
    rhot = []
    for i, Re in enumerate(Re_):
        dsets = datasets[Re]
        len_min = np.min([len(dset) for dset in dsets])
        t_list = []
        rho_list = []
        rho_mean = np.zeros(len(dsets))
        t_loc = np.zeros(len(dsets))
        for j, data in enumerate(dsets):
            t_list.append(data[:len_min, 0])
            t_loc[j] = data[-1, 0]
            rho_list.append(data[:len_min, 2]*l_c/L)
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
        rhot.append((Re, t_avg, rho_avg))

    print("Re_c =", args.Re_c)

    rho0_ = np.zeros_like(Re_)
    rhoT_ = np.zeros_like(Re_)
    for i, (Re, t_avg, rho_avg) in enumerate(rhot):
        rho0_[i] = rho_avg[0]
        if t_avg[-1] < T:
            rhoT_[i] = 0.
        else:
            rhoT_[i] = rho_avg[-1]
    rho_anal = -1./np.log(-(
        alpha_d(Re_)-alpha_s(Re_))/(beta_d(Re_)-beta_s(Re_)))

    plt.plot(Re_, rho0_, label="Init")
    plt.plot(Re_, rhoT_, label="End")
    plt.plot(Re_, rho_anal,
             label="Analytical")
    plt.legend()
    plt.xlabel("$Re$")
    plt.ylabel("$\\rho$")

    plt.figure()
    plt.plot(Re_, rho0_**(1./beta), label="Init")
    plt.plot(Re_, rhoT_**(1./beta), label="End")
    plt.plot(Re_, rho_anal**(1./beta),
             label="Analytical")
    plt.legend()
    plt.xlabel("$Re$")
    plt.ylabel("$\\rho^{1/\\beta}$")
    
    plt.show()
    
    plt.figure()
    for Re, t_avg, rho_avg in rhot:
        #print(len(t_avg), len(rho_avg))
        plt.plot(t_avg, rho_avg)

    ax = plt.gca()
    ax.set_xscale("log", nonposx='clip')
    ax.set_yscale("log", nonposy='clip')
    plt.xlabel("$t$")
    plt.ylabel("$\\rho_t$")
    plt.show()

    for Re, t_avg, rho_avg in rhot:
        t = t_avg[:]
        rho = rho_avg[:]
        abs_eps = abs(Re-Re_c)
        plt.plot(t*abs_eps**nu_t, t**alpha*rho)

    ax = plt.gca()
    ax.set_xscale("log", nonposx='clip')
    ax.set_yscale("log", nonposy='clip')
    plt.xlabel("$t|\\epsilon|^{\\nu_t}$")
    plt.ylabel("$t^{\\alpha} \\rho_t$")
    plt.show()

    # t_std_ = np.sqrt(t_var_)
    # fig = plt.figure()
    # ax = plt.gca()
    # plt.plot(Re_, (np.log10(t_-t_std_)))
    # plt.plot(Re_, (np.log10(t_)))
    # plt.plot(Re_, (np.log10(t_+t_std_)))
    # plt.plot([Re_c, Re_c], [np.log10(t_.min()), np.log10(t_.max())])
    # ax.set_yscale("log")
    # plt.xlabel("$R$")
    # plt.ylabel("$\\log_{10} t$")
    # plt.show()
    
    # M = 6
    # R__ = np.zeros(len(Re_)-M)
    # rho__ = np.zeros(len(rho_)-M)
    # for m in range(M):
    #     R__ += Re_[m:len(Re_)-M+m]
    #     rho__ += rho_[m:len(rho_)-M+m]
    # Re__ /= M
    # rho__ /= M

    # plt.errorbar(Re_, rho_, np.sqrt(rho_var_))
    # #plt.plot(R__, rho__)
    # plt.xlabel("$R$")
    # plt.ylabel("$\\rho$")
    # plt.plot([Re_c, Re_c], [0., rho_.max()])
    # plt.show()

    # np.savetxt(os.path.join(args.folder, "rho_vs_R.dat"),
    #            np.vstack((Re_, rho_, np.sqrt(rho_var_))).T)

    # plt.plot(Re_, rho_**(1./beta))
    # plt.plot([Re_c, Re_c], [0., rho_.max()**(1./beta)])
    # plt.xlabel("$R$")
    # plt.ylabel("$\\rho^{1/\\beta}$")
    # plt.show()

    # eps_ = Re_-Re_c
    # rho__ = rho_[eps_ > 0.]
    # eps__ = eps_[eps_ > 0.]
    # rho___ = rho__[rho__ > 0.]
    # eps___ = eps__[rho__ > 0.]
    # log_rho__ = np.log10(rho___)
    # log_eps__ = np.log10(eps___)

    # pp = np.polyfit(log_eps__, log_rho__, 1)
    # plt.plot(log_eps__, log_rho__)
    # plt.plot(log_eps__, pp[0]*log_eps__ + pp[1])
    # plt.xlabel("$\\log_{10} R$")
    # plt.ylabel("$\\log_{10} \\epsilon$")
    # plt.show()

    # print("beta_est =", pp[0])


if __name__ == "__main__":
    main()
