import numpy as np
from matplotlib.patches import Polygon
from matplotlib.collections import PatchCollection
import matplotlib.pyplot as plt

def read_line_not_empty(file):
    line = ""
    while len(line) is 0:
        line = file.readline().strip()

    return line


def draw_polygon(filename):

    fig, ax = plt.subplots()
    patches = []

    with open(filename) as file:
        polygons_count = int(read_line_not_empty(file))
        for p in range(polygons_count):
            vertices_count = int(read_line_not_empty(file))
            points = []

            for v in range(vertices_count):
                line = read_line_not_empty(file)
                p1, p2 = line.split()
                points.append([float(p1), float(p2)])

            polygon = Polygon(points, True)
            patches.append(polygon)

            print(points)


    colors = 100 * np.random.rand(len(patches))
    pc = PatchCollection(patches)

    pc.set_array(np.array(colors))
    ax.add_collection(pc)
    ax.grid()
    ax.axis('equal')

    plt.show()

if __name__ == '__main__':
    import sys
    if len(sys.argv) > 1 and len(sys.argv[1]) > 0:
        draw_polygon(sys.argv[1])

    draw_polygon("E:/Dev/cgal_sd/other/result/S_0.txt")
    # draw_polygon("E:/Dev/cgal_sd/other/q.txt")
    # draw_polygon("E:/Dev/cgal_sd/other/p.txt")
    # draw_polygon("E:/Dev/cgal_sd/other/d.txt")