import puffdyn
import numpy as np
import matplotlib.pyplot as plt
from rates import alpha_d, alpha_s, beta_d, beta_s, Re_c, equilibrium_spacing, v_0, l_c

Re = 2040
L = 1e3
T = 2e8

l_eq = equilibrium_spacing(Re)
N0 = np.round(L/l_eq)

state0 = np.linspace(0., L, N0, endpoint=False)
state, t, out = puffdyn.simulate(state0,
                                 alpha_d=alpha_d(Re), beta_d=beta_d(Re),
                                 alpha_s=alpha_s(Re), beta_s=beta_s(Re),
                                 L=L, T=T, D=1e-2, v_0=v_0, l_c=l_c,
                                 full_output=True)

print(state, t)

plt.plot(out[:, 0], out[:, 2])
plt.show()
