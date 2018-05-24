import matplotlib.pyplot as plt
from matplotlib.patches import PathPatch
from matplotlib.path import Path
from polygon_view import PolygonWithHolesView


"""
# PathPatch example
vertices = [(2,2)      ,(2,-2)     ,(-2,-2)    ,(-2,2)     ,(0,0)         ,(1,0)      ,(-1,1)     ,(-1,-0.5)    ,(0,0)    ]
codes = [Path.MOVETO,Path.LINETO,Path.LINETO,Path.LINETO,Path.CLOSEPOLY,Path.MOVETO,Path.LINETO,Path.LINETO,Path.CLOSEPOLY]
path = Path(vertices,codes)
patch = PathPatch(path)

"""


if __name__ == '__main__':
    import sys
    if len(sys.argv) > 1:
        filename = sys.argv[1]
    else:
        filename = "./examples/1/results/S_0.txt"

    ax = plt.gca()

    poly = PolygonWithHolesView(filename)
    patch = PathPatch(poly.fetch_path())

    ax.add_patch(patch)
    ax.axis('equal')

    plt.show()