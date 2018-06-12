from subprocess import call
import matplotlib.animation as animation
import matplotlib.pyplot as plt
from matplotlib import style
from matplotlib.collections import PatchCollection
from matplotlib.patches import PathPatch

from polygon_view import PolygonSetView


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
    plt.tight_layout()
    on_polygon_rendered(i)


def render(i):
    render_poly(p_poly, axp, i)
    render_poly(q_poly, axq, i)
    render_poly(m_poly, axm, i)


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
            '--m', args.m,
            '-o', args.results
        ])
        print(res)
    except Exception as e:
        print(e)


if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser(description='Plot two polygons P,Q and target set M.')
    parser.add_argument('--p', default="./examples/1/p.txt", help='polyon P')
    parser.add_argument('--q', default="./examples/1/q.txt", help='polyon Q')
    parser.add_argument('--m', default="./examples/1/m.txt", help='polyon M')
    parser.add_argument('--results', '-o', default="./examples/1/results/")
    parser.add_argument('-s', '--style', default='seaborn-bright',
                        help=f'Plot style. Available: {str(", ").join(style.available)}.')

    args = parser.parse_args()
    p_poly = PolygonSetView(args.p, color='green')
    q_poly = PolygonSetView(args.q, color='red')
    m_poly = PolygonSetView(args.m, color='blue')
    style.use(args.style)

    fig = plt.figure()
    axp = plt.subplot2grid((2, 3), (0, 0), title='test')
    axq = plt.subplot2grid((2, 3), (1, 0))
    axm = plt.subplot2grid((2, 3), (0, 1), colspan=2, rowspan=3)

    ani = animation.FuncAnimation(fig, render, interval=2000)
    plt.show()
    # render(0)
    # plt.savefig("E:\\Dev\\master\\images\\example3_pqm.pdf", format="pdf")
    
