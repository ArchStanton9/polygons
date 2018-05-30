from polygon_view import PolygonView
from helpers import extend_file_paths
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.collections import PolyCollection
import matplotlib.animation as animation
import matplotlib.pyplot as plt
import numpy as np

def render(i):
    if i > 0:
        changed = any(p.is_changed for p in polygon_views)
        if not changed:
            print(f"{i} no changes")
            return

    verts = []
    for polygon_view in polygon_views:
        verts.append(polygon_view.fetch_vertices())

    poly = PolyCollection(verts)
    poly.set_array(np.array(colors))

    ax.clear()
    ax.add_collection3d(poly, zs=range(len(verts)), zdir='y')
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.set_xlim(-3, 6)
    ax.set_ylim(0, 8)
    ax.set_zlim(-2, 5)
    ax.grid()


if __name__ == '__main__':
    import sys

    paths =[]
    
    if len(sys.argv) > 1:
        paths = sys.argv[1:]
    else:
        paths = ["./examples/1/results/W_*.txt"]
        
    polygon_views = [PolygonView(p) for p in extend_file_paths(paths)]

    if len(polygon_views) is 0:
        print("No polygons found.")
        sys.exit(-1)

    np.random.seed(196808127)
    fig = plt.figure()
    ax = fig.gca(projection='3d')
    colors = 100 * np.random.rand(len(polygon_views))
    ani = animation.FuncAnimation(fig, render, interval=2000)

    plt.show()
