from polygon_view import PolygonView
from matplotlib.patches import Polygon
from matplotlib.collections import PatchCollection
import matplotlib.animation as animation
import matplotlib.pyplot as plt
import numpy as np
import glob


def read_polygons(paths):
    result = []
    for path in paths:
        if str(path).count('*') > 0:
            data = read_polygons(glob.glob(path))
            result.extend(data)
            continue
        else:
            polygon = PolygonView(path)
            result.append(polygon)

    return result


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

    polygon_views = []
    if len(sys.argv) > 1 and len(sys.argv[1]) > 0:
        polygon_views = read_polygons(sys.argv[1:])
    else:
        print("Pass filenames for input files.", file=sys.stderr)
        sys.exit(-1)

    np.random.seed(196808127)
    fig, ax = plt.subplots()
    colors = 100 * np.random.rand(len(polygon_views))
    ani = animation.FuncAnimation(fig, render, interval=2000)

    plt.show()
