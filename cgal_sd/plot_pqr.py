from polygon_view import PolygonSetView
from subprocess import call
from matplotlib import style
from matplotlib.patches import Polygon, PathPatch
from matplotlib.collections import PatchCollection
import matplotlib.animation as animation
import matplotlib.pyplot as plt


def render_poly(polygon_view: PolygonSetView, ax, i):
    if i > 0 and not polygon_view.is_changed:
        return

    patches = [PathPatch(p) for p in polygon_view.fetch_paths()]
    pc = PatchCollection(patches)
    pc.set_color(polygon_view.color)

    ax.clear()
    ax.grid()
    ax.add_collection(pc)
    ax.axis('equal')
    on_polygon_rendered(i)


def render(i):
    render_poly(p_poly, axp, i)
    render_poly(q_poly, axq, i)
    render_poly(r_poly, axr, i)


def on_polygon_rendered(i):
    """
    Start program to recalculate bridge, if polygon P,Q or M was changed.
    :param i: animation step number 
    """
    
    if i < 1:
        return

    try:
        res = call([
            './build/main.exe',
            '--p', args.p,
            '--q', args.q,
            '--r', args.r,
            '-o', args.results
        ])
        print(res)
    except Exception as e:
        print(e)


if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser(description='Plot two polygons P,Q and target set R.')
    parser.add_argument('--p', default="./examples/4/p.txt", help='polyon P')
    parser.add_argument('--q', default="./examples/4/q.txt", help='polyon Q')
    parser.add_argument('--r', default="./examples/4/r.txt", help='polyon R')
    parser.add_argument('--results', '-o', default="./examples/3/results/")
    parser.add_argument('-s', '--style', default='seaborn-bright',
                        help=f'Plot style. Available: {str(", ").join(style.available)}.')

    args = parser.parse_args()
    p_poly = PolygonSetView(args.p, color='blue')
    q_poly = PolygonSetView(args.q, color='green')
    r_poly = PolygonSetView(args.r, color='red')
    style.use(args.style)

    fig = plt.figure()
    axp = plt.subplot2grid((2, 3), (0, 0))
    axq = plt.subplot2grid((2, 3), (1, 0))
    axr = plt.subplot2grid((2, 3), (0, 1), colspan=2, rowspan=3)
    ani = animation.FuncAnimation(fig, render, interval=2000)

    plt.show()
