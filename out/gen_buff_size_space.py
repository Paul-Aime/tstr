import numpy as np
from pprint import pprint

def main():
    # Generate a suitable xspace for impulse buffer size.
    puiss = np.array([[i, 2**i] for i in range(6, 18)])
    diff = puiss[1:, 1] - puiss[:-1, 1]

    print('\nPuissances:')
    pprint(puiss)

    print('\nDifférences:')
    pprint(diff)

    n_pts_per_intvl = 5
    step = (diff/(n_pts_per_intvl+1)).astype(np.int)

    print('\nStep:')
    print(step)

    # Round steps
    step_r = step
    for i, s in enumerate(step_r):
        # Get the number of decimals to the left
        pow10 = len(str(int(s)))

        step_r[i] = np.around(s, decimals=-(pow10-1))

    print('\nStep rounded:')
    pprint(step_r)

    # Round puiss
    puiss_r = puiss[:, 1]
    for i, p in enumerate(puiss_r):
        # Get the number of decimals to the left
        pow10 = len(str(int(p)))
        if (10**pow10)/2 > p:
            pow10 -= 1

        puiss_r[i] = np.around(p, decimals=-pow10+1)

    print('\nPuissances rounded:')
    pprint(puiss_r)

    # Calculate points
    pts = []
    for p, s in zip(puiss_r, step_r):
        for n in range(n_pts_per_intvl):
            pts.append(p+(n+1)*s)

    print('\nPoints:')
    pprint(pts)
    print(len(pts))


    

if __name__ == '__main__':
    main()