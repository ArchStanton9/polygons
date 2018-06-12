from polygon_view import PolygonSetView, PolygonView
from helpers import extend_file_paths
from matplotlib.patches import Polygon, PathPatch
from matplotlib.collections import PatchCollection
from matplotlib.path import Path
from matplotlib import style
import matplotlib.animation as animation
import matplotlib.pyplot as plt
import numpy as np


def render_poly(polygon_view: PolygonSetView, ax, i):
    if i > 0 and not polygon_view.is_changed:
        return

    patches = [PathPatch(p, facecolor='r', alpha=0.5) for p in polygon_view.fetch_paths()]
    pc = PatchCollection(patches, True)
    pc.set_color(polygon_view.color)

    
    ax.clear()
    ax.grid()
    ax.add_collection(pc)
    ax.axis('equal')
    plt.tight_layout()


if __name__ == '__main__':
    fig = plt.figure()
    
    ax1 = fig.add_subplot(121)
    p_poly = PolygonSetView('./examples/spiral.txt', color='green')
    render_poly(p_poly, ax1, 0)
    plt.title('2083 + 2041 вершин')

    ax2 = fig.add_subplot(122)
    q_poly = PolygonSetView('./examples/4/results/W_00000.txt', color='red')
    render_poly(q_poly, ax2, 0)
    plt.title('111 + 147 вершин')

    plt.show()