import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


def main():
    fs = 44100
    Ts = 1/fs

    suffix = "_f"

    rows_fname = "rows_buffs_size" + suffix + ".csv"
    cols_fname = "cols_irs_size" + suffix + ".csv"
    buffs_fname = "n_proc_buffers" + suffix + ".csv"
    proc_fname = "proc_duration" + suffix + ".csv"

    # [:, :-1] Remove last value caused by trailing comma
    rows = pd.read_csv(rows_fname, header=None).to_numpy()[:, :-1]
    cols = pd.read_csv(cols_fname, header=None).to_numpy()[:, :-1]
    buffs_n = pd.read_csv(buffs_fname, header=None).to_numpy()[:, :-1]
    proc = pd.read_csv(proc_fname, header=None).to_numpy()[:, :-1]

    print(rows)
    print('\n')
    print(cols)
    print('\n')
    print(buffs_n)
    print('\n')
    print(proc)
    print('\n')

    proc[proc <= 0] = np.nan

    plt.figure()
    for i in range(2, len(proc)):
        p = proc[i]
        buff_size = rows[0, i]
        print(buff_size*Ts)
        plt.plot(cols.T, p/(buff_size*Ts), '--o',
                 label="buffer_size = {:.0f}".format(buff_size))

    # plt.figure()
    # for i, p in enumerate(proc):
    #     buff_size = rows[0, i]
    #     plt.plot(cols.T, p/(buff_size*Ts), '--o',
    #              label="buffer_size = {:.0f}".format(buff_size))

    plt.hlines(1, xmin=cols[0, 0], xmax=cols[0, -1], linestyles='dashdot')

    plt.xlabel("Impulse response size")
    plt.ylabel("Processing time")
    plt.xticks(cols.T)
    plt.legend()

    plt.savefig(os.path.join('../img/', 'graph' + suffix + '_tmp.png'))
    plt.show()


if __name__ == "__main__":
    main()
