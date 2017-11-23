import matplotlib.pyplot as plt
import glob
import numpy as np


def main():
    for filename in glob.glob("results/*"):
        data = np.loadtxt(filename)
        t = data[:, 0]
        x = data[:, 1]

        plt.plot(t, x, 'r')
    plt.show()

if __name__ == "__main__":
    main()
