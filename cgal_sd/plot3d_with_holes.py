from matplotlib.patches import Circle, PathPatch
from mpl_toolkits.mplot3d import Axes3D
import mpl_toolkits.mplot3d.art3d as art3d
from matplotlib.patches import PathPatch
import matplotlib.animation as animation
from matplotlib.transforms import Affine2D
from helpers import extend_file_paths, get_cmap
from polygon_view import PolygonWithHolesView
import numpy as np
import matplotlib.pyplot as plt

paths = ['./examples/1/results/W_*.txt']
polygon_views = [PolygonWithHolesView(p) for p in extend_file_paths(paths)]


def add_patch(patch, z):
    ax.add_patch(patch)
    art3d.pathpatch_2d_to_3d(patch, z=z, zdir="y")


def render(i):
    if i > 0:
        changed = any(p.is_changed for p in polygon_views)
        if not changed:
            print(f"{i} no changes")
            return

    ax.clear()
    
    count = len(polygon_views)
    for z in range(count):
        polygon = polygon_views[z]
        patch = PathPatch(polygon.fetch_path(), facecolor=cmap(z), linewidth=0.2)
        add_patch(patch, z)


    ax.set_xlim(-2, 8)
    ax.set_ylim(0, 10)
    ax.set_zlim(-1, 6)


if __name__ == '__main__':
    import sys

    paths = []
    if len(sys.argv) > 1:
        paths = sys.argv[1:]
    else:
        paths = ["./examples/2/results/W_*.txt"]

    polygon_views = [PolygonWithHolesView(p) for p in extend_file_paths(paths)]

    if len(polygon_views) is 0:
        print("No polygons found.")
        sys.exit(-1)

    np.random.seed(196808127)
    fig = plt.figure()
    ax = fig.gca(projection='3d')

    cmap = get_cmap(len(polygon_views))
    ani = animation.FuncAnimation(fig, render, interval=2000)

    plt.show()
