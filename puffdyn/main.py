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
             L, dt, t_0, T,
             v_0, l_c, alpha_d, beta_d, alpha_s, beta_s, D):

    tmpfiles = dict()
    for name in ["state_in", "params_in", "state_out", "params_out"]:
        fh, infile = tempfile.mkstemp(suffix=".dat")
        os.close(fh)
        tmpfiles[name] = infile

    with open(tmpfiles["state_in"], "w") as f:
        f.write(pack_state(state_in))

    with open(tmpfiles["params_in"], "w") as f:
        f.write(pack_parameters(L=L, dt=dt, t_0=t_0, T=T, v_0=v_0,
                                l_c=l_c, alpha_d=alpha_d, beta_d=beta_d,
                                alpha_s=alpha_s, beta_s=beta_s, D=D))

    _simulate_f2f(tmpfiles["state_in"],
                  tmpfiles["params_in"],
                  tmpfiles["state_out"],
                  tmpfiles["params_out"])

    with open(tmpfiles["state_out"], "r") as f:
        contents = f.read()
        state_out = unpack_state(contents)

    with open(tmpfiles["params_out"], "r") as f:
        contents = f.read().strip()
        params_out = unpack_parameters(contents)

    t = params_out["t"]

    return state_out, t
