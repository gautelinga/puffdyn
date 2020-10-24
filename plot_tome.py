import argparse
import matplotlib.pyplot as plt
import numpy as np


parser = argparse.ArgumentParser(description="plot")
parser.add_argument("infile", type=str, help="Input file")
args = parser.parse_args()

data = np.loadtxt(args.infile)

t = data[:, 0]
n = data[:, 2]

n1 = np.mean(n)
n2 = np.mean(n**2)
n4 = np.mean(n**4)

nstd = np.sqrt(n2-n1**2)

print(n1, n2, n4)

plt.plot(t, n)
plt.plot([t[0], t[-1]], [n1, n1])
plt.plot([t[0], t[-1]], [n1+nstd, n1+nstd])
plt.plot([t[0], t[-1]], [n1-nstd, n1-nstd])
plt.show()

ncorr = np.zeros(len(t)//2)
for i in range(len(t)//2):
    ncorr[i] = np.mean((n[:len(n)-i]-n.mean())*(n[i:]-n.mean()))

plt.plot(t[:len(ncorr)], ncorr)
plt.show()

bins = np.arange(0, n.max() + 1.5)-0.5
plt.hist(n, bins)
plt.show()

hist, x_edges = np.histogram(n)
plt.plot(x_edges[1:], hist)
plt.show()
