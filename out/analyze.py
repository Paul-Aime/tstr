import os
import pandas as pd
import matplotlib.pyplot as plt

import fileinput



def main():
    fs = 44100
    Ts = 1/fs

    suffix = "_tmp"

    rows_fname = "rows_buffs_size" + suffix + ".csv"
    cols_fname = "cols_irs_size" + suffix + ".csv"
    buffs_fname = "n_proc_buffers" + suffix + ".csv"
    proc_fname = "proc_duration" + suffix + ".csv"

    # Pre-processing to remove unwanted last comma
    # (more easy to remove here than in c++)
    pre_process(rows_fname)
    pre_process(cols_fname)
    pre_process(buffs_fname)
    pre_process(proc_fname)

    rows = pd.read_csv(rows_fname, header=None).to_numpy()
    cols = pd.read_csv(cols_fname, header=None).to_numpy()
    buffs_n = pd.read_csv(buffs_fname, header=None).to_numpy()
    proc = pd.read_csv(proc_fname, header=None).to_numpy()

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
        buff_size = rows[0, i]
        plt.plot(cols.T, p/(buff_size*Ts), '--o',
                 label="buffer_size = {}".format(buff_size))

    plt.hlines(1, xmin=cols[0, 0], xmax=cols[0, -1], linestyles='dashdot')

    plt.xlabel("Impulse response size")
    plt.ylabel("Processing time")
    plt.xticks(cols.T)
    plt.legend()

    plt.savefig(os.path.join('../img/', 'graph' + suffix + '_tmp.png'))
    plt.show()


def pre_process(csv_path):

    for line in fileinput.input(csv_path, inplace=True):
        # The standard output is redirected to the original file within the loop
        print('{}'.format(line.rstrip(',;\n')), end='\n') # for Python 3

if __name__ == "__main__":
    main()
