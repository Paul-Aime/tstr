import pandas as pd
import matplotlib.pyplot as plt


def main():
    fs = 44100
    Ts = 1/44100
    suffix = "_t2"
    rows = pd.read_csv("rows_buffs_size"+ suffix + ".csv", header=None).to_numpy()
    cols = pd.read_csv("cols_irs_size"+ suffix + ".csv", header=None).to_numpy()
    buffs_n = pd.read_csv("n_proc_buffers"+ suffix + ".csv", header=None).to_numpy()
    proc = pd.read_csv("proc_duration"+ suffix + ".csv", header=None).to_numpy()

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
        plt.plot(cols.T, p/(buff_size*Ts), '--o', label="buffer_size = {}".format(buff_size))

    plt.hlines(1, xmin=cols[0, 0], xmax=cols[0, -1], linestyles='dashdot')


    plt.xlabel("Impulse response size")
    plt.ylabel("Processing time")
    plt.xticks(cols.T)
    plt.legend()
    plt.show()


if __name__ == "__main__":
    main()
