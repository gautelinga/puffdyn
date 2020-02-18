import numpy as np
import argparse
import os
import matplotlib.pyplot as plt


def parse_args():
    parser = argparse.ArgumentParser(description="Plot sweep.")
    parser.add_argument("folder", type=str, help="Folder with data.")
    args = parser.parse_args()
    return args


def main():
    beta = 0.276486
    nu_t = 1.733847
    alpha = beta/nu_t

    args = parse_args()

    Re_folders = dict()
    for folder in os.listdir(args.folder):
        if folder[:2] == "Re" and os.path.exists(
                os.path.join(args.folder, folder, "0", "tdata.dat")):
            Re_loc = float(folder[2:])
            Re_folders[Re_loc] = os.path.join(args.folder, folder)
    Re_keys = sorted(Re_folders.keys())

    # fig = plt.figure()

    colors = plt.cm.jet(np.linspace(0, 1, len(Re_keys)))

    N = []
    for i, Re_loc in enumerate(Re_keys):
        tdata_loc = np.loadtxt(os.path.join(Re_folders[Re_loc],
                                            "0", "tdata.dat"))
        # print(tdata_loc.max(0))
        if tdata_loc[-1, 2] > 3:
            plt.plot(np.log10(tdata_loc[:, 0]), np.log10(tdata_loc[:, 2]),
                     label="{}".format(Re_loc), color=colors[i])

            n_init = int(0.5*len(tdata_loc[:, 0]))
            N_avg = np.mean(tdata_loc[n_init:, 2])
            x_loc = np.array([tdata_loc[n_init, 0],
                              tdata_loc[-1, 0]])
            plt.plot(np.log10(x_loc),
                     np.log10(N_avg*np.ones_like(x_loc)), color='k')
            N.append([Re_loc, N_avg])

    plt.show()

    N = np.array(N)
    plt.plot(N[:, 0], N[:, 1])
    plt.show()

    beta = 0.276486
    nu_t = 1.733847
    alpha = beta/nu_t
    
    plt.plot(N[:, 0], N[:, 1]**(1./beta))
    plt.show()

    fit_data = []
    for i, Re_loc in enumerate(Re_keys):
        tdata_loc = np.loadtxt(os.path.join(Re_folders[Re_loc], "0", "tdata.dat"))
        # print(tdata_loc.max(0))
        if tdata_loc[-1, 2] <= 3:
            plt.plot((tdata_loc[:, 0]), np.log10(tdata_loc[:, 2]),
                     label="{}".format(Re_loc), color=colors[i])

            n_init = len(tdata_loc[:, 0])//100
            poly = np.polyfit(
                tdata_loc[n_init:, 0],
                np.log10(tdata_loc[n_init:, 2]), 1)

            x_loc = np.array([tdata_loc[n_init, 0], tdata_loc[-1, 0]])
            plt.plot(x_loc, x_loc*poly[0]+poly[1], color='k')
            fit_data.append([Re_loc, 1./poly[0]])

    # plt.legend(loc="lower left")
    plt.show()

    fit_data = np.array(fit_data)
    plt.plot(fit_data[:, 0], fit_data[:, 1])
    plt.show()


if __name__ == "__main__":
    main()
