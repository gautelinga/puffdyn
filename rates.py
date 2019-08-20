import numpy as np
import matplotlib.pyplot as plt
import argparse


a_d = -9.187149761728774
b_d = 67.45338604868417
c_d = 0.005897608616540853
d_d = -0.036872821458203905

a_s = 9.287142828546877
b_s = -8.583449491545009
c_s = -0.0031645844211272835
d_s = 0.0047135153271309575


def alpha_d(Re):
    return a_d + c_d*Re


def alpha_s(Re):
    return a_s + c_s*Re


def beta_d(Re):
    return b_d + d_d*Re


def beta_s(Re):
    return b_s + d_s*Re


Re_c = - (a_d-a_s)/(c_d-c_s)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Compute rate functions...")
    parser.add_argument("--plot", action="store_true", help="Plot")
    parser.add_argument("Re", type=float, default=Re_c, help="Re")
    args = parser.parse_args()

    print("alpha_d(Re={}) = {}".format(args.Re, alpha_d(args.Re)))
    print("beta_d(Re={})  = {}".format(args.Re, beta_d(args.Re)))
    print("alpha_s(Re={}) = {}".format(args.Re, alpha_s(args.Re)))
    print("beta_s(Re={})  = {}".format(args.Re, beta_s(args.Re)))

    if args.plot:
        print("Re_c =", Re_c)
        Re = np.linspace(Re_c, 2100, 100)
        plt.plot(Re, alpha_d(Re))
        plt.plot(Re, beta_d(Re))
        plt.plot(Re, alpha_s(Re))
        plt.plot(Re, beta_s(Re))
        plt.show()

    
