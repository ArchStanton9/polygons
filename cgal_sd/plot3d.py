from plot import read_polygons
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.collections import PolyCollection
import matplotlib.pyplot as plt
import numpy as np

# Fixing random state for reproducibility
np.random.seed(196808127)

if __name__ == '__main__':
    import sys

    paths = sys.argv[1:]
    verts = read_polygons(paths)

    if not verts:
        print("No polygons found.")
        sys.exit(-1)

    fig = plt.figure()
    ax = fig.gca(projection='3d')

    poly = PolyCollection(verts)
    colors = 100 * np.random.rand(len(verts))
    poly.set_array(np.array(colors))

    ax.add_collection3d(poly, zs=range(len(verts)), zdir='y')
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.set_xlim(-3, 6)
    ax.set_ylim(0, 8)
    ax.set_zlim(-2, 5)
    ax.grid()

    plt.show()
