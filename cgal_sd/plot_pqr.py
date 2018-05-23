from polygon_view import PolygonView
from matplotlib.patches import Polygon
from matplotlib.collections import PatchCollection
from matplotlib import style
import matplotlib.animation as animation
import matplotlib.pyplot as plt
import numpy as np
import glob

np.random.seed(321421)


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


def render_poly(polygon_view: PolygonView, ax, i):
    if i > 0 and not polygon_view.is_changed:
        return

    vertices = polygon_view.fetch_vertices()
    pc = PatchCollection([Polygon(vertices, True)])
    pc.set_color(polygon_view.color)
    
    ax.clear()
    ax.grid()
    ax.add_collection(pc)
    ax.axis('equal')


def render(i):
    render_poly(p_poly, axp, i)
    render_poly(q_poly, axq, i)
    render_poly(r_poly, axr, i)


if __name__ == '__main__':
    p_poly = PolygonView("C:/Users/Arch Stanton/Desktop/an.txt", color='blue')
    q_poly = PolygonView("C:/Users/Arch Stanton/Desktop/aq.txt", color='green')
    r_poly = PolygonView("C:/Users/Arch Stanton/Desktop/ar.txt", color='red')

    style.use('seaborn-bright')
    fig = plt.figure()
    axp = plt.subplot2grid((2,3), (0,0))
    axq = plt.subplot2grid((2,3), (1,0))
    axr = plt.subplot2grid((2,3), (0,1), colspan=2, rowspan=3)
    ani = animation.FuncAnimation(fig, render, interval=2000)

    plt.show()
