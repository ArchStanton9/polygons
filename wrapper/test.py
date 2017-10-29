import build.wrapper.Debug.wrapper as CGAL
import numpy as np
import matplotlib.pyplot as plt


def compare_point_sets():
    random = list(CGAL.random(10000).points())
    halton = list(CGAL.halton(10000).points())

    plt.figure(1)
    plt.subplot(121)
    plt.scatter([p.x for p in random], [p.y for p in random], s=1)
    plt.axis([-0.5, 0.5, -0.5, 0.5])

    plt.subplot(122)
    plt.scatter([p.x for p in halton], [p.y for p in halton], s=1)
    plt.axis([-0.5, 0.5, -0.5, 0.5])

    plt.show()


def triangulation():
    ps = list(CGAL.halton(10000).points())
    x = np.array([p.x for p in ps])
    y = np.array([p.y for p in ps])

    plt.triplot(x, y)
    plt.show()


def area_histogram():
    tr = CGAL.Triangulation(CGAL.random(10000))
    triangles = list(tr.triangles())
    print(len(triangles))
    
    data = np.array([t.area() for t in triangles])
    plt.hist(data, 100)
    plt.show()

    
def angle_histogram():
    tr = CGAL.Triangulation(CGAL.random(10000))
    triangles = list(tr.triangles())
    print(len(triangles))

    data = np.array([t.angle() for t in triangles])
    plt.hist(data, 60)
    plt.show()
    pass


if __name__ == '__main__':
    # compare_point_sets()
    # triangulation()
    # area_histogram()
    # angle_histogram()
    pass
