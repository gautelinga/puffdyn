import tempfile
import os
import numpy as np

from _puffdyn import (
    _simulate_f2f
)


def pack_state(state_in):
    return " ".join(["{}".format(s) for s in state_in])


def unpack_state(text):
    return np.array([float(x) for x in text.strip().split(" ")])


def pack_parameters(**kwargs):
    output = ""
    for param, val in kwargs.items():
        output += "{}={}\n".format(param, val)
    return output


def unpack_parameters(text):
    params = dict()
    for line in text.split("\n"):
        key, val = line.split("=")
        params[key] = val
    return params


def simulate(state_in,
             L=1000.0,
             dt=1e1,
             t_0=0.0,
             T=1e8,
             v_0=0.2406,
             l_c=12.0,
             l_in=20.0,
             alpha_d=3.07,
             beta_d=-9.24,
             alpha_s=2.70,
             beta_s=1.22,
             D=1e1,
             dump_pos=False,
             verbose=False,
             log_gaps=False,
             log_events=False,
             stat_intv=10,
             dump_intv=10
             ):

    tmpfiles = dict()
    for name in ["state_in", "params_in", "state_out", "params_out"]:
        fh, infile = tempfile.mkstemp(suffix=".dat")
        os.close(fh)
        tmpfiles[name] = infile
    tmpfolder = tempfile.mkdtemp()

    with open(tmpfiles["state_in"], "w") as f:
        f.write(pack_state(state_in))

    with open(tmpfiles["params_in"], "w") as f:
        f.write(pack_parameters(L=L, dt=dt, t=t_0, T=T, v0=v_0,
                                lc=l_c, lin=l_in,
                                alpha_d=alpha_d, beta_d=beta_d,
                                alpha_s=alpha_s, beta_s=beta_s, D=D,
                                dump_pos=dump_pos, verbose=verbose,
                                log_gaps=log_gaps, log_events=log_events,
                                stat_intv=stat_intv, dump_intv=dump_intv
                                ))

    _simulate_f2f(tmpfiles["state_in"],
                  tmpfiles["params_in"],
                  tmpfiles["state_out"],
                  tmpfiles["params_out"],
                  tmpfolder)

    with open(tmpfiles["state_out"], "r") as f:
        contents = f.read()
        state_out = unpack_state(contents)

    with open(tmpfiles["params_out"], "r") as f:
        contents = f.read().strip()
        params_out = unpack_parameters(contents)

    t = params_out["t"]

    return state_out, t
