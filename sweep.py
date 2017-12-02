import argparse
import numpy as np
import subprocess
from mpi4py import MPI
import os

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()


def parse_args():
    parser = argparse.ArgumentParser("Sweep parameter range")
    parser.add_argument("-L", type=float, default=100.0, help="System size")
    parser.add_argument("-N", type=int, default=10, help="Number of puffs in random init_mode.")
    parser.add_argument("-d", type=float, default=0.1, help="Interaction distance")
    parser.add_argument("-T", type=float, default=100000.0, help="Total time")
    parser.add_argument("-dt", type=float, default=0.01, help="Timestep")
    parser.add_argument("-w", type=float, default=0.0005,
                        help="Mean of the two rates at the critical point")
    parser.add_argument("-R_min", type=float, default=0.000001,
                        help="Control parameter MIN")
    parser.add_argument("-R_max", type=float, default=0.00005,
                        help="Control parameter MAX")
    parser.add_argument("-R_n", type=int, default=30,
                        help="Control parameter NUM STEPS")
    parser.add_argument("-A", type=float, default=50.0,
                        help="Short-range amplification/reduction factor.")
    parser.add_argument("-D", type=float, default=0.001, help="Diffusion coefficient.")
    parser.add_argument("-S", type=int, default=5,
                        help="Number of samples per parameter set.")
    parser.add_argument("--verbose", action="store_true", help="Verbose output.")
    parser.add_argument("--log_gaps", action="store_true", help="Log gaps.")
    parser.add_argument("--dump_pos", action="store_true", help="Dump positions.")
    parser.add_argument("--stat_intv", type=int, default=1000, help="Statistics interval.")
    parser.add_argument("--dump_intv", type=int, default=1000, help="Dump interval")
    parser.add_argument("--init_mode", choices=["mf", "random", "equid"], default="equid", help="Initialization mode.")
    args = parser.parse_args()
    return args


def param2str(key, val):
    if isinstance(val, bool):
        return "{}={}".format(key, "True" if val else "False")
    return "{}={}".format(key, val)


def main():
    args = parse_args()

    w = args.w
    R_ = np.linspace(args.R_min, args.R_max, args.R_n)

    wd_ = w - R_
    ws_ = w + R_

    n_sets = len(R_)
    param_sets = zip(range(n_sets), wd_, ws_, R_)

    cmd_kwargs = []
    
    rho = np.zeros((n_sets, args.S))
    for i, wd, ws, R in param_sets[rank::size]:
        # ./puff L=500.0 N=10 d=0.1 wd=0.0005 ws=0.00055 A=50.0 T=100000.0 dt=0.01
        # D=0.001 dump_pos=false results_folder=results/15 verbose=False
        # stat_intv=1000 dump_intv=1000 init_mode=mf log_gaps=False
        command = ["./puff", param2str("L", args.L),
                   param2str("N", args.N), param2str("d", args.d),
                   param2str("wd", wd), param2str("ws", ws),
                   param2str("A", args.A),
                   param2str("T", args.T), param2str("dt", args.dt),
                   param2str("D", args.D), param2str("dump_pos", args.dump_pos),
                   param2str("verbose", args.verbose), param2str("log_gaps", args.log_gaps),
                   param2str("init_mode", args.init_mode), param2str("stat_intv", args.stat_intv),
                   param2str("dump_intv", args.dump_intv)]
        for s in xrange(args.S):
            print "Process {}: Set {} of {}, sample {} of {}.".format(
                rank, i, n_sets, s, args.S)
            results_folder = os.path.join("results",
                "L{}_d{}_T{}_dt{}_D{}_w{}_A{}".format(
                    args.L, args.d, args.T, args.dt, args.D, w, args.A),
                "R{}".format(R), "{}".format(s))
            cmd = command + [param2str("results_folder", results_folder)]
            output = subprocess.check_output(cmd)
            with open(os.path.join(results_folder, "output.dat"), "w") as ofile:
                ofile.write(output)


if __name__ == "__main__":
    main()
