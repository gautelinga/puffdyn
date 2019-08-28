import matplotlib.pyplot as plt
import matplotlib.colors as colors
import matplotlib.cm as cmx
import glob
import numpy as np
import argparse
import os
import operator
import pandas as pd
from scipy.optimize import curve_fit
from scipy import odr
from scipy.stats import norm
import rates


XT_PREFIX = "/xt_"
XT_SUFFIX = ".dat"


def bootstrap(x, y, nbins=50):
    x_min = x.min()-1e-9
    x_max = x.max()+1e-9
    binned_data = [[] for _ in range(nbins)]
    for pt in zip(x, y):
        x_loc = pt[0]
        i = int(np.floor(nbins*(x_loc-x_min)/(x_max-x_min)))
        binned_data[i].append(pt)

    for i in range(len(binned_data)):
        binned_data[i] = np.array(binned_data[i])

    data_bs = []
    for bin_data in binned_data:
        N = len(bin_data)
        if N > 2:
            x_mean = np.mean(bin_data[:, 0])
            x_std = np.std(bin_data[:, 0])/np.sqrt(N)
            y_mean = np.mean(bin_data[:, 1])
            y_std = np.std(bin_data[:, 1])/np.sqrt(N)

            if bool(np.isfinite(x_mean)
                    and np.isfinite(y_mean)
                    and np.isfinite(x_std)
                    and np.isfinite(y_std)):
                data_bs.append((x_mean, x_std, y_mean, y_std))
    data_bs = np.array(data_bs)
    return data_bs[:, :2], data_bs[:, 2:]


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


def loadtxt(filename):
    pddata = pd.read_csv(filename, delimiter=" ", header=None)
    return pddata.values


def parse_args():
    parser = argparse.ArgumentParser(description="Reverse engineer")
    parser.add_argument("folder", type=str, help="Folder")
    parser.add_argument("-t_min", type=float, default=0.0, help="t_min")
    parser.add_argument("-t_max", type=float, default=0.0, help="t_max")
    parser.add_argument("-dit", type=int, default=1, help="dit")
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

    tdata = np.loadtxt(tdata_filename)
    t_all = tdata[:, 0]
    x_mean_all = tdata[:, 1]
    u_mean = (x_mean_all[-1]-x_mean_all[0])/(t_all[-1]-t_all[0])

    series = dict()
    t_max = 0.0
    for filename in glob.glob(args.folder + XT_PREFIX + "*" + XT_SUFFIX):
        key = int(str(filename).split(XT_PREFIX)[-1].split(XT_SUFFIX)[0])
        data = loadtxt(filename)
        print(filename, key)
        if data.ndim > 1:
            t = data[:, 0]
            x = (data[:, 1]-u_mean*t) % L
            t_max = max(t.max(), t_max)
            t_min = args.t_min
            if args.t_max > 0:
                t_max = args.t_max
            ids = np.logical_and(t > t_min, t < t_max)
            t = t[ids]
            x = x[ids]
            abs_dx = np.abs(np.diff(x))
            mask = np.hstack([abs_dx > 0.5*L, [False]])
            masked_data = np.ma.MaskedArray(x, mask)
            series[key] = (t, masked_data)

    if len(series):
        import colormaps
        cols = colormaps._parula_data
        ax = plt.gca()
        ax.set_facecolor(cols[-1])

        order = dict()
        pos = dict()
        for key, (t_, x_) in series.items():
            pos[key] = dict()
            plt.plot(x_, t_, color=cols[0], linewidth=.2)
            for t, x in zip(t_, x_):
                if t in order:
                    order[t].append((key, x))
                else:
                    order[t] = [(key, x)]
                pos[key][t] = x

        plt.xlim(0, L)
        plt.ylim(t_min, t_max)
        ax.invert_yaxis()
        plt.ylabel("Time")
        plt.xlabel("Space (co-moving)")
        plt.show()

        for t in order:
            order[t].sort(key=operator.itemgetter(1))

        order_red = dict()
        for t in order:
            order_red[t] = []
            if len(order[t]) > 1:
                for ic, c in order[t]:
                    if ic not in order_red[t]:
                        order_red[t].append(ic)
        for i in range(0, args.dit):
            dit = 2**i
            print("dit =", dit)
            approximate(order_red, pos, dit, t_min, t_max, L, u_mean)


def func(x, a, b, c):
    return a + b*np.exp(-c*x)


def func2(p, x):
    a, b, c = p
    return func(x, a, b, c)


def approximate(order_red, pos, dit, t_min, t_max, L, u_mean):
    t_ = np.array(sorted(list(order_red.keys())))
    dt_ = np.diff(t_)
    dt = dt_.max()*dit
    t_ = np.arange(t_min+dt, t_max-dt, dt)

    data = []
    for t in t_:
        ic_ = order_red[t]
        for i in range(1, len(ic_)):
            ic = ic_[i]
            ic_upst = ic_[i-1]
            if t + dt in pos[ic]:
                x_this = pos[ic][t]
                x_upst = pos[ic_upst][t]
                x_next = pos[ic][t+dt]
                
                dx = x_next - x_this
                if dx < -L/2:
                    dx += L
                elif dx > L/2:
                    dx -= L
                dxdt = dx/dt
                l_upst = x_this - x_upst

                if np.isfinite(dxdt) and np.isfinite(l_upst):
                    data.append((dxdt, l_upst))

    print("number of data points:", len(data))

    data = np.array(data)
    ids = np.logical_and(np.isfinite(data[:, 0]),
                         np.isfinite(data[:, 1]))
    data = data[ids]

    l_bs, dx_bs = bootstrap(data[:, 1], data[:, 0], nbins=200)

    odr_data = odr.RealData(l_bs[:, 0], dx_bs[:, 0],
                            sx=l_bs[:, 1], sy=dx_bs[:, 1])
    odr_obj = odr.ODR(odr_data, odr.Model(func2),
                      beta0=(-u_mean, rates.v_0, 1./rates.l_c))
    out = odr_obj.run()
    popt_odr = out.beta

    #popt, pcov = curve_fit(func, data[:, 1], data[:, 0],
    #                       p0=popt_odr)
        
    plt.figure()
    # plt.scatter(data[:, 1], data[:, 0])
    x = np.linspace(data[:, 1].min(), data[:, 1].max(), 1000)
    plt.plot(x, func(x, *popt_odr), 'r-')
    #plt.plot(x, func(x, *popt), 'y-')
    plt.plot(x, func(x, -u_mean, rates.v_0, 1./rates.l_c), 'k')
    plt.errorbar(l_bs[:, 0], dx_bs[:, 0],
                 xerr=l_bs[:, 1], yerr=dx_bs[:, 1], fmt='m')

    plt.figure()
    plt.hist2d(data[:, 1], data[:, 0],
               bins=(200, 200))
    plt.show()

    v_inf = popt_odr[0]
    v_0 = popt_odr[1]
    l_c = 1./popt_odr[2]

    print("v_inf =", v_inf, "(compare with", u_mean, "?)")
    print("v_0   =", v_0)
    print("l_c   =", l_c)

    dz = data[:, 0] - func(data[:, 1], *popt_odr)

    mu, sigma = norm.fit(dz)
    print("mu    =", mu)
    print("sigma =", sigma)
    D = sigma**2*dt
    print("D     =", D)

    print(dit, dt, v_inf, v_0, l_c, D)

if __name__ == "__main__":
    main()
