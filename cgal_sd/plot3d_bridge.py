from mpl_toolkits.mplot3d import Axes3D, art3d
from matplotlib.patches import PathPatch
import matplotlib.animation as animation
from helpers import extend_file_paths, get_cmap
from polygon_view import PolygonSetView
import matplotlib.pyplot as plt
import matplotlib.colors as colors


def add_patches(patches, ax, z):
    for patch in patches:
        ax.add_patch(patch)
        art3d.pathpatch_2d_to_3d(patch, z=z * 10 / len(polygon_views), zdir="y")


def render(ax):
    ax.clear()

    count = len(polygon_views)
    for z in range(count):
        polygon = polygon_views[z]
        patches = [PathPatch(p,
                             facecolor=cmap(z),
                             # edgecolor=colors.to_rgba('gray', 0.8),
                             # linestyle='dotted',
                             linewidth=0.2)
                   for p in polygon.fetch_paths()]
        add_patches(patches, ax, z)

    ax.set_xlim(0, 10)
    ax.set_ylim(0, 10)
    ax.set_zlim(0, 6) # 0,6 for example 1

    ax.yaxis.set_label_text("t", fontsize=32)
    ax.xaxis.set_label_text("x", fontsize=32)
    ax.zaxis.set_label_text("y", fontsize=32)

    ax.set_yticklabels([])
    ax.set_xticklabels([])
    ax.set_zticklabels([])


if __name__ == '__main__':
    import sys

    paths = []
    if len(sys.argv) > 1:
        paths = sys.argv[1:]
    else:
        paths = ["./examples/1/results/W_*.txt"]

    polygon_views = [PolygonSetView(p) for p in extend_file_paths(paths)]

    if len(polygon_views) is 0:
        print("No polygons found.")
        sys.exit(-1)

    fig = plt.figure(figsize=(20, 12))
    ax1 = fig.add_subplot(121, projection='3d')
    ax2 = fig.add_subplot(122, projection='3d')

    cmap = get_cmap(len(polygon_views))

    render(ax1)
    render(ax2)

    # example 1
    ax1.view_init(elev=29., azim=-137)
    ax2.view_init(elev=10., azim=41)

    # example 2
    # ax1.view_init(elev=7., azim=-115)
    # ax2.view_init(elev=17., azim=117)

    # example 3
    # ax1.view_init(elev=16., azim=-125)
    # ax2.view_init(elev=27., azim=140)


    plt.tight_layout()
    plt.show()
    # plt.savefig("E:\\Dev\\master\\images\\example3_bridge.pdf", format="pdf")
