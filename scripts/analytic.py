import numpy as np
from rates import l_c, v_0
import matplotlib.pyplot as plt


T = 1e5

t = np.linspace(0., T, 400)
x0 = 0*t
x1 = l_c*np.log(1 + v_0/l_c*t)

plt.plot(t, x0)
plt.plot(t, x1)

plt.show()
