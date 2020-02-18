import argparse
import numpy as np
import os
from rates import alpha_d, alpha_s, beta_d, beta_s, Re_c, l_c, v_0


def parse_args():
    parser = argparse.ArgumentParser("Sweep initialize parameter range")
    parser.add_argument("-L", type=float, default=5000.0,
                        help="System size")
    parser.add_argument("-N", type=int, default=1,
                        help="Number of puffs in random init_mode.")
    parser.add_argument("-l_c", type=float, default=l_c,
                        help="Interaction distance")
    parser.add_argument("-l_in", type=float, default=20.,
                        help="Splitting distance.")
    parser.add_argument("-T", type=float, default=1e7,
                        help="Total time")
    parser.add_argument("-dt", type=float, default=1e1,
                        help="Timestep")
    parser.add_argument("-v_0", type=float, default=v_0,
                        help="Speed amplification")
    parser.add_argument("-Re_min", type=float, default=Re_c,
                        help="Control parameter MIN")
    parser.add_argument("-Re_max", type=float, default=2100,
                        help="Control parameter MAX")
    parser.add_argument("-Re_N", type=int, default=100,
                        help="Control parameter NUM STEPS")
    parser.add_argument("-D", type=float, default=1e-2,
                        help="Diffusion coefficient.")
    parser.add_argument("-rho", type=float, default=1e-1,
                        help="Initial puff density.")
    parser.add_argument("-S", type=int, default=1,
                        help="Number of samples per parameter set.")
    parser.add_argument("--verbose", action="store_true",
                        help="Verbose output.")
    parser.add_argument("--log_gaps", action="store_true",
                        help="Log gaps.")
    parser.add_argument("--log_events", action="store_true",
                        help="Log events.")
    parser.add_argument("--dump_pos", action="store_true",
                        help="Dump positions.")
    parser.add_argument("--stat_intv", type=int, default=1000,
                        help="Statistics interval.")
    parser.add_argument("--dump_intv", type=int, default=1000,
                        help="Dump interval")
    parser.add_argument("--init_mode",
                        choices=["mf", "random", "equid", "pair", "density"],
                        default="mf", help="Initialization mode.")
    args = parser.parse_args()
    return args


def param2str(key, val):
    if isinstance(val, bool):
        return "{}={}".format(key, "True" if val else "False")
    return "{}={}".format(key, val)


def main():
    args = parse_args()

    Re_ = np.linspace(args.Re_min, args.Re_max, args.Re_N)

    for i, Re in enumerate(Re_):
        params = [
            param2str("L", args.L),
            param2str("N", args.N),
            param2str("v0", args.v_0),
            param2str("lc", args.l_c),
            param2str("lin", args.l_in),
            param2str("alpha_d", alpha_d(Re)),
            param2str("beta_d", beta_d(Re)),
            param2str("alpha_s", alpha_s(Re)),
            param2str("beta_s", beta_s(Re)),
            param2str("t", 0.0),
            param2str("T", args.T),
            param2str("dt", args.dt),
            param2str("D", args.D),
            param2str("rho", args.rho),
            param2str("dump_pos", args.dump_pos),
            param2str("verbose", args.verbose),
            param2str("log_gaps", args.log_gaps),
            param2str("log_events", args.log_events),
            param2str("init_mode", args.init_mode),
            param2str("stat_intv", args.stat_intv),
            param2str("dump_intv", args.dump_intv),
        ]
        for s in range(args.S):
            results_folder = os.path.join(
                "results",
                "sweep_init",
                "L{}_lc{}_lin{}_dt{}_v0{}_D{}".format(
                    args.L, args.l_c, args.l_in, args.dt, args.v_0, args.D),
                "Re{}".format(Re),
                "{}".format(s)
            )
            initial_folder = os.path.join("..", results_folder, "initial")
            if not os.path.exists(initial_folder):
                os.makedirs(initial_folder)
            
            params.append(param2str("results_folder", results_folder))
            
            params_str = "\n".join(params)
            
            with open(os.path.join(initial_folder,
                                   "params.dat"), "w") as paramsfile:
                paramsfile.write(params_str)

            with open(os.path.join(initial_folder,
                                   "state.dat"), "w") as statefile:
                x_ = np.linspace(0, args.L, int(np.round(args.L/args.l_in)), endpoint=False)
                x_ += x_[0]/2
                for i, xi in enumerate(x_):
                    statefile.write("{} {}\n".format(i, xi))


if __name__ == "__main__":
    main()
