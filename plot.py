import matplotlib.pyplot as plt
import matplotlib.colors as colors
import matplotlib.cm as cmx
import glob
import numpy as np
import argparse
import os

XT_PREFIX = "/xt_"
XT_SUFFIX = ".dat"


def parse_params(text):
    params = dict()
    param_strings = text.strip().split(" ")
    for param_string in param_strings:
        key, val = param_string.split("=")
        key = str(key)
        val = str(val)
        try:
            val = eval(val)
        except:
            pass
        params[key] = val
    return params


def parse_args():
    parser = argparse.ArgumentParser(description="Plot")
    parser.add_argument("folder", type=str, help="Folder")
    return parser.parse_args()


def main():
    args = parse_args()

    params_filename = os.path.join(args.folder, "params.dat")
    if os.path.exists(params_filename):
        with open(params_filename, "r") as params_file:
            text = params_file.read().replace("\n", " ")
            params = parse_params(text)
    L = float(params["L"])

    tdata_filename = os.path.join(args.folder, "tdata.dat")
    if os.path.exists(tdata_filename):
        tdata = np.loadtxt(tdata_filename)
        t_all = tdata[:, 0]
        x_mean_all = tdata[:, 1]
        plt.plot(t_all, x_mean_all)
        plt.show()

        plt.plot(t_all, tdata[:, 2])
        plt.show()
    else:
        exit()
    u_mean = (x_mean_all[-1]-x_mean_all[0])/(t_all[-1]-t_all[0])

    # gaps_filename = args.folder + "/gaps.dat"
    # if os.path.exists(gaps_filename):
    for gaps_filename in glob.glob(args.folder + "gaps_*.dat"):
        xgaps = np.loadtxt(gaps_filename)
        pgaps = np.arange(1, len(xgaps)+1)/float(len(xgaps))
        # plt.plot(xgaps, pgaps)
        # plt.show()
        plt.semilogy(xgaps, 1.-pgaps)
    plt.show()

    series = dict()
    for filename in glob.glob(args.folder + XT_PREFIX + "*" + XT_SUFFIX):
        key = int(str(filename).split(XT_PREFIX)[-1].split(XT_SUFFIX)[0])
        data = np.loadtxt(filename)
        print filename, key
        if data.ndim > 1:
            t = data[:, 0]
            x = (data[:, 1]-u_mean*t) % L

            abs_dx = np.abs(np.diff(x))
            mask = np.hstack([ abs_dx > 0.5*L, [False]])
            masked_data = np.ma.MaskedArray(x, mask)
            series[key] = (t, masked_data)

    if len(series):
        keys = np.array(series.keys())
        cmap = plt.get_cmap("viridis")
        cnorm = colors.Normalize(vmin=keys.min(), vmax=keys.max())
        scalar_map = cmx.ScalarMappable(norm=cnorm, cmap=cmap)

        for key, (t, x) in series.items():
            cval = scalar_map.to_rgba(key)
            plt.plot(t, x, color=cval)
        plt.show()

if __name__ == "__main__":
    main()