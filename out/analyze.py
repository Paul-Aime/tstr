import pandas as pd
import matplotlib.pyplot as plt


def main():
    rows = pd.read_csv("rows_buffs_size.csv", header=None).to_numpy()
    cols = pd.read_csv("cols_irs_size.csv", header=None).to_numpy()
    buffs_n = pd.read_csv("n_proc_buffers.csv", header=None).to_numpy()
    proc = pd.read_csv("proc_duration.csv", header=None).to_numpy()

    print(rows)
    print('\n')
    print(cols)
    print('\n')
    print(buffs_n)
    print('\n')
    print(proc)
    print('\n')

    plt.figure()
    for i, p in enumerate(proc):
        plt.plot(cols.T, p, '--o', label="buffer_size = {}".format(rows[0, i]))
    plt.xlabel("Impulse response size")
    plt.ylabel("Processing time")
    plt.legend()
    plt.show()


if __name__ == "__main__":
    main()
