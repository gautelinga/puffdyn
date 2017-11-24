import matplotlib.pyplot as plt
import glob
import numpy as np
import argparse


def parse_args():
    parser = argparse.ArgumentParser(description="Plot")
    parser.add_argument("folder", type=str, help="Folder")
    return parser.parse_args()


def main():
    args = parse_args()

    series = []
    for filename in glob.glob(args.folder + "/*"):
        data = np.loadtxt(filename)
        print filename
        t = data[:, 0]
        x = data[:, 1]
        series.append((t, x))

    for t, x in series:
        plt.plot(t, x, 'r')
    plt.show()

if __name__ == "__main__":
    main()