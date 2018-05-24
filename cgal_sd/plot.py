from polygon_view import PolygonWithHolesView
from helpers import extend_file_paths
from matplotlib.patches import Polygon
from matplotlib.collections import PatchCollection
import matplotlib.animation as animation
import matplotlib.pyplot as plt
import numpy as np


def render(i):
    if i > 0:
        changed = any(p.is_changed for p in polygon_views)
        if not changed:
            print(f"{i} no changes")
            return
    
    patches = []
    for polygon_view in polygon_views:            
        vertices = polygon_view.fetch_vertices()
        polygon = Polygon(vertices, True)
        patches.append(polygon)
    
    pc = PatchCollection(patches)
    pc.set_array(np.array(colors))

    ax.clear()
    ax.grid()
    ax.add_collection(pc)
    ax.axis('equal')


if __name__ == '__main__':
    import sys
    
    if len(sys.argv) > 1 and len(sys.argv[1]) > 0:
        filenames = sys.argv[1:]
    else:
        print("Pass filenames for input files.", file=sys.stderr)
        sys.exit(-1)

    polygon_views = [PolygonWithHolesView(p) for p in extend_file_paths(filenames)]

    np.random.seed(1968081271)
    fig, ax = plt.subplots()
    colors = 100 * np.random.rand(len(polygon_views))
    ani = animation.FuncAnimation(fig, render, interval=2000)

    plt.show()
